#include "parser/grammar.hpp"
#include "parser/input_rules.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

/*
TEST(Grammar, InputRules)
{
    std::vector<std::wstring> s_roles;
    s_roles.emplace_back(L"NP -> det N'");
    s_roles.emplace_back(L"NP -> N'");
    s_roles.emplace_back(L"NP -> pré-det NP");
    s_roles.emplace_back(L"N' -> Pro");
    s_roles.emplace_back(L"N' -> N");

    grammar::cfg::file file;

    auto ret = true;
    auto rules = file.parser(s_roles, ret);

    ASSERT_TRUE(ret);
    ASSERT_EQ(rules.size(), s_roles.size());

    for (const auto &rule : rules)
    {
        std::wcout << *rule << std::endl;
    }
}*/

/*
TEST(Grammar, InsertRulesPCFG)
{
    std::vector<std::wstring> s_roles;
    s_roles.emplace_back(L"NP -> det N'");
    s_roles.emplace_back(L"NP -> N'");
    s_roles.emplace_back(L"NP -> pré-det NP");
    s_roles.emplace_back(L"N' -> Pro");
    s_roles.emplace_back(L"N' -> N");

    grammar::cfg::file file;

    auto ret = true;
    auto rules = file.parser(s_roles, ret);

    grammar::PCFG<std::wstring, std::vector> pcfg;

    for (const auto &rule : rules)
    {
        pcfg.add_rule(rule);
    }

    std::wcout << pcfg << std::endl;
}
*/

TEST(Grammar, Personal)
{
    std::vector<std::wstring> s_roles;
    s_roles.emplace_back(L"S -> A B");
    s_roles.emplace_back(L"S -> B A");
    s_roles.emplace_back(L"S -> B");
    s_roles.emplace_back(L"B -> C");
    s_roles.emplace_back(L"B -> C D");
    s_roles.emplace_back(L"A -> E F");
    s_roles.emplace_back(L"A -> E");

    std::vector<std::pair<std::wstring, std::wstring>> terminals;

    terminals.emplace_back(std::make_pair(L"C", L"c"));
    terminals.emplace_back(std::make_pair(L"D", L"d"));
    terminals.emplace_back(std::make_pair(L"E", L"e"));
    terminals.emplace_back(std::make_pair(L"F", L"f"));

    std::vector<std::wstring> tokens = {L"e", L"f", L"c", L"d"};

    grammar::cfg::file file;

    auto ret = true;
    auto rules = file.parser(s_roles, ret);

    grammar::PCFG<std::wstring, std::vector> pcfg;

    for (const auto &rule : rules)
    {
        pcfg.add_rule(rule);
    }

    for (const auto &terminal : terminals)
    {
        pcfg.add_terminal(terminal.first, terminal.second);
    }

    auto mtx = pcfg.parser(tokens);

    for (int i = 0; i < mtx._rows; ++i)
    {
        for (int j = 0; j < mtx._cols; ++j)
        {
            std::wcout << mtx[i][j] << L" ";
        }
        std::wcout << std::endl;
    }
}

/*
TEST(Grammar, CheckIfMatchRightSide)
{
    std::vector<std::wstring> s_rules;
    s_rules.emplace_back(L"NP -> det N'");

    grammar::cfg::file file;
    auto ret = true;
    auto rules = file.parser(s_rules, ret);

    auto rule = rules[0];

    std::vector<std::wstring> mustBeTrue = {L"det", L"N'"};
    std::vector<std::wstring> mustBeFalse = {L"N'", L"det"};
    EXPECT_TRUE(rule->is_valid(mustBeTrue));
    EXPECT_FALSE(rule->is_valid(mustBeFalse));
}
*/