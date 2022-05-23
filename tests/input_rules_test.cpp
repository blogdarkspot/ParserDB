#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "grammar/input_rules.hpp"


TEST(InputRules, LoadRules)
{
    ::grammar::cfg::file file("grammar.cnf");

    EXPECT_TRUE(file.open());

    auto rules = file.get_all();

    EXPECT_EQ(rules.size(), 52);
}

