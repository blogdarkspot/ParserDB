#include "constituency.hpp"
#include "core/tokens.hpp"
#include "rules.hpp"
#include "pcfg.hpp"
#include "cky.hpp"
#include "igrammar.hpp"
#include "input_rules.hpp"
#include "utils/matrix.hpp"
#include "lexicon/lexicon.h"
#include "lexicon/ceten_folha/ceten_folha.hpp"

#include <cwctype>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <utility>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

void printBT(const std::wstring& prefix, const std::shared_ptr<grammar::cfg::symbol> node, bool isLeft)
{
    if( node != nullptr )
    {
        std::wcout << prefix;

        std::wcout << (isLeft ? L"├──" : L"└──" );

        bool is_terminal = node->is_terminal;
        // print the value of the node
        std::wcout << node->value <<  (is_terminal ? L"──" : L"\n");

        if(is_terminal)
        {
            std::wcout << (std::static_pointer_cast<grammar::cfg::lexicon>(node))->lex << std::endl;
        }
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->left, true);
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->right, false);
    }
}

void printBT(const std::shared_ptr<grammar::cfg::symbol> node)
{
    printBT(L"", node, false);    
}

namespace grammar{ 

struct LexiconEntry
{
    std::wstring word;
    std::wstring lemma;
    std::wstring category;
};

class grammar
{
  public:

    grammar() : _pcfg(new cfg::PCFG()),
    _cky(new parser::cky()),
    _lexicon(new lexicon::lexicon())

    {
        _pcfg->set_start_symbol(L"DP");
        _cky->set_cfg(_pcfg);
    }

    void add_rule(std::wstring rule)
    {
        ::grammar::cfg::file file;
        auto r = file.parser(rule);
        if(r != nullptr)
        {
            _pcfg->set_rules(r);
        }
        else {
            std::cout << "rule faile" << std::endl;
        }
    }

    void add_terminal(std::wstring left, std::wstring right)
    {
        std::vector<std::wstring> _right;
        _right.emplace_back(right);
        auto terminal = std::make_shared<::grammar::cfg::ProbabilisticRule>(left, _right, true);
        _pcfg->set_terminals(terminal);
    }

    void  check(std::vector<std::wstring> tokens)
    {
        auto trees = _cky->get_trees(tokens);

        for(const auto& v : trees)
        {
                printBT(v);
        }
    }

    void delaf_path_file(std::string path)
    {
        std::cout << path << std::endl;
        _lexicon->file(path);
    }

    void delaf_constractions_file(std::string path)
    {
        _lexicon->set_contractions_path(path);
    }

    std::vector<std::pair<std::pair<std::wstring, std::wstring>, std::pair<std::wstring, std::wstring>>> split_contraction(std::wstring con)
    {
        auto result = _lexicon->split_contraction(con);
        return result;
    }    
    bool load_delaf()
    {
        return _lexicon->load();
    }

    std::vector<std::shared_ptr<lexicon::LexiconType<std::size_t>>> get_word_info(std::wstring word)
    {

        auto entries = _lexicon->get_word_info(word);
        return entries;    
    }    

    std::wstring rules_info()
    {
        std::wstringstream ss;
        ss << *_pcfg;
        return ss.str();
    }

    std::wstring get_word_desc(std::shared_ptr<lexicon::LexiconType<std::size_t>> entry)
    {
        return _lexicon->get_word_desc(entry);
    }


    ~grammar()
    {
        std::cout << "Grammar Destructor" << std::endl;
    }

  private:
    std::shared_ptr<cfg::PCFG> _pcfg;
    std::shared_ptr<parser::cky> _cky;
    std::shared_ptr<lexicon::lexicon> _lexicon;
};

};

static void tokenize(std::wstring const &str, std::vector<std::wstring> &out)
{
    std::wstring tmp = str;
    size_t pos = 0;
    while ((pos = tmp.find(L" ")) != std::wstring::npos)
    {
        auto token = tmp.substr(0, pos);
        std::transform(token.begin(), token.end(), token.begin(), std::towlower);
        out.emplace_back(token);
        tmp.erase(0, pos + std::wstring(L" ").length());
    }
    if(tmp.size() > 0)
    {
        out.emplace_back(tmp);
    }
}

struct rule
{
    rule() = default;

