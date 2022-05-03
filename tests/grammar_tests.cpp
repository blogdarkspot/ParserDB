#include "grammar/pcfg.hpp"
#include "grammar/cky.hpp"
#include "rules.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

std::shared_ptr<grammar::cfg::ProbabilisticRule<std::wstring, std::vector>> _parser(const std::wstring &rule);
std::shared_ptr<grammar::cfg::ProbabilisticRule<std::wstring, std::vector>> add_terminal(std::wstring left, std::wstring right);
void printBT(const std::wstring& prefix, const std::shared_ptr<grammar::cfg::symbol<std::wstring>> node, bool isLeft);
void printBT(const std::shared_ptr<grammar::cfg::symbol<std::wstring>> node);

TEST(Grammar, InputRules)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG<std::wstring, std::vector>>();
    auto _cky = std::make_shared<grammar::parser::cky<std::wstring>>();
    
    _pcfg->set_rules(_parser(L"S -> A"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_start_symbol(L"S");
    std::vector<std::wstring> tokens = {L"a"};
    
    _cky->set_cfg(_pcfg);

    auto trees = _cky->get_trees(tokens);

    EXPECT_GT(trees.size(), 0);

    auto tree = trees[0];
}

TEST(Grammar, TwoLevels)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG<std::wstring, std::vector>>();
    auto _cky = std::make_shared<grammar::parser::cky<std::wstring>>();
    
    _pcfg->set_rules(_parser(L"S -> A B"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"B", L"b"));
    _pcfg->set_start_symbol(L"S");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"b", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 0);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

}

TEST(Grammar, TreeLevels)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG<std::wstring, std::vector>>();
    auto _cky = std::make_shared<grammar::parser::cky<std::wstring>>();
    
    _pcfg->set_rules(_parser(L"S -> A B"));
    _pcfg->set_rules(_parser(L"B -> C D"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"C", L"b"));
    _pcfg->set_terminals(add_terminal(L"D", L"c"));
    _pcfg->set_start_symbol(L"S");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"b", L"c", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 0);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

}

TEST(Grammar, Recursive)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG<std::wstring, std::vector>>();
    auto _cky = std::make_shared<grammar::parser::cky<std::wstring>>();
    
    _pcfg->set_rules(_parser(L"S -> A B"));
    _pcfg->set_rules(_parser(L"B -> C D"));
    _pcfg->set_rules(_parser(L"B -> B B"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"C", L"b"));
    _pcfg->set_terminals(add_terminal(L"D", L"c"));
    _pcfg->set_start_symbol(L"S");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"b", L"c", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 0);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c", L"b", L"c", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_GT(trees.size(), 1);
    }

}

TEST(Grammar, NPRules)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG<std::wstring, std::vector>>();
    auto _cky = std::make_shared<grammar::parser::cky<std::wstring>>();
    
    _pcfg->set_rules(_parser(L"NP -> N"));
    _pcfg->set_rules(_parser(L"NP -> NP AP"));
    _pcfg->set_rules(_parser(L"NP -> AP NP"));
    _pcfg->set_rules(_parser(L"NP -> NP PP"));
    _pcfg->set_rules(_parser(L"NP -> N PP"));

    _pcfg->set_rules(_parser(L"AP -> A"));
    _pcfg->set_rules(_parser(L"AP -> AdvP AP"));
    _pcfg->set_rules(_parser(L"AP -> AP AdvP"));
    _pcfg->set_rules(_parser(L"AP -> AP PP"));
    _pcfg->set_rules(_parser(L"AP -> A PP"));


    _pcfg->set_rules(_parser(L"PP -> PREP"));
    _pcfg->set_rules(_parser(L"PP -> AdvP PP"));
    _pcfg->set_rules(_parser(L"PP -> P DP"));
    _pcfg->set_rules(_parser(L"PP -> P AdvP"));
    _pcfg->set_rules(_parser(L"PP -> P PP"));

    _pcfg->set_rules(_parser(L"AdvP -> ADV"));
    _pcfg->set_rules(_parser(L"AdvP -> AdvP AdvP"));
    _pcfg->set_rules(_parser(L"AdvP -> ADV PP"));


    _pcfg->set_terminals(add_terminal(L"N", L"n"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"ADV", L"c"));
    _pcfg->set_terminals(add_terminal(L"PREP", L"p"));
    _pcfg->set_start_symbol(L"NP");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"n"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }
    {
        std::vector<std::wstring> tokens = {L"n", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }
    {
        std::vector<std::wstring> tokens = {L"a", L"n"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        printBT(trees[0]);
    }
    {
        std::vector<std::wstring> tokens = {L"c", L"a", L"n"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        printBT(trees[0]);
    }

}

std::shared_ptr<grammar::cfg::ProbabilisticRule<std::wstring, std::vector>> add_terminal(std::wstring left, std::wstring right)
{
    std::vector<std::wstring> _right;
    _right.emplace_back(right);
    return std::make_shared<::grammar::cfg::ProbabilisticRule<std::wstring, std::vector>>(left, _right, true);
}

std::shared_ptr<grammar::cfg::ProbabilisticRule<std::wstring, std::vector>> _parser(const std::wstring &rule)
{
    auto pos = rule.find_first_of(L"-");

    std::wstring left;
    std::vector<std::wstring> right;

    if (pos == rule.npos)
    {
        return nullptr;
    }
    if (rule[pos + 1] != L'>')
    {
        return nullptr;
    }

    left = rule.substr(0, pos - 1);
    pos += 3;

    do
    {
        auto tmp = rule.find_first_of(L' ', pos);

        if (tmp == rule.npos)
        {
            right.emplace_back(rule.substr(pos));
            break;
        }
        else
        {
            auto s = rule.substr(pos, tmp - pos);
            right.emplace_back(s);
            pos += (tmp - pos) + 1;
        }
    } while (true);

    return std::make_shared<::grammar::cfg::ProbabilisticRule<std::wstring, std::vector>>(left, right, false);
}

void printBT(const std::wstring& prefix, const std::shared_ptr<grammar::cfg::symbol<std::wstring>> node, bool isLeft)
{
    if( node != nullptr )
    {
        std::wcout << prefix;

        std::wcout << (isLeft ? L"├──" : L"└──" );

        bool is_terminal = node->is_terminal;
        // print the value of the node
        std::wcout << node->value <<  (is_terminal ? L"──" : L"\n");

        if(is_terminal)
        {
            std::wcout << (std::static_pointer_cast<grammar::cfg::lexicon<std::wstring>>(node))->lex << std::endl;
        }
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->left, true);
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->right, false);
    }
}

void printBT(const std::shared_ptr<grammar::cfg::symbol<std::wstring>> node)
{
    printBT(L"", node, false);    
}