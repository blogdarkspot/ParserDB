#ifndef __UNIT__
#define __UNIT__

#include "grammar/rules.hpp"

#include <memory>
#include <utility>
#include <vector>
#include <string>

namespace grammar::cfg
{

struct symbol : public std::enable_shared_from_this<symbol>
{
    std::wstring terminal;
    std::wstring value;
    double probability;
    std::wstring parent;
    std::wstring canonical;
    std::wstring category;
    double parentProbability;
    bool is_terminal;
    std::shared_ptr<symbol> left = nullptr;
    std::shared_ptr<symbol> right = nullptr;


    bool operator<(const symbol& r) const
    {
        return std::tie(value) < std::tie(r.value);
    }

    bool operator()(const symbol& r) const
    {
        return std::tie(value) < std::tie(r.value);
    }
};

struct symbols : public std::enable_shared_from_this<symbols>
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

    symbols& operator+=(const std::shared_ptr<symbol>& rhs)
    {
        _rules.emplace_back(rhs);
        return *this;
    }

    bool operator<(const symbols& r) const
    {
        return std::tie(value) < std::tie(r.value);
    }

    bool operator<(const symbol& r) const
    {
        return std::tie(value) < std::tie(r.value);
    }
    
    void set_symbol(const std::shared_ptr<symbol>& rhs)
    {
        _rules.emplace_back(rhs);
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