    rule(const std::wstring &s)
    {
        std::vector<std::wstring> out;
        tokenize(s, out);

        for (int i =0; i < out.size(); ++i)
        {
            if(i == 0)
            {
                left = out[i];
                continue;
            }
            if(out[i] == L"-->")
            {
                continue;
            }
            right.emplace_back(out[i]);
        }
    }

    bool operator<(const rule& r) const
    {
        return std::tie(left, right) < std::tie(r.left, r.right);
    }

    std::wstring left;
    std::vector<std::wstring> right;
};

void cfgTocnf()
{

    std::locale utf8{"en_US.UTF-8"};
    std::wifstream _debug("debug_words.txt");
    std::wstring line;
    std::vector<rule> rs;

    std::set<rule> cnf;
    std::set<std::wstring> no_terminals;

    while (std::getline(_debug, line))
    {
        rule r(line);

        no_terminals.insert(r.left);

        if(r.right.size() == 3)
        {
            rule p, aux;
            p.left = r.left;
            aux.left = L"'"+ r.left;
            aux.right.emplace_back(r.right[0]);
            aux.right.emplace_back(r.right[1]);
            p.right.emplace_back(aux.left);
            p.right.emplace_back(r.right[2]);
            cnf.insert(p);
            cnf.insert(aux);
        }
        else
        {
            cnf.insert(r);
        }
    }

    for(auto& r : cnf)
    {
        if(r.right.size() == 1)
        {
            
        }
    }

    
    std::wofstream _cnffile("grammar.cnf");
    _cnffile.imbue(utf8);

    for(const auto& rcnf : cnf)
    {
        _cnffile << rcnf.left << L" --> ";
        for(int i = 0; i < rcnf.right.size(); ++i)
        {
            _cnffile << rcnf.right[i];

            if(i+1 == rcnf.right.size())
            {
                _cnffile << std::endl;
            }
            else
            {
                _cnffile << L" ";
            }
        }
    }
    _cnffile.close();
}

struct r
{
    std::string cat;
    std::vector<std::string> edges;
    std::string word;

    bool operator<(const r& rhs) const
    {
        return std::tie(cat, edges, word) < std::tie(rhs.cat, rhs.edges, rhs.word);
    }

    size_t rhs_size()
    {
        return edges.size();
    }


    std::string str()
    {
        std::string ret = cat + " --> ";
        for(int i = 0; i < edges.size(); ++i)
        {
            ret += edges[i];

            if(i + 1 == edges.size())
                ret += "\n";
            else
                ret += " ";
        }
        return ret;
    }

    void print(bool a)
    {
        std::cout << cat << " --> ";
        for(auto& ed : edges) 
        {
            std::cout << ed << " ";
        }
        std::cout << std::endl;
    }
};

std::map<std::string, r> rules;

std::vector<r> parser_graph(boost::property_tree::ptree& graph, std::vector<std::pair<std::string,std::string>>&);

std::set<r> filtred_rules;
std::vector<std::vector<std::pair<std::string,std::string>>> sentencas;
void xml()
{
    boost::property_tree::ptree root;
    boost::property_tree::read_xml("propbank_br_v2.xml", root);
    auto corpus = root.get_child("corpus");
    auto y = corpus.get_child("body");
    int tot_sentencas = 0;
    std::vector<std::pair<std::string, std::string>> sentenca;

    for(const auto& z : y)
    {
        auto graph = z.second.get_child("graph");
        auto ruls = parser_graph(graph, sentenca);

        if(sentenca.size() > 0)
        {
            sentencas.emplace_back(sentenca);
        }

        for(auto& r : ruls)
        {
            filtred_rules.insert(r);
        }
    }
    std::cout << "total de sentenças " << sentencas.size() << std::endl;

    std::locale utf8{"en_US.UTF-8"};
    std::ofstream _debug("debug_words.txt");
    std::ofstream _sdebug("debug_sentences.txt");

    int tot = 0;
    int sum_size = 0;

    for(auto frul : filtred_rules)
    {
        ++tot;
        sum_size += frul.rhs_size();
        //f.first.print(_debug);
            _debug << frul.str();
    }

    for(auto set : sentencas)
    {
        for(auto token : set)
        {
            _sdebug << token.first << " ";
        }
        _sdebug << std::endl;
    }

    std::cout << "media de tamanho das regras " <<  sum_size / tot << " total " << tot << std::endl;
} 



