#ifndef __CKY__
#define __CKY__
#include "igrammar.hpp"
#include "input_rules.hpp"
#include "utils/matrix.hpp"
#include "constituency.hpp"
#include <vector>
#include <iostream>
#include <limits>

namespace grammar::parser
{

template<typename _StringType>
class cky : public iparser<_StringType>
{

public:

    using tree_list = std::vector<std::shared_ptr<::grammar::cfg::symbol<_StringType>>>;
    using symbol_list = std::vector<std::shared_ptr<grammar::cfg::symbol<_StringType>>>;

    virtual void set_cfg(std::shared_ptr<::grammar::cfg::icfg<_StringType>> cfg) override 
    {
        _cfg = cfg;
    }

    virtual tree_list get_trees(std::vector<_StringType> tokens) override
    {
        auto encoded = encode(tokens);
        return _get_trees(encoded);
    }

    virtual std::shared_ptr<::grammar::cfg::symbol<_StringType>> get_best_tree(std::vector<_StringType> tokens) override
    {
        auto encoded = encode(tokens);
        auto trees = _get_trees(encoded);
        return _get_best_tree(trees);

    }

private:

    std::shared_ptr<::grammar::cfg::symbol<_StringType>> _get_best_tree (tree_list trees)
    {
        std::shared_ptr<::grammar::cfg::symbol<_StringType>> ret;
        double best_probability = std::numeric_limits<double>::min();

        for(const auto& tree : trees)
        {
            double probability = std::numeric_limits<double>::min();
            compute_probability(tree, probability);
            if(probability > best_probability)
            {
                ret = tree;
                best_probability = probability;
            }
        }

        return ret;
    }

    void compute_probability(std::shared_ptr<::grammar::cfg::symbol<_StringType>> node, double &p)
    {
        if(node == nullptr) return;

        if(p == std::numeric_limits<double>::min())
        {
            p = node->probability;
        }
        else
        {
            p *= node->probability;
        }

        compute_probability(node->left, p);
        compute_probability(node->right, p);
    }

    tree_list _get_trees(utils::colections::matrix<symbol_list>& m)
    {
        tree_list ret;
        auto pos0n = m[0][m._cols - 1]; 
        
        for(const auto& pos : pos0n)
        {
            if(pos->value == _cfg->get_start_symbol())
            {
                ret.emplace_back(pos);
            }
        }

        return ret;
    }

    utils::colections::matrix<symbol_list> encode(std::vector<_StringType> tokens)
    {
        auto ret = utils::colections::matrix<symbol_list>(tokens.size() + 1, tokens.size() + 1);

        for (int j = 1; j <= tokens.size(); ++j)
        {
            auto lexicons = _cfg->get_lexicon(tokens[j - 1]);

            for(const auto& lex : lexicons)
            {
                auto tmp = std::make_shared<grammar::cfg::lexicon<_StringType>>();
                tmp->is_terminal = true;
                tmp->value = lex->get_left_side();
                tmp->lex = lex->get_right_side()[0];
                ret[j - 1][j].emplace_back(tmp);
            }

            for (int i = j - 2; i >= 0; --i)
            {
                for (int k = i + 1; k <= j - 1; ++k)
                {
                    auto first = ret[i][k];
                    auto second = ret[k][j];

                    for (const auto &value : first)
                    {
                        for (const auto &value2 : second)
                        {
                            auto rules = _cfg->get_match_rules(value->value, value2->value);

                            for (const auto &rule : rules)
                            {
                                 auto new_value = std::make_shared<grammar::cfg::constituency<_StringType>>();
                                 new_value->value = rule->get_left_side();
                                 new_value->left = value;
                                 new_value->right = value2;
                                 new_value->probability = rule->get_probability();
                                 ret[i][j].emplace_back(new_value);
                            }
                        }
                    }
                }
            }
        }
        return ret;
    }

    std::shared_ptr<::grammar::cfg::icfg<_StringType>> _cfg;
};


};
#endif