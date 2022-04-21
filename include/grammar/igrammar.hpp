#ifndef __PARSER__
#define __PARSER__

#include <memory>
#include <vector>
#include <rules.hpp>
#include "constituency.hpp"

namespace grammar::cfg
{
    template<typename _StringType>
    class icfg
    {
        public:
        using rule_ptr = std::shared_ptr<cfg::ProbabilisticRule<_StringType, std::vector>>;
        virtual void set_terminals(rule_ptr) = 0;
        virtual void set_rules(rule_ptr) = 0;
        virtual void set_start_symbol(_StringType) = 0;
        virtual std::vector<rule_ptr> get_match_rules(_StringType, _StringType) = 0;
        virtual std::vector<rule_ptr> get_lexicon(_StringType) = 0;
        virtual _StringType get_start_symbol() = 0;
    };
};

namespace grammar::parser
{

template<typename _StringType>
class iparser
{
    public:
    virtual void set_cfg(std::shared_ptr<::grammar::cfg::icfg<_StringType>>) = 0;
    virtual std::vector<std::shared_ptr<::grammar::cfg::symbol<_StringType>>> get_trees(std::vector<_StringType>) = 0;
};
}
#endif