#include "parser/grammar.hpp"
#include "parser/input_rules.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

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
}

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