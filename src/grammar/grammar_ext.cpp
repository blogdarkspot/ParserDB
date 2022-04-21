#include <boost/python.hpp>

#include "constituency.hpp"
#include "rules.hpp"
#include "pcfg.hpp"
#include "cky.hpp"
#include "igrammar.hpp"
#include "input_rules.hpp"
#include "utils/matrix.hpp"
#include <boost/python/list.hpp>
#include <iostream>
#include <memory>
#include <set>
#include <string>


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
class grammar
{
  public:

    grammar() : _pcfg(new cfg::PCFG<std::wstring, std::vector>()),
    _cky(new parser::cky<std::wstring>())
    {
        _pcfg->set_start_symbol(L"S");
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

    boost::python::list check(boost::python::list tokens)
    {
        auto len = boost::python::len(tokens);
        std::vector<std::wstring> vtokens;


        
        for(int i = 0; i < len; i++)
        {
          vtokens.emplace_back(boost::python::extract<std::wstring>(tokens[i]));
        }

        auto trees = _cky->get_trees(vtokens);

        for(const auto& v : trees)
        {
                printBT(v);
        }

//        utils::colections::matrix<boost::python::list> ret(map._rows, map._cols); 
        boost::python::list ret2;

        /*
        for (int i = 0; i < map._rows; ++i)
        {
            boost::python::list tmp;
            for (int j = 0; j < map._cols; ++j)
            {
                auto values = map[i][j].get();
                for (const auto &value : values)
                {
                    ret[i][j].append(value);
                    tmp.append(value);
                }
            }

            ret2.append(tmp);
        }*/
        return ret2;
    }

    std::wstring rules_info()
    {
        std::wstringstream ss;
     //   ss << *pcfg ;
        return ss.str();
    }


    ~grammar()
    {
        std::cout << "Grammar Destructor" << std::endl;
    }

  private:
    std::shared_ptr<cfg::PCFG<std::wstring, std::vector>> _pcfg;
    std::shared_ptr<parser::iparser<std::wstring>> _cky;
};

};

BOOST_PYTHON_MODULE(gramatico)
{
  using namespace boost::python;
  class_<grammar::grammar>("grammar")
    .def("add_rule", &grammar::grammar::add_rule)
    .def("rules_info", &grammar::grammar::rules_info)
    .def("add_terminal", &grammar::grammar::add_terminal)
    .def("check", &grammar::grammar::check);
  
  class_<utils::colections::matrix<boost::python::list>>("matrix", init<int, int>())
    .def("get", &utils::colections::matrix<boost::python::list>::get)
    .def_readonly("rows", &utils::colections::matrix<boost::python::list>::_rows)
    .def_readonly("cols", &utils::colections::matrix<boost::python::list>::_cols);
}

