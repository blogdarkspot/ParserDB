import base64
import os
import re
from flask import Flask, render_template, request, redirect, url_for

import Files
import utils
import gramatico



_gramatico = gramatico.grammar()
files = Files.Files()

app = Flask(__name__)
app.secret_key = "secret key"
app.config['TEMPLATES_AUTO_RELOAD'] = True

path = os.getcwd()

# file Upload
UPLOAD_FOLDER = os.path.join(path, 'uploads')

if not os.path.isdir(UPLOAD_FOLDER):
    os.mkdir(UPLOAD_FOLDER)

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER


def set_paths():
    delaf = files.get_delaf_path(app)
    grammar = files.get_grammar_path(app)
    contractions = files.get_contractions_path(app)
    terminals = files.get_terminals_path(app)
    if delaf != '':
        _gramatico.set_delaf_path(delaf)
    if grammar != '':
        _gramatico.set_cnf(grammar)
    if contractions != '':
        _gramatico.set_delaf_constractions_path(contractions)

def load_files():
    _gramatico.clear_rules()
    _gramatico.load_cnf()
    info = _gramatico.delaf_info()
    if not info.is_delaf_loading:
        _gramatico.load_delaf()


def rules():
    rules = _gramatico.rules_info()
    if rules:
        rules = rules.splitlines()
        return rules
    else:
        return ['no rules yet']

def check(sentence):
    results = utils.build_terminals(sentence, _gramatico)
    grafs = []
    for result in results:
        grafs.append(utils.build_image_tree(result))
    return grafs

def best_tree(sentence):
    results = utils.get_best_tree(sentence, _gramatico)
    grafs = []
    for result in results:
        grafs.append(utils.build_image_tree(result))
    return grafs

@app.route('/config', methods=('GET', 'POST'))
def config():
    files.save_files(app, request)
    set_paths()
    load_files()
    start_symbol = request.form.get('start_symbol')
    if start_symbol:
        _gramatico.set_start_symbol(start_symbol)
    return redirect('/')


@app.route('/dictionary', methods=('GET', 'POST'))
def dictionary():
    if request.method == 'POST':
        if request.form['submit_button'] == 'check':
            text = request.form['text']
            text = text.lower()
            result = utils.get_lexicon(text, _gramatico)
    delaf_info = _gramatico.delaf_info()
    r = rules()
    if result:
        return redirect('/#lexicoModal', delaf_info=delaf_info, rules=r, lexicon=result)
    else:
        return redirect('/#lexicoModal', delaf_info=delaf_info, rules=r)

@app.route('/', methods=('GET', 'POST'))
def index():
    graphs = [] 
    if request.method == 'POST':
        if request.form['submit_button'] == 'check':
            text = request.form['text']
            text = text.lower()
            ret = None
            if request.form['tree'] == 'best_tree':
                ret = best_tree(text)
                print(*ret)
            else:
                ret = check(text)
            for v in ret:
                graph = v.pipe(format='png')
                graph = base64.b64encode(graph).decode('utf-8')
                graphs.append(graph)

    delaf_info = _gramatico.delaf_info()
    r = rules()

    return render_template('index.html', delaf_info=delaf_info, rules=r, graphs=graphs)