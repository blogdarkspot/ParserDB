#include "grammar/constituency.hpp"
#include "grammar/rules.hpp"
#include "grammar/pcfg.hpp"
#include "grammar/cky.hpp"
#include "grammar/igrammar.hpp"
#include "grammar/input_rules.hpp"
#include "utils/matrix.hpp"
#include "lexicon/lexicon.h"
#include "lexicon/core/tokens.hpp"

#include <boost/python.hpp>
#include <boost/python/list.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>



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

void printBT(const std::shared_ptr<::grammar::cfg::symbol> node)
{
    printBT(L"", node, false);    
}


namespace grammar{ 

struct LexiconEntry
{
    std::wstring word;
    std::wstring lemma;
    std::wstring category;
    std::wstring type;
};

struct TreeNote
{
    std::wstring value;
    std::wstring terminal;
    int left = -1;
    int right = -1;
};

struct Contraction
{
    std::wstring prefix;
    std::wstring prefix_class;
    std::wstring suffix;
    std::wstring suffix_class;
};

int build_tree(const std::shared_ptr<grammar::cfg::symbol> node, std::vector<TreeNote>& result)
{
    int ret = -1;
    if(node != nullptr)
    {
        TreeNote value;
        value.value =  node->value;
        if(node->is_terminal)
        {
            value.terminal = (std::static_pointer_cast<grammar::cfg::lexicon>(node))->lex;
        }
        result.emplace_back(value);
        int pos = result.size() - 1;
        result[pos].left = build_tree(node->left, result);
        result[pos].right = build_tree(node->right, result);
        return pos;
    }
    return ret;
}

class grammar
{
  public:

    grammar() : _pcfg(new cfg::PCFG()),
    _cky(new parser::cky()),
    _lexicon(new lexicon::lexicon())

