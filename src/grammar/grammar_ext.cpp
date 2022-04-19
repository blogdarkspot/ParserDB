#include <boost/python.hpp>

#include "rules.hpp"
#include "grammar.hpp"
#include "input_rules.hpp"
#include "utils/matrix.hpp"
#include <boost/python/list.hpp>
#include "utils/unit.hpp"
#include <iostream>
#include <memory>
#include <set>
#include <string>

void printBT(const std::wstring& prefix, const std::shared_ptr<rule_description<std::wstring>> node, bool isLeft)
{
    if( node != nullptr )
    {
        std::wcout << prefix;

        std::wcout << (isLeft ? L"├──" : L"└──" );

        // print the value of the node
        std::wcout << node->value << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->left, true);
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->right, false);
    }
}

void printBT(const std::shared_ptr<rule_description<std::wstring>> node)
{
    printBT(L"", node, false);    
}

namespace grammar{ 
class grammar
{
  public:

    grammar() : pcfg(new PCFG<std::wstring, std::vector>())
    {

    }

    void add_rule(std::wstring rule)
    {
        ::grammar::cfg::file file;
        auto r = file.parser(rule);
        
        if(r != nullptr)
        {
            pcfg->add_rule(r);
        }
        else {
            std::cout << "rule failed" << std::endl;
        }
    }

    void add_terminal(std::wstring left, std::wstring right)
    {
        pcfg->add_terminal(left, right);
    }

    boost::python::list check(boost::python::list tokens)
    {
        auto len = boost::python::len(tokens);
        std::vector<std::wstring> vtokens;


        
        for(int i = 0; i < len; i++)
        {
          vtokens.emplace_back(boost::python::extract<std::wstring>(tokens[i]));
        }

        auto map = pcfg->parser(vtokens);

        auto unit = map[0][map._cols - 1];
        for(const auto& v : unit.get())
        {
            if(v->value == L"S")
            {
                printBT(v);
            }
        }

        utils::colections::matrix<boost::python::list> ret(map._rows, map._cols);
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
        ss << *pcfg ;
        return ss.str();
    }


    ~grammar()
    {
        std::cout << "Grammar Destructor" << std::endl;
    }

  private:
    std::shared_ptr<PCFG<std::wstring, std::vector>> pcfg;
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

