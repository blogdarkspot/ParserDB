#include "ceten_folha.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(CETEN_CORPUS, OpenFileOk)
{
    visl_cg::decoder decoder;
    EXPECT_TRUE(decoder.load_file("CETENFolha-1.0_jan2014.cg"));
}

TEST(CETEN_CORPUS, ParserFile)
{

    visl_cg::decoder decoder;
    EXPECT_TRUE(decoder.load_file("CETENFolha-1.0_jan2014.cg"));
    decoder.parser_file();
}