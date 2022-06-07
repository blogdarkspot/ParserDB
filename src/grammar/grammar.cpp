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

#include <cstdlib>
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
            if(out[i] == L"->")
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
        std::string ret = cat + " -> ";
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
        std::cout << cat << " -> ";
        for(auto& ed : edges) 
        {
            std::cout << ed << " ";
        }
        std::cout << std::endl;
    }
};

std::map<std::string, r> rules;
std::shared_ptr<grammar::cfg::PCFG> _pcfg;
std::shared_ptr<grammar::parser::cky> _cky;
std::shared_ptr<lexicon::lexicon> _lexicon;

int parser_graph(boost::property_tree::ptree& graph, std::vector<std::pair<std::string,std::string>>&);

std::set<std::string> categorias;
std::set<r> filtred_rules;
std::vector<std::vector<std::pair<std::string,std::string>>> sentencas;


void get_rules()
{
    boost::property_tree::ptree root;
    boost::property_tree::read_xml("propbank_br_v2.xml", root);
    auto corpus = root.get_child("corpus").get_child("body");
    std::vector<std::pair<std::string, std::string>> sentenca;
    for(const auto& z : corpus)
    {
        auto graph = z.second.get_child("graph");
        parser_graph(graph, sentenca);
    }
};

void xml()
{
    boost::property_tree::ptree root;
    boost::property_tree::read_xml("propbank_br_v2.xml", root);
    auto corpus = root.get_child("corpus").get_child("body");
    int tot_sentencas = 0;
    std::vector<std::pair<std::string, std::string>> sentenca;
    int succ = 0;
    int fail = 0;
    _pcfg->set_start_symbol(L"IP");

    std::locale utf8{"en_US.UTF-8"};
    std::ofstream _debug("debug_words.txt");
    std::ofstream _sdebug("debug_sentences.txt");

    for(const auto& z : corpus)
    {
        auto graph = z.second.get_child("graph");
        tot_sentencas += parser_graph(graph, sentenca);
        _pcfg->clear_terminals();
        std::vector<std::wstring> wsentence;
        for(const auto token : sentenca)
        {
                std::wstring w = std::wstring(token.first.begin(), token.first.end());
                wsentence.emplace_back(w);
                std::vector<std::wstring> _right;
                _right.emplace_back(w);
                auto terminal = std::make_shared<::grammar::cfg::ProbabilisticRule>(std::wstring(token.second.begin(), token.second.end()), _right, true);
                _pcfg->set_terminals(terminal);
        }

        auto ret = _cky->get_best_tree(wsentence);

        if(ret != nullptr)
        {
            succ++;
        }
        else
        {
            fail++;
            for(auto t : sentenca)
            {
                _sdebug << t.first << " ";
            }
            _sdebug << std::endl;
            for(auto t : sentenca)
            {
                _sdebug << t.second << " ";
            }
            _sdebug << std::endl;
            _sdebug << std::endl;
        }
        sentenca.clear(); 
        ++tot_sentencas;
    }
    std::cout << "total de sentenças " << tot_sentencas << std::endl;
    
} 

int pro = 0;
int parser_graph(boost::property_tree::ptree& graph, std::vector<std::pair<std::string, std::string>>& sentenca)
{
    auto terminals = graph.get_child("terminals");
    auto no_terminals = graph.get_child("nonterminals");
    rules.clear();
    int i = 0;
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
        rules[id] = ter;
        filtred_rules.insert(ter);
    }

    for(const auto& n : no_terminals)
    {

        r r;
        for(const auto& nt : n.second)
        {
            if (std::string(nt.first.data()) == "<xmlattr>")
            {
                for (const auto attr : nt.second)
                {
                    if (std::string(attr.first.data()) == "cat")
                    {
                        r.cat = attr.second.data();
                    }
                }
            }

            if(std::string(nt.first.data()) == "edge")
            {
                for(const auto attr : nt.second)
                {
                    if (std::string(attr.first.data()) == "<xmlattr>")
                    {
                        for(const auto ed : attr.second)
                        {
                            if(std::string(ed.first.data()) == "label")
                            {
                                r.edges.emplace_back(ed.second.data());
                            }
                        }
                    }
                }
            }
        }
        filtred_rules.insert(r);
    }
    return 1;
}



bool load_cnf()
{
    auto ret = true;
    ::grammar::cfg::file file;
    file.set_path("grammar.cnf");
    ret = file.open();
    if(ret)
    {
        auto rules = file.get_all();
        if(rules.size() == 0)
        {
            ret = false;
        }
        else {
            for(auto rule : rules)
            {
                if(rule != nullptr)
                    _pcfg->set_rules(rule);
            }
        }
    }
    return ret;
}

int main()
{
  //  _pcfg = std::make_shared<grammar::cfg::PCFG>();
  //  _cky = std::make_shared<grammar::parser::cky>();
  //  _lexicon = std::make_shared<lexicon::lexicon>();
  //  _cky->set_cfg(_pcfg);
  // load_cnf();
  // xml();

    get_rules();

   std::cout << " finished " << std::endl;
   return 0;
}