std::vector<r> parser_graph(boost::property_tree::ptree& graph, std::vector<std::pair<std::string, std::string>>& sentenca)
{
    auto terminals = graph.get_child("terminals");
    auto no_terminals = graph.get_child("nonterminals");
    rules.clear();
    for(const auto& t : terminals)
    {
        const auto& attrs = t.second.get_child("<xmlattr>");

        r ter;
        std::string id;
        for(const auto& attr : attrs)
        {
            if(std::string(attr.first.data()) == "id")
            {
                id = attr.second.data();
            }else
            if(std::string(attr.first.data()) == "pos")
            {
                ter.cat = attr.second.data();
            }else
            if(std::string(attr.first.data()) == "word")
            {
                ter.word = attr.second.data();
            }
        }
        sentenca.emplace_back(std::make_pair(ter.word, ter.cat));
        rules[id] = ter;
    }

    for(const auto& terminal : no_terminals)
    {
        const auto& attrs = terminal.second.get_child("<xmlattr>");


        std::string id = "";
        r ru;

        for(const auto& attr : attrs)
        {
            auto value = attr.first.data();
            if(std::string(attr.first.data()) == "id")
            {
                id = attr.second.data();
            }

            if(std::string(attr.first.data()) == "cat")
            {
                ru.cat = attr.second.data();
            }
        }

        for(const auto& edge : terminal.second)
        {
            if(std::string(edge.first.data()) == "<xmlattr>") continue;  

            const auto& attrs2 = edge.second.get_child("<xmlattr>");
            for(const auto& attr2 : attrs2)
            {
                if(std::string(attr2.first.data()) == "idref")
                {

                    ru.edges.emplace_back(attr2.second.data());
                }
            }
        }
        rules[id] = ru;
    }

    std::vector<r> ret;

    for(const auto& p : rules)
    {
        r tmp;
        tmp.cat = p.second.cat;

        for(const auto&v : p.second.edges)
        {
            tmp.edges.emplace_back(rules[v].cat);
        }
        if(p.second.edges.size() > 3)
        {
            sentenca.clear();
        }
        if(p.second.edges.size() != 0 && p.second.edges.size() <=3)
            ret.emplace_back(tmp);
    }

    return ret;

}

void readCnf()
{
    std::locale utf8{"en_US.UTF-8"};
    std::wifstream _debug("grammar.cnf");
    std::wstring line;
    std::vector<rule> rs;

    std::set<rule> cnf;
    std::set<std::wstring> tokens;
    
    while (std::getline(_debug, line))
    {
        auto ru = rule(line);
        cnf.insert(ru); 
        tokens.insert(ru.left);
        for(auto& i : ru.right )
        {
            tokens.insert(i);
        }
    }

    for(auto& i : tokens)
    {
        std::wcout << i << std::endl;
    }
}

