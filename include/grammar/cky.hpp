#ifndef __CKY__
#define __CKY__
#include "igrammar.hpp"
#include "input_rules.hpp"
#include "pcfg.hpp"
#include "utils/matrix.hpp"
#include "constituency.hpp"
#include <vector>
#include <iostream>
#include <limits>

#include<map>
#include<set>

namespace grammar::parser
{

class cky 
{

public:

    using tree_list = std::vector<std::shared_ptr<::grammar::cfg::symbol>>;
    using symbol_list = std::vector<std::shared_ptr<grammar::cfg::symbol>>;

    virtual void set_cfg(std::shared_ptr<::grammar::cfg::PCFG> cfg)
    {
        _cfg = cfg;
    }

    virtual tree_list get_trees(std::vector<std::wstring> tokens) 
    {
        auto encoded = encode(tokens);
        return _get_trees(encoded);
    }

    tree_list get_trees2(std::vector<std::wstring> tokens)
    {
        auto e = encode2(tokens);
        return _get_trees(e, tokens.size());
    }

    virtual std::shared_ptr<::grammar::cfg::symbol> get_best_tree(std::vector<std::wstring> tokens)
    {
        auto encoded = encode(tokens);
        auto trees = _get_trees(encoded);
        std::cout << "total tree find " << trees.size() << std::endl;
        return _get_best_tree(trees);

    }

private:

    std::shared_ptr<::grammar::cfg::symbol> _get_best_tree (tree_list trees)
    {
        std::shared_ptr<::grammar::cfg::symbol> ret;
        double best_probability = std::numeric_limits<double>::min();

        for(const auto& tree : trees)
        {
            auto tmp = tree;
            double probability = std::numeric_limits<double>::min();
            compute_probability(tmp, probability);
            std::cout << "best probability " << best_probability << " current " << probability << std::endl;
            if(probability > best_probability)
            {
                ret = tree;
                best_probability = probability;
            }
        }

        return ret;
    }

