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

    void clear_rules()
    {
        _rules.clear();
        _rules_terminal.clear();
    }

    void clear_terminals()
    {
        _terminals.clear();
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
            auto tmp = std::static_pointer_cast<ProbabilisticRule<_Ky, std::vector>>(rule);

            auto it = _rules.find(rule->get_left_side());
            if(it != _rules.end())
            {
                tmp->tot_rules_same_left_side = it->second[0]->tot_rules_same_left_side;
            }
            else {
                tmp->tot_rules_same_left_side = std::make_shared<size_t>(0);
            }

            *(tmp->tot_rules_same_left_side) += 1;

            _rules[rule->get_left_side()].emplace_back(tmp);
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
        std::vector<typename cfg::icfg<_Ky>::rule_ptr> ret = _terminals[lex];
        auto tmp = ret; 
        for(const auto i : tmp)
        {
            _ContainerT<_Ky> grammarR; // = {value, value2};
            grammarR.emplace_back(i->get_left_side());
            
            for (const auto &[key, rules] : _rules_terminal)
            {
                for( const auto &rule : rules)
                {
                    
                    if(rule->is_valid(grammarR))
                    {
                        auto tmp1 = std::make_shared<ProbabilisticRule<_Ky, std::vector>>(rule->get_left_side(), i->get_right_side(), true);
                        ret.emplace_back(tmp1);
                    }
                }
            }
        }
        return ret;
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
        for (const auto &[key, value] : r._rules_terminal)
        {
            for (const auto &prule : value)
            {
                os << *prule << std::endl;
            }
        }
        for (const auto &[key, value] : r._terminals)
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

