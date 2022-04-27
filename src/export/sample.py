import gramatico
from flask import Flask

g = gramatico.grammar()

""" 
g.add_rule(u"S -> B A")
g.add_rule(u"S -> B")
g.add_rule(u"B -> C")
g.add_rule(u"B -> C D")
g.add_rule(u"A -> E F")
g.add_rule(u"A -> E")

g.add_terminal(u"D", u"d")
g.add_terminal(u"E", u"e")
g.add_terminal(u"F", u"f")

tokens = [u'e', u'f', u'c', u'd']


result = g.check(tokens)



"""

g.add_rule(u"S -> NP VP")
g.add_rule(u"S -> VP NP")
g.add_rule(u"S -> VP")
g.add_rule(u"S -> ADVP S")

g.add_rule(u"DP -> ART")
g.add_rule(u"DP -> ART NP")
g.add_rule(u"DP -> ART NumP")
g.add_rule(u"DP -> ART PossP")

g.add_rule(u"DP -> PRO")
g.add_rule(u"DP -> PRO NP")
g.add_rule(u"DP -> PRO NumP")
g.add_rule(u"DP -> PRO PossP")

g.add_rule(u"NP -> N")
g.add_rule(u"NP -> NP AP")
g.add_rule(u"NP -> AP NP")
g.add_rule(u"NP -> NP PP")
g.add_rule(u"NP -> N PP")

g.add_rule(u"NumP -> NUM NP")
g.add_rule(u"NumP -> NUM PP")

g.add_rule(u"PossP -> PRO NP")
g.add_rule(u"PossP -> NP PRO")
g.add_rule(u"PossP -> PRO NumP")

g.add_rule(u"AP -> A")
g.add_rule(u"AP -> ADVP AP")
g.add_rule(u"AP -> AP ADVP")
g.add_rule(u"AP -> AP PP")
g.add_rule(u"AP -> A PP")
g.add_rule(u"AP -> A CP")

g.add_rule(u"PP -> ADVP PP")
g.add_rule(u"PP -> PREP DP")
g.add_rule(u"PP -> PREP ADVP")
g.add_rule(u"PP -> PREP CP")
g.add_rule(u"PP -> PREP PP")
g.add_rule(u"PP -> PREP")






g.set_delaf_path('delaf_linux.dic')
g.set_delaf_constractions_path('contracoes.cont')
print('loading lexicon...')
ret = g.load_delaf()


def printBT(prefix, tree, node, isLeft):
    if node == -1:
        return
    print(prefix, end='')
    print(tree[node].value)
    suffix = u"      "
    if isLeft:
        print(u"├──", end='')
        suffix = u"│   "
    else:
        print(u"└──", end='')


    printBT(prefix + suffix , tree, tree[node].left, True);
    printBT(prefix + suffix , tree, tree[node].right, False);

if ret:
    #NP VP AP AdvP PP 
    while True:
        sentence = str(input('input sentence: '))
        s = u'um jogador de futebol do benfica'
        tokens = s.split()
        terminals = set()
        newtokens = []
        for token in tokens:
            result = g.get_word_info(token.lower())
            if len(result) == 0:
                print('Token {} not found'.format(token))
                continue
            else:
                prefix = ''
                suffix = ''
                for entry in result:
                    if entry.category == "CON":
                        contractions = g.split_contraction(entry.word)
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
            g.add_terminal(terminal[0], terminal[1])
        
        results = g.check(newtokens)
        for result in results:
            printBT(u"", result, 0, False)
else:
    print('error to loading lexicon...')
        #print (u'Entry {} Category {} '.format(entry.word ,entry.category))
#for i in range(0, result.rows):

#    for j in range(0, result.cols):
#        print(result.get(i, j))