    {
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

    void clear_rules()
    {
        _pcfg->clear_rules();
    }

    void clear_terminals()
    {
        _pcfg->clear_terminals();
    }

    void add_terminal(std::wstring left, std::wstring right)
    {
        std::vector<std::wstring> _right;
        _right.emplace_back(right);
        auto terminal = std::make_shared<::grammar::cfg::ProbabilisticRule>(left, _right, true);
        terminal->set_probability(1);
        _pcfg->set_terminals(terminal);
    }

    void add_terminal_ca(std::wstring left, std::wstring right, std::wstring canonical)
    {
        std::vector<std::wstring> _right;
        _right.emplace_back(right);
        _right.emplace_back(canonical);
        auto terminal = std::make_shared<::grammar::cfg::ProbabilisticRule>(left, _right, true);
        //_pcfg->set_canonical_terminal(right, left, canonical);
        terminal->set_probability(1);
        _pcfg->set_terminals(terminal);
    }

    boost::python::list check(boost::python::list tokens)
    {
        auto len = boost::python::len(tokens);
        std::vector<std::wstring> vtokens;


        
        for(int i = 0; i < len; i++)
        {
          vtokens.emplace_back(boost::python::extract<std::wstring>(tokens[i]));
        }

        auto trees = _cky->get_trees(vtokens);

        boost::python::list ret2;

        for(const auto& v : trees)
        {
            std::vector<TreeNote> result;
            build_tree(v, result);
            boost::python::list pr;
            for(auto r : result)
            {
                pr.append(r);
            }
            ret2.append(pr);
        }

        return ret2;
    }
    
    boost::python::list best_tree(boost::python::list tokens)
    {
        auto len = boost::python::len(tokens);
        std::vector<std::wstring> vtokens;

        
        for(int i = 0; i < len; i++)
        {
          vtokens.emplace_back(boost::python::extract<std::wstring>(tokens[i]));
        }

        auto tree = _cky->get_best_tree(vtokens);

        boost::python::list ret2;

            std::vector<TreeNote> result;
            build_tree(tree, result);
            boost::python::list pr;
            for(auto r : result)
            {
                pr.append(r);
            }
            ret2.append(pr);

        return ret2;
    }

    void delaf_path_file(std::string path)
    {
        _lexicon->file(path);
    }

    void delaf_constractions_file(std::string path)
    {
        _lexicon->set_contractions_path(path);
    }


    void cnf_path(std::string path)
    {
        _cnf_path = path;
    }
    
    bool load_cnf()
    {
        auto ret = true;
        ::grammar::cfg::file file;
        file.set_path(_cnf_path);
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

    void set_start_symbol(std::wstring symbol)
    {
        _pcfg->set_start_symbol(symbol);
    }

    bool load_delaf()
    {
        return _lexicon->load();
    }

    boost::python::list split_contraction(std::wstring con)
    {
        auto result = _lexicon->split_contraction(con);
        boost::python::list ret;
        for(const auto& r : result)
        {
            Contraction tmp;
            tmp.prefix = r.first.first;
            tmp.prefix_class = r.first.second;
            tmp.suffix = r.second.first;
            tmp.suffix_class = r.second.second;
            ret.append(tmp);
        }
        return ret;
    }

    boost::python::list get_word_info(std::wstring word)
    {
        boost::python::list ret;

        auto entries = _lexicon->get_word_info(word);
        
        std::set<std::wstring> left;
        std::set<std::wstring> right;

        for(int i = 0; i < entries.size(); ++i)
        {
            LexiconEntry tmp;
            tmp.word = _lexicon->get_word_desc(entries[i]);
            tmp.lemma = _lexicon->get_lemma_desc(entries[i]);
            tmp.category = *(entries[i]->category->categoria);
            if(tmp.category == L"PRO")
            {
                tmp.type = *(entries[i]->category->tipo);
            }
            else {
                tmp.type = L"";
            }
            ret.append(tmp);
        }

        return ret;
    }    

    std::wstring rules_info()
    {
        std::wstringstream ss;
        ss << *_pcfg << std::endl;
        return ss.str();
    }

    lexicon::DelafInfo delaf_info()
    {
        return _lexicon->get_delaf_info();
    }

    ~grammar()
    {
        std::cout << "Grammar Destructor" << std::endl;
    }

  private:
    std::shared_ptr<cfg::PCFG> _pcfg;
    std::shared_ptr<parser::cky> _cky;
    std::shared_ptr<lexicon::lexicon> _lexicon;
    std::string _cnf_path;
};



};


BOOST_PYTHON_MODULE(gramatico)
{
  using namespace boost::python;
  class_<grammar::grammar>("grammar")
    .def("add_rule", &grammar::grammar::add_rule)
    .def("rules_info", &grammar::grammar::rules_info)
    .def("add_terminal", &grammar::grammar::add_terminal)
    .def("add_terminal_ca", &grammar::grammar::add_terminal_ca)
    .def("check", &grammar::grammar::check)
    .def("best_tree", &grammar::grammar::best_tree)
    .def("load_delaf", &grammar::grammar::load_delaf)
    .def("get_word_info", &grammar::grammar::get_word_info)
    .def("set_delaf_path", &grammar::grammar::delaf_path_file)
    .def("clear_rules", &grammar::grammar::clear_rules)
    .def("clear_terminals", &grammar::grammar::clear_terminals)
    .def("set_delaf_constractions_path", &grammar::grammar::delaf_constractions_file)
    .def("split_contraction", &grammar::grammar::split_contraction)
    .def("delaf_info", &grammar::grammar::delaf_info)
    .def("load_cnf", &grammar::grammar::load_cnf)
    .def("set_cnf", &grammar::grammar::cnf_path)
    .def("set_start_symbol", &grammar::grammar::set_start_symbol);

  
  class_<grammar::LexiconEntry>("LexiconEntry")
    .def_readonly("word", &grammar::LexiconEntry::word)
    .def_readonly("lemma", &grammar::LexiconEntry::lemma)
    .def_readwrite("category", &grammar::LexiconEntry::category)
    .def_readonly("type", &grammar::LexiconEntry::type);

   class_<lexicon::DelafInfo>("DelafInfo")
    .def_readonly("total_entry", &lexicon::DelafInfo::total_entry)
    .def_readonly("total_lemma", &lexicon::DelafInfo::total_lemma)
    .def_readonly("total_contractions", &lexicon::DelafInfo::total_contractions)
    .def_readonly("total_rules", &lexicon::DelafInfo::total_lexicon)
    .def_readonly("is_delaf_loading", &lexicon::DelafInfo::is_delaf_loading)
    .def_readonly("is_contractions_loading", &lexicon::DelafInfo::is_contractions_loading);
    
    class_<grammar::TreeNote>("TreeNode")
    .def_readonly("value", &grammar::TreeNote::value)
    .def_readonly("left", &grammar::TreeNote::left)
    .def_readonly("right", &grammar::TreeNote::right)
    .def_readonly("terminal", &grammar::TreeNote::terminal);

    class_<grammar::Contraction>("Contraction")
    .def_readonly("prefix", &grammar::Contraction::prefix)
    .def_readonly("prefix_class", &grammar::Contraction::prefix_class)
    .def_readonly("suffix", &grammar::Contraction::suffix)
    .def_readonly("suffix_class", &grammar::Contraction::suffix_class);
}
