import base64
import os
import re
from flask import Flask, render_template, request 
import gramatico
from graphviz import Graph, nohtml
import graphviz


app = Flask(__name__)
_gramatico = gramatico.grammar()
_gramatico.delaf_info()
_gramatico.add_rule(u"S -> NP VP")
_gramatico.add_rule(u"S -> VP NP")
_gramatico.add_rule(u"S -> VP")
_gramatico.add_rule(u"S -> ADVP S")

_gramatico.add_rule(u"DP -> ART")
_gramatico.add_rule(u"DP -> ART NP")
_gramatico.add_rule(u"DP -> ART NumP")
_gramatico.add_rule(u"DP -> ART PossP")

_gramatico.add_rule(u"DP -> PRO")
_gramatico.add_rule(u"DP -> PRO NP")
_gramatico.add_rule(u"DP -> PRO NumP")
_gramatico.add_rule(u"DP -> PRO PossP")

_gramatico.add_rule(u"NP -> N")
_gramatico.add_rule(u"NP -> NP AP")
_gramatico.add_rule(u"NP -> AP NP")
_gramatico.add_rule(u"NP -> NP PP")
_gramatico.add_rule(u"NP -> N PP")

_gramatico.add_rule(u"NumP -> NUM NP")
_gramatico.add_rule(u"NumP -> NUM PP")

_gramatico.add_rule(u"PossP -> PRO NP")
_gramatico.add_rule(u"PossP -> NP PRO")
_gramatico.add_rule(u"PossP -> PRO NumP")

_gramatico.add_rule(u"AP -> A")
_gramatico.add_rule(u"AP -> ADVP AP")
_gramatico.add_rule(u"AP -> AP ADVP")
_gramatico.add_rule(u"AP -> AP PP")
_gramatico.add_rule(u"AP -> A PP")
_gramatico.add_rule(u"AP -> A CP")

_gramatico.add_rule(u"PP -> ADVP PP")
_gramatico.add_rule(u"PP -> PREP DP")
_gramatico.add_rule(u"PP -> PREP ADVP")
_gramatico.add_rule(u"PP -> PREP CP")
_gramatico.add_rule(u"PP -> PREP PP")
_gramatico.add_rule(u"PP -> PREP")
app.secret_key = "secret key"
#app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024

path = os.getcwd()
# file Upload
UPLOAD_FOLDER = os.path.join(path, 'uploads')

if not os.path.isdir(UPLOAD_FOLDER):
    os.mkdir(UPLOAD_FOLDER)

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER


def  set_delaf(path):
    _gramatico.set_delaf_path(path)

def set_contractions(path):
    _gramatico.set_delaf_constractions_path(path)

def load_grammar(stream):
    for line in stream:
        tmp = line.decode("utf-8")[:-1]
        #_gramatico.add_rule(tmp)


def load_files(request):
    if 'file_delaf' in request.files:
        delaf = request.files['file_delaf']
        if delaf.filename != '':
            filename = delaf.filename
            delaf.stream
            delaf.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
    if 'file_contracoes' in request.files:
        contracoes = request.files['file_contracoes']
        if contracoes.filename != '':
            filename = contracoes.filename
            contracoes.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
    if 'file_gramatica' in request.files:
        gramatica = request.files['file_gramatica']
        if gramatica.filename != '':
            filename = gramatica.filename
            load_grammar(gramatica.stream)
            gramatica.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))


def load_lexicon(request):
    delaf = False
    contractions = False
    for f in os.listdir(app.config['UPLOAD_FOLDER']):
        if f.endswith('.dic'):
            _gramatico.set_delaf_path(os.path.join(app.config['UPLOAD_FOLDER'], f))
            delaf = True
        if f.endswith('.cont'):
            _gramatico.set_delaf_constractions_path(os.path.join(app.config['UPLOAD_FOLDER'], f))
            contractions = True

    if delaf:
      return _gramatico.load_delaf()
    else:
        return False


def rules():
    rules = _gramatico.rules_info()
    if rules:
        rules = rules.splitlines()
        return rules
    else:
        return ['no rules yet']

def build_image_tree(tree):
    g = graphviz.Digraph('g', filename='btree.gv',
                     node_attr={'shape': 'record', 'height': '.1'})

    for idx, node in enumerate(tree):
        g.node('node{}'.format(idx), nohtml('<f0> |<f1> {}|<f2>'.format(node.value)))
            
    tot = len(tree)
    for idx, node in enumerate(tree):
        if not node.left == -1:
            g.edge('node{}:f0'.format(idx), 'node{}:f1'.format(node.left))
        if not node.right == -1:
            g.edge('node{}:f2'.format(idx), 'node{}:f1'.format(node.right))
        if not node.terminal == '':
            tot = tot + 1
            g.node('node{}'.format(tot), nohtml('<f0> |<f1> {}|<f2>'.format(node.terminal)))
            g.edge('node{}:f1'.format(idx), 'node{}:f1'.format(tot))

    return g

def check(sentence):
    _gramatico.clear_terminals()
    tokens = sentence.split()
    terminals = set()
    newtokens = []
    for token in tokens:
        result = _gramatico.get_word_info(token.lower())
        if len(result) == 0:
            print('Token {} not found'.format(token))
            continue
        else:
            prefix = ''
            suffix = ''
            for entry in result:
                if entry.category == "CON":
                    contractions = _gramatico.split_contraction(entry.word)
                    for contraction in contractions:
                        prefix = contraction.prefix
                        suffix = contraction.suffix
                        terminals.add((contraction.prefix_class, contraction.prefix))
                        terminals.add((contraction.suffix_class, contraction.suffix))
                else:
                    terminals.add((u'{}'.format(entry.category), u'{}'.format(entry.word)))
            if prefix != '' and suffix != '':
                newtokens.append(prefix)
                newtokens.append(suffix)
            else:
                newtokens.append(token)

        
    for terminal in terminals:
        _gramatico.add_terminal(terminal[0], terminal[1])
    
    results = _gramatico.check(newtokens)
    grafs = []
    for result in results:
        grafs.append(build_image_tree(result))
    return grafs


@app.route('/', methods=('GET', 'POST'))
def index():
    graphs = [] 
    if request.method == 'POST':
        if request.form['submit_button'] == 'paths':
           load_files(request) 
        if request.form['submit_button'] == 'lexicon':
            load_lexicon(request)
        if request.form['submit_button'] == 'check':
            text = request.form['text']
            text = text.lower()
            ret = check(text)
            for v in ret:
                graph = v.pipe(format='png')
                graph = base64.b64encode(graph).decode('utf-8')
                graphs.append(graph)

    delaf_info = _gramatico.delaf_info()
    r = rules()

    return render_template('index.html', delaf_info=delaf_info, rules=r, graphs=graphs)