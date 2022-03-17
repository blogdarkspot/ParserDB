#include "delaf_dictionary.h"
#include "gtest/gtest.h"

TEST(DelafDictionary, OpenFileOk)
{
    auto delaf_dic = dictionary::delaf_dictionary("delaf_linux.dic");
    EXPECT_TRUE(delaf_dic.open());
    delaf_dic.close();
}

TEST(DelafDictionary, OpenFileError)
{
    auto delaf_dic = dictionary::delaf_dictionary("delafa_linux.dic");
    EXPECT_FALSE(delaf_dic.open());
    delaf_dic.close();
}

TEST(DelafDictionary, ParserFileOk)
{
    auto delaf_dic = dictionary::delaf_dictionary("delaf_linux.dic");
    EXPECT_TRUE(delaf_dic.open());
    EXPECT_TRUE(delaf_dic.parser());
    delaf_dic.print_values();
    delaf_dic.close();
}

TEST(DelafDictionary, ParserFileFalse)
{
    auto delaf_dic = dictionary::delaf_dictionary("delaf_linux.dic");
}
