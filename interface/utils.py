from graphviz import Graph, nohtml
import graphviz

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

def get_lexicon(word, gramatico):
    word = word.lower()
    result = gramatico.get_word_info(word)

    obj = object()

    if len(result) == 0:
        return None
    for entry in result:
        obj.categoria.append(entry.category)
        obj.canonicals.append(entry.lemma)
    return obj

def build_terminals(sentence, gramatico):
    gramatico.clear_terminals()
    tokens = sentence.split()
    terminals = set()
    newtokens = []

    for token in tokens:
        result = gramatico.get_word_info(token.lower())
        if len(result) == 0:
            print('Token {} not found'.format(token))
            break 
        else:
            prefix = ''
            suffix = ''
            for entry in result:
                if entry.category == "CON":
                    contractions = gramatico.split_contraction(entry.word)
                    for contraction in contractions:
                        prefix = contraction.prefix
                        suffix = contraction.suffix
                        terminals.add((contraction.prefix_class, contraction.prefix))
                        terminals.add((contraction.suffix_class, contraction.suffix))
                else:
                    entry = parser_tokens(entry)
                    terminals.add((u'{}'.format(entry.category), u'{}'.format(entry.word)))
            if prefix != '' and suffix != '':
                newtokens.append(prefix)
                newtokens.append(suffix)
            else:
                newtokens.append(token)
        
    for terminal in terminals:
        gramatico.add_terminal(terminal[0], terminal[1])
    
    results = gramatico.check(newtokens)
    return results

def get_best_tree(sentence, gramatico):
    gramatico.clear_terminals()
    tokens = sentence.split()
    terminals = set()
    newtokens = []

    for token in tokens:
        result = gramatico.get_word_info(token.lower())
        if len(result) == 0:
            print('Token {} not found'.format(token))
            break
        else:
            prefix = ''
            suffix = ''
            for entry in result:
                if entry.category == "CON":
                    contractions = gramatico.split_contraction(entry.word)
                    for contraction in contractions:
                        prefix = contraction.prefix
                        suffix = contraction.suffix
                        terminals.add((contraction.prefix_class, contraction.prefix))
                        terminals.add((contraction.suffix_class, contraction.suffix))
                else:
                    entry = parser_tokens(entry)
                    terminals.add((u'{}'.format(entry.category), u'{}'.format(entry.word)))
            if prefix != '' and suffix != '':
                newtokens.append(prefix)
                newtokens.append(suffix)
            else:
                newtokens.append(token)
        
    for terminal in terminals:
        gramatico.add_terminal(terminal[0], terminal[1])
    
    result = gramatico.best_tree(newtokens)
    print(*result)
    return result

def parser_tokens(entry):
    if entry.category == "PRO":
        if entry.type == "Pos":
            entry.category = "Poss"
        else:
            entry.category = "DET"
        if entry.category == "ART":
            entry.category = "DET"
    return entry