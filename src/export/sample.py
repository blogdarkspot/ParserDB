import gramatico


g = gramatico.grammar()

g.add_rule(u"S -> A B")
g.add_rule(u"S -> B A")
g.add_rule(u"S -> B")
g.add_rule(u"B -> C")
g.add_rule(u"B -> C D")
g.add_rule(u"A -> E F")
g.add_rule(u"A -> E")

g.add_terminal(u"C", u"c")
g.add_terminal(u"D", u"d")
g.add_terminal(u"E", u"e")
g.add_terminal(u"F", u"f")

tokens = [u'e', u'f', u'c', u'd']

result = g.check(tokens)
#for i in range(0, result.rows):
#    for j in range(0, result.cols):
#        print(result.get(i, j))

print(result)

