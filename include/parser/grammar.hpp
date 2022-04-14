#ifndef __GRAMMAR__
#define __GRAMMAR__

#include "parser/rules.hpp"
#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace grammar
{

// Probablistic Context-Grammar Free
/*
_no_terminal  =  a set of non-terminal symbols (or variables)
  _terminal =  a set of terminal symbols (disjoint from N)
 _roles =  a set of rules or productions, each of the form A → β [p],
        where A is a _no_terminal,
        β is a string of symbols from the infinite set of strings (_no_terminal ∪ _terminal)∗,
and p is a number between 0 and 1 expressing P(β |A)
_start a designated start symbol
*
*/

template <typename _Ky, template <class...> class _ContainerT> class PCFG
{
    struct ProbabilisticRule
    {
        unsigned int total;
        unsigned int times;
        typename ::grammar::cfg::rule<_Ky, _ContainerT>::rule_sptr _rule;

        friend std::wostream &operator<<(std::wostream &os, const ProbabilisticRule &r)
        {
            os << L"Total left : " << r.total << L" Total match " << r.times << L" Height " << r.times / r.total
               << L"\n"
               << *r._rule;
            return os;
        }
    };

  public:
    PCFG() = default;

    void add_rule(std::shared_ptr<cfg::rule<_Ky, _ContainerT>> rule)
    {
        auto p = std::make_shared<ProbabilisticRule>();

        p->_rule = rule;

        _rules[rule->get_left_side()].emplace_back(p);

        auto it = _rules.find(rule->get_left_side());

        for (auto i = 0; i < it->second.size(); ++i)
        {
            it->second[i]->total = it->second.size();
        }
    }

    /*
    _M<_Ry> parser(_ContainerT<_Ky> ordered_list)
    {
        auto ret = _M<_Ry>(ordered_list.size() + 1, ordered_list.size() + 1);

        for (int j = 1; j <= ordered_list.size(); ++j)
        {
            auto _terminal_grammar = terminals[ordered_list[j]];

            for (const auto &r : _terminal_grammar)
            {
                ret[j - 1][j] += r->get_right_side()[0];
            }

            for (int i = j - 2; i > 0; --i)
            {
                for (int k = i + i; k < j - 1; ++k)
                {
                }
            }
        }
    }*/

    friend std::wostream &operator<<(std::wostream &os, const PCFG<_Ky, _ContainerT> &r)
    {
        for (const auto &[key, value] : r._rules)
        {
            for (const auto &prule : value)
            {
                os << *prule << std::endl;
            }
        }
        return os;
    }

  private:
    // std::map<std::string, std::vector<std::shared_ptr<rule<_Ky, _C>>>> terminals;
    std::map<_Ky, std::vector<std::shared_ptr<ProbabilisticRule>>> _rules;
};
}; // namespace grammar
#endif