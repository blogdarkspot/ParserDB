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

class PCFG 
{

  public:
    PCFG() = default;

    virtual void set_terminals(typename cfg::icfg::rule_ptr terminal)//, std::wstring canonical)
    {
        auto tmp = std::static_pointer_cast<ProbabilisticRule>(terminal);
        _terminals[terminal->get_right_side()[0]].emplace_back(tmp);
    }

    void set_canonical_terminal(std::wstring word, std::wstring category, std::wstring canonical)
    {
        _canonicals[std::make_pair(word, category)] = canonical;
    }

    std::wstring get_canonical_terminal(std::wstring word, std::wstring category)
    {
        return _canonicals[std::make_pair(word, category)];
    }

    void clear_rules()
    {
        _rules.clear();
        _rules_terminal.clear();
    }

    void clear_terminals()
    {
        _terminals.clear();
        _canonicals.clear();
    }

    virtual void set_rules(typename cfg::icfg::rule_ptr rule) 
    {
    
        auto tmp = std::static_pointer_cast<ProbabilisticRule>(rule);

        if (rule->get_right_side().size() == 1)
        {
            _rules_terminal[rule->get_left_side()].emplace_back(tmp);
                
        }
        else
        {
            _rules[rule->get_left_side()].emplace_back(tmp);
        }
    }

    virtual void set_start_symbol(std::wstring symbol) 
    {
        _symbol = symbol;
    }


    virtual std::wstring get_start_symbol() 
    {
        return _symbol;
    }

    virtual std::vector<cfg::icfg::rule_ptr> get_lexicon(std::wstring lex)
    {
        std::vector<cfg::icfg::rule_ptr> ret = _terminals[lex];
        auto tmp = ret; 
        for(const auto i : tmp)
        {
            std::vector<std::wstring> grammarR; // = {value, value2};
            grammarR.emplace_back(i->get_left_side());
            
            for (const auto &[key, rules] : _rules_terminal)
            {
                for( const auto &rule : rules)
                {
                    
                    if(rule->is_valid(grammarR))
                    {
                        ret.emplace_back(rule);
                    }
                }
            }
        }
        return ret;
    }

    virtual std::vector<cfg::icfg::rule_ptr> get_match_rules(std::wstring first, std::wstring second)
    {
        std::vector<std::wstring> grammarR; // = {value, value2};
        grammarR.emplace_back(first);
        grammarR.emplace_back(second);

        std::vector<cfg::icfg::rule_ptr> ret;

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

    const std::map<std::wstring, std::vector<std::shared_ptr<ProbabilisticRule>>>& get_rules()
    {
        return _rules;
    }

    const std::map<std::wstring, std::vector<std::shared_ptr<ProbabilisticRule>>>& get_terminal_rules()
    {
        return _rules_terminal;
    }

    const std::map<std::wstring, std::vector<std::shared_ptr<ProbabilisticRule>>>& get_terminals()
    {
        return _terminals;
    }

  private:
    std::map<std::wstring, std::vector<std::shared_ptr<ProbabilisticRule>>> _rules;
    std::map<std::wstring, std::vector<std::shared_ptr<ProbabilisticRule>>> _rules_terminal;
    std::map<std::wstring, std::vector<std::shared_ptr<ProbabilisticRule>>> _terminals;
    std::map<std::pair<std::wstring, std::wstring>, std::wstring> _canonicals;
    std::wstring _symbol;

  public:
    friend std::wostream &operator<<(std::wostream &os, const PCFG &r)
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

