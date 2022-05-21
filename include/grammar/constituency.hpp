#ifndef __UNIT__
#define __UNIT__

#include <memory>
#include <utility>
#include <vector>
#include <rules.hpp>

namespace grammar::cfg
{

struct symbol
{
    std::wstring value;
    double probability;
    bool is_terminal;
    std::shared_ptr<symbol> left = nullptr;
    std::shared_ptr<symbol> right = nullptr;
};

struct symbols
{
    std::wstring value;

    void set_left(std::shared_ptr<symbol> left)
    {
        for(auto rule : _rules)
        {
            rule->left = left;
        }
    }

    void set_right(std::shared_ptr<symbol> right)
    {
        for(auto rule : _rules)
        {
            rule->right = right;
        }
    }

    symbols& operator+=(std::shared_ptr<symbol> rhs)
    {
        _rules.emplace_back(rhs);
        return *this;
    }

    private:
    std::vector<std::shared_ptr<symbol>> _rules;
};

struct lexicon : symbol
{
    std::wstring lex;
};

struct constituency : public symbol
{
    void set_left(std::shared_ptr<constituency> l)
    {
        symbol::left = l;
    }

    void set_right(std::shared_ptr<constituency> r)
    {
        symbol::right = r;
    }
};
};
#endif