    void compute_probability(std::shared_ptr<::grammar::cfg::symbol> node, double &p)
    {
        if(node == nullptr || node->is_terminal) return;

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

    void compute_probability_parent(std::shared_ptr<::grammar::cfg::symbol> node, double &p)
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

    tree_list _get_trees(std::map<int, std::map<int, symbol_list>>& m, int sz)
    {
        tree_list ret;
        auto pos0n = m[0][sz]; 
        
        for(const auto& pos : pos0n)
        {
            if(pos->value == _cfg->get_start_symbol())
            {
                ret.emplace_back(pos);
            }
        }

        return ret;
    }


    std::map<int, std::map<int, symbol_list>> encode2(std::vector<std::wstring> tokens)
    {
        std::map<int, std::map<int, symbol_list>> ret;
        for (int j = 1; j <= tokens.size(); ++j)
        {
            {

                auto terminais = _cfg->get_terminals();
                auto t_categories = terminais[tokens[j - 1]];
                auto rules = _cfg->get_terminal_rules();

                for (const auto &kv : rules)
                {
                    const auto &lhs = kv.first;
                    const auto &rules = kv.second;

                    for (const auto &rule : rules)
                    {
                        for (const auto &category : t_categories)
                        {

                            if (rule->get_right_side()[0] == category->get_left_side())
                            {

                                auto new_value = std::make_shared<grammar::cfg::symbol>();
                                new_value->value = rule->get_left_side();
                                new_value->canonical = category->get_right_side()[1];
                                new_value->probability = rule->get_probability();
                                new_value->terminal = tokens[j - 1];

                                ret[j - 1][j].emplace_back(new_value);

                            }
                        }
                    }
                }

                for (const auto terminal : t_categories)
                {
                    auto new_value = std::make_shared<grammar::cfg::symbol>();
                    new_value->value = terminal->get_left_side();
                    new_value->probability = terminal->get_probability();
                    new_value->terminal = tokens[j -1];
                    ret[j - 1][j].emplace_back(new_value);
                }

                {
                    auto mrules = _cfg->get_rules();

                    for (int i = j - 2; i >= 0; --i)
                    {
                        for (int k = i + 1; k <= j - 1; ++k)
                        {
                            for(const auto& kv : mrules)
                            {
                                const auto& lhs = kv.first;
                                const auto& rules = kv.second;

                                for(const auto& rule : rules)
                                {
                                    auto rhs1 = find(rule->get_right_side()[0], ret[i][k]);
                                    auto rhs2 = find(rule->get_right_side()[1], ret[k][j]);

                                    if(rhs1 != nullptr && rhs2 != nullptr)
                                    {
                                        auto new_value = std::make_shared<grammar::cfg::constituency>();
                                        new_value->value = rule->get_left_side();
                                        new_value->left = rhs1;
                                        new_value->right = rhs2;
                                        new_value->probability = rule->get_probability();
                                        //rhs1->parentProbability = rule->get_parent_probability(rhs1->value);
                                        //rhs2->parentProbability = rule->get_parent_probability(rhs2->value);
                                        ret[i][j].emplace_back(new_value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return ret;
    }

    std::shared_ptr<grammar::cfg::symbol> find(std::wstring token, const symbol_list& list)
    {
        std::shared_ptr<grammar::cfg::symbol> ret = nullptr;    

        for(const auto& value : list)
        {
            if(value->value == token)
            {
                if(ret == nullptr)
                {
                    ret = value;
                    continue;
                } 

                if(ret->probability < value->probability)
                {
                    ret = value;
                }
            }
        }
        return ret;
    }

    utils::colections::matrix<symbol_list> encode(std::vector<std::wstring> tokens)
    {
        auto ret = utils::colections::matrix<symbol_list>(tokens.size() + 1, tokens.size() + 1);

        for (int j = 1; j <= tokens.size(); ++j)
        {
            auto lexicons = _cfg->get_lexicon(tokens[j - 1]);

            for(const auto& lex : lexicons)
            {
                auto new_value = std::make_shared<grammar::cfg::constituency>();
                new_value->value = lex->get_left_side();
                new_value->probability = lex->get_probability();
                ret[j - 1][j].emplace_back(new_value);
                auto tmp = std::make_shared<grammar::cfg::lexicon>();
                tmp->is_terminal = true;
                tmp->value = lex->get_right_side()[0];
                if(tokens[j - 1] != lex->get_right_side()[0])
                {
                    tmp->lex = tokens[j - 1];
                }
                else {
                    tmp->lex = L"";
                }

                new_value->left = tmp;
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
                                 auto new_value = std::make_shared<grammar::cfg::constituency>();
                                 new_value->value = rule->get_left_side();
                                 new_value->left = value;
                                 new_value->right = value2;
                                 new_value->probability = rule->get_probability();
                                 value->parentProbability = rule->get_parent_probability(value->value);
                                 value2->parentProbability = rule->get_parent_probability(value2->value);
                                 ret[i][j].emplace_back(new_value);
                            }
                        }
                    }
                }
            }
        }
        return ret;
    }

std::shared_ptr<grammar::cfg::symbol> find(const std::wstring& symbol, 
            std::shared_ptr<grammar::cfg::symbol> value, 
            std::shared_ptr<grammar::cfg::symbol> value2,
            bool inverse = false)
{
    if(value->category == symbol)
        return inverse ? value2 : value;
    else
        return inverse ? value : value2;
}


std::shared_ptr<grammar::cfg::symbol> assing_head(std::wstring lhs, std::shared_ptr<grammar::cfg::symbol> value, std::shared_ptr<grammar::cfg::symbol> value2)
    {
        if(lhs == L"VP" || lhs == L"SP")
        {
            return find(L"V", value, value2);
        }
        if(lhs == L"NP")
        {
            return find(L"N", value, value2);
        }
        if(lhs == L"PP")
        {
            return find(L"PREP", value, value2);
        } 
        if(lhs == L"DET" || lhs == L"IP")
        {
            return value2;
        }
        if(lhs == L"NumP" || lhs == L"CP")
        {
                return value;
        }
        if(lhs == L"PossP")
        {
            return find(L"Poss", value, value2);
        }
        if(lhs == L"AP")
        {
            return find(L"A", value, value2);
        }
        if(lhs == L"ADV")
        {
            return find(L"ADV", value, value2);
        }
        return value;
    }



    std::shared_ptr<::grammar::cfg::PCFG> _cfg;
};


};
#endif