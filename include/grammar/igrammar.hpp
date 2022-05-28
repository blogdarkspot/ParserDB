#ifndef __PARSER__
#define __PARSER__

#include <memory>
#include <vector>
#include "grammar/rules.hpp"
#include "grammar/constituency.hpp"
#include <string>

namespace grammar::cfg
{
    class icfg
    {
        public:
        using rule_ptr = std::shared_ptr<cfg::ProbabilisticRule>;
        virtual void set_terminals(rule_ptr) = 0;
        virtual void set_rules(rule_ptr) = 0;
        virtual void set_start_symbol(std::wstring) = 0;
        virtual std::vector<rule_ptr> get_match_rules(std::wstring, std::wstring) = 0;
        virtual std::vector<rule_ptr> get_lexicon(std::wstring) = 0;
        virtual std::wstring get_start_symbol() = 0;
    };
};

namespace grammar::parser
{

class iparser
{
    public:
    virtual void set_cfg(std::shared_ptr<::grammar::cfg::icfg>) = 0;
    virtual std::vector<std::shared_ptr<::grammar::cfg::symbol>> get_trees(std::vector<std::wstring>) = 0;
    virtual std::shared_ptr<::grammar::cfg::symbol> get_best_tree(std::vector<std::wstring>) = 0;
};
}
#endif
