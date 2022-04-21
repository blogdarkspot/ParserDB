#ifndef __GRAMMAR__
#define __GRAMMAR__

#include "rules.hpp"
#include "utils/matrix.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "igrammar.hpp"
#include "constituency.hpp"

namespace grammar::cfg
{

template <typename _Ky, template <class...> class _ContainerT> class PCFG : public icfg<_Ky>
{

  public:
    PCFG() = default;

    virtual void set_terminals(typename cfg::icfg<_Ky>::rule_ptr terminal) override
    {
            _terminals[terminal->get_right_side()[0]].emplace_back(
                std::static_pointer_cast<ProbabilisticRule<_Ky, std::vector>>(terminal));
    }

    virtual void set_rules(typename cfg::icfg<_Ky>::rule_ptr rule) override
    {
        if (rule->get_right_side().size() == 1)
        {
            _rules_terminal[rule->get_left_side()].emplace_back(
                std::static_pointer_cast<ProbabilisticRule<_Ky, std::vector>>(rule));
        }
        else
        {
            _rules[rule->get_left_side()].emplace_back(
                std::static_pointer_cast<ProbabilisticRule<_Ky, std::vector>>(rule));

            auto it = _rules.find(rule->get_left_side());

            for (auto i = 0; i < it->second.size(); ++i)
            {
                it->second[i]->total = it->second.size();
            }
        }
    }

    virtual void set_start_symbol(_Ky symbol) override
    {
        _symbol = symbol;
    }


    virtual _Ky get_start_symbol() override 
    {
        return _symbol;
    }

    virtual std::vector<typename cfg::icfg<_Ky>::rule_ptr> get_lexicon(_Ky lex) override
    {
        std::vector<typename cfg::icfg<_Ky>::rule_ptr> ret;

        return _terminals[lex];
    }

    virtual std::vector<typename cfg::icfg<_Ky>::rule_ptr> get_match_rules(_Ky first, _Ky second) override
    {
        _ContainerT<_Ky> grammarR; // = {value, value2};
        grammarR.emplace_back(first);
        grammarR.emplace_back(second);

        std::vector<typename cfg::icfg<_Ky>::rule_ptr> ret;

        for (const auto &[key, rules] : _rules)
        {
            for (const auto &rule : rules)
            {
                if (rule->is_valid(grammarR))
                {
                    ret.emplace_back(rule);
                }
            }
        }
        return ret;
    }

  private:
    std::map<_Ky, std::vector<std::shared_ptr<ProbabilisticRule<_Ky, std::vector>>>> _rules;
    std::map<_Ky, std::vector<std::shared_ptr<ProbabilisticRule<_Ky, std::vector>>>> _rules_terminal;
    std::map<_Ky, std::vector<std::shared_ptr<ProbabilisticRule<_Ky, std::vector>>>> _terminals;
    _Ky _symbol;

  public:
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
};
} // namespace grammar::cfg
#endif

