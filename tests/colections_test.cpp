#include "utils/matrix.hpp"
#include "utils/unit.hpp"
#include <gtest/gtest.h>
#include <set>

TEST(UtilsUnit, CreateObjectWithSet)
{
    unit<int, std::set> s;
    s = 1;
    s += 2;
    auto ret = s.get();
    EXPECT_EQ(ret.size(), 2);
}

TEST(UtilsUnit, CreateMatrixWithUnitPtrNullptr)
{
    utils::colections::matrix<unit<int, std::set>::unit_ptr> m(2, 2);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            ASSERT_TRUE(m[i][j] == nullptr);
        }
    }
}

TEST(UtilsUnit, CreateMAtrixAndFillWithUnitPtr)
{

    utils::colections::matrix<unit<int, std::set>::unit_ptr> m(2, 2);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            auto ptr = std::make_shared<unit<int, std::set>>();
            *ptr = i * j;
            m[i][j] = ptr;
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            ASSERT_EQ(*((m[i][j])->get().begin()), i * j);
        }
    }
}