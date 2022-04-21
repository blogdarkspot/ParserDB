#include "cky.hpp"
#include "rules.hpp"
#include "input_rules.hpp"
#include "pcfg.hpp"
#include "igrammar.hpp"
#include <cwchar>
#include <cstring>
#include <iostream>
#include <memory>

namespace grammar
{

class grammar
{
  public:

    grammar() : pcfg(new cfg::PCFG<std::wstring, std::vector>()),
    _cky(new parser::cky<std::wstring>())
    {
        _cky->set_cfg(pcfg);
    }

    void add_rule(std::wstring rule, double probability)
    {
        ::grammar::cfg::file file;
        auto r = file.parser(rule);
        if(r != nullptr)
        {
            pcfg->set_rules(r);
        }
        else {
            std::cout << "rule faile" << std::endl;
        }
    }

    void add_terminal(const std::wstring& left, const std::wstring& right, double probability)
    {
        /*
        std::vector<std::wstring> _right;
        _right.emplace_back(right);
        auto terminal = std::make_shared<::grammar::cfg::rule<std::wstring, std::vector>>(left, _right, true);
        pcfg->set_terminals(terminal);
        */
    }

    void parser(std::vector<std::wstring> tokens)
    {
        auto trees = _cky->get_trees(tokens);
    }

    void rules_info()
    {
    }


    ~grammar()
    {
        std::cout << "Grammar Destructor" << std::endl;
    }

  private:
    std::shared_ptr<cfg::PCFG<std::wstring, std::vector>> pcfg;
    std::shared_ptr<parser::iparser<std::wstring>> _cky;
};


}; // namespace grammar