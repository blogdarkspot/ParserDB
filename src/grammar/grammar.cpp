#include "rules.hpp"
#include "grammar.hpp"
#include "input_rules.hpp"
#include <cstdio>
#include <cwchar>
#include <cstring>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>

namespace grammar
{

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
            std::cout << "rule faile" << std::endl;
        }
    }

    void rules_info(wchar_t *info, size_t size)
    {
        std::memset(info, 0, sizeof(wchar_t) * size);
        std::wstringstream ss;
        ss << *pcfg ;
        wcscpy(info, ss.str().c_str());
        std::wcout << *pcfg;

    }


    ~grammar()
    {
        std::cout << "Grammar Destructor" << std::endl;
    }

  private:
    std::shared_ptr<PCFG<std::wstring, std::vector>> pcfg;
};


}; // namespace grammar