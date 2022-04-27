#include "constituency.hpp"
#include "rules.hpp"
#include "pcfg.hpp"
#include "cky.hpp"
#include "igrammar.hpp"
#include "input_rules.hpp"
#include "utils/matrix.hpp"
#include "lexicon/lexicon.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <sstream>




void printBT(const std::wstring& prefix, const std::shared_ptr<grammar::cfg::symbol<std::wstring>> node, bool isLeft)
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
            std::wcout << (std::static_pointer_cast<grammar::cfg::lexicon<std::wstring>>(node))->lex << std::endl;
        }
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->left, true);
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->right, false);
    }
}

void printBT(const std::shared_ptr<grammar::cfg::symbol<std::wstring>> node)
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

    grammar() : _pcfg(new cfg::PCFG<std::wstring, std::vector>()),
    _cky(new parser::cky<std::wstring>()),
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
        auto terminal = std::make_shared<::grammar::cfg::ProbabilisticRule<std::wstring, std::vector>>(left, _right, true);
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
    std::shared_ptr<cfg::PCFG<std::wstring, std::vector>> _pcfg;
    std::shared_ptr<parser::iparser<std::wstring>> _cky;
    std::shared_ptr<lexicon::lexicon> _lexicon;
};

};

int main()
{
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
    return 0;
}
