#include "parser/grammar.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>

template <typename _Ty> struct wrapper_vector
{
    std::vector<_Ty> value;
};

TEST(PCFG_TEST, testone)
{
    grammar::rules<std::string, wrapper_vector> rule;
}