int main()
{
   //xml();
   //cfgTocnf();
   readCnf();
   std::cout << " finished " << std::endl;
   // ceten_parser();
    /*
    auto g = grammar::grammar();
    g.add_rule(L"S -> NP VP");
    g.add_rule(L"S -> VP NP");
    g.add_rule(L"S -> VP");
    g.add_rule(L"S -> ADVP S");

    g.add_rule(L"DP -> ART");
    g.add_rule(L"DP -> ART NP");
    g.add_rule(L"DP -> ART NumP");
    g.add_rule(L"DP -> ART PossP");

    g.add_rule(L"DP -> PRO");
    g.add_rule(L"DP -> PRO NP");
    g.add_rule(L"DP -> PRO NumP");
    g.add_rule(L"DP -> PRO PossP");

    g.add_rule(L"NP -> N");
    g.add_rule(L"NP -> NP AP");
    g.add_rule(L"NP -> AP NP");
    g.add_rule(L"NP -> NP PP");
    g.add_rule(L"NP -> N PP");

    g.add_rule(L"NumP -> NUM NP");
    g.add_rule(L"NumP -> NUM PP");

    g.add_rule(L"PossP -> POSS NP");
    g.add_rule(L"PossP -> NP POSS");
    g.add_rule(L"PossP -> POSS NumP");

    g.add_rule(L"AP -> A");
    g.add_rule(L"AP -> ADVP AP");
    g.add_rule(L"AP -> AP ADVP");
    g.add_rule(L"AP -> AP PP");
    g.add_rule(L"AP -> A PP");
    g.add_rule(L"AP -> A CP");

    g.add_rule(L"PP -> ADVP PP");
    g.add_rule(L"PP -> PREP DP");
    g.add_rule(L"PP -> PREP ADVP");
    g.add_rule(L"PP -> PREP CP");
    g.add_rule(L"PP -> PREP PP");
    g.add_rule(L"PP -> PREP");

    g.delaf_path_file("delaf_linux.dic");
    g.delaf_constractions_file("contracoes.cont");
    
    auto ret = g.load_delaf();
    while(ret)
    {
        std::wstring msg = L"um jogador de futebol do benfica";
        std::wistringstream iss(msg);
        std::wstring s;
        std::vector<std::wstring> tokens;
        while (std::getline( iss, s, L' ' ) ) {
            tokens.emplace_back(s);
        }

        for(const auto& token : tokens)
        {
            auto lex = g.get_word_info(token);

            std::set<std::wstring> l, r;
            for(const auto u : lex)
            {
                if((*u->category->categoria) == L"CON")
                {
                        auto entry = g.get_word_desc(u);
                        auto result = g.split_contraction(entry);
                        for(auto& v : result)
                        {
                            g.add_terminal(v.first.second, v.first.first);
                            g.add_terminal(v.second.second, v.second.first);
                        }
                }
                else
                {
                    std::wstring sterminal = g.get_word_desc(u);
                    std::wstring snoterminal(*u->category->categoria);

                    auto it1 = l.find(snoterminal);
                    auto it2 = r.find(sterminal);

                    if (it1 != l.end() && it2 != r.end())
                        continue;
                    ;

                    l.insert(snoterminal);
                    r.insert(sterminal);

                    g.add_terminal(snoterminal, sterminal);
                }
            }
        }
        g.check(tokens);
    }

    g.add_terminal(L"PRO", L"aquele");
    g.add_terminal(L"N", L"computador");
//    std::wcout << g.rules_info() << std::endl;
*/
    return 0;
}

void ceten_parser()
{

  //  auto g = grammar::grammar();
  //  g.delaf_path_file("delaf_linux.dic");
  //  g.delaf_constractions_file("contracoes.cont");
    
    //auto ret = g.load_delaf();
    std::locale utf8{"en_US.UTF-8"};
    std::wofstream _debug("debug_words.txt");
    std::wofstream _debug_class("debug_class.txt");
    _debug.imbue(utf8);
    _debug_class.imbue(utf8);

    corpus::folha::decoder decoder;
    decoder.load_file("CETENFolha-1.0_jan2014.cg");
    std::set<std::wstring> checked_classes;
    if(decoder.parser_file())
    {
        auto corpus = decoder.get_corpus();

        for(auto& corpus_u : corpus)
        {
            for(auto& lex : corpus_u->_lexicon)
            {

                std::wstring original_class;
                original_class = lex->category;

                if (lexicon::CategoryType.find(lex->category) != lexicon::CategoryType.end())
                    continue;

                if (lex->category == L"PRP")
                {
                    lex->category = L"PREP";
                }else
                if (lex->category == L"PROP")
                {
                    lex->category = L"N";
                }else
                if (lex->category == L"ADJ")
                {
                    lex->category = L"A";
                }else
                if (lex->category == L"KS")
                {
                    lex->category = L"CONJ";
                }else
                if (lex->category == L"KC")
                {
                    lex->category = L"CONJ";
                }else
                if (lex->category == L"IN")
                {
                    lex->category = L"INTERJ";
                }else
                if (lex->category == L"EC")
                {
                    lex->category = L"PFX";
                }else
                if (lex->category == L"SPEC" || lex->category == L"PERS")
                {
                    lex->category = L"PRO";
                }else
                if (lex->category == L"DET")
                {
                    for (auto sec : lex->secundary_information)
                    {
                        if (sec == L"<artd>" || sec == L"<arti>")
                        {
                            lex->category = L"ART";
                            break;
                        }
                        if (sec == L"<quant>" || sec == L"<dem>" || sec == L"<poss>" || sec == L"<refl>" ||
                            sec == L"<rel>" || sec == L"<interr>")
                        {
                            lex->category = L"PRO";
                        }
                        else
                        {
                            std::wcout << L"unkown categoria " << lex->category  << L" sec: info " << sec << std::endl;
                        }
                    }
                }else {
                    std::wcout << L"unkown categoria " << lex->category << std::endl;
                }
            }
        }
    }
}