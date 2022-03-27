#include "delaf_dictionary.h"
#include "dictionary.hpp"
#include "tokens.hpp"
#include "gtest/gtest.h"
#include <string>
/*
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
*/

TEST(DelafDictionary, ParserFileOk)
{
    auto delaf_dic = dictionary::delaf_dictionary("delaf_linux.dic");
    EXPECT_TRUE(delaf_dic.open());
    EXPECT_TRUE(delaf_dic.parser());
    delaf_dic.print_values();
    delaf_dic.close();
}

TEST(Dictionary, InsertWord)
{

    dictionary::dictionary<std::wstring> dic;

    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        auto word = L"casa";
        auto canonical = L"casar";
        dic.add(word, canonical, lex);

        auto ret = dic.search_by_canonical(canonical);
        EXPECT_EQ(ret.size(), 1);
    }
}

TEST(Dictionary, GetWordByCanonical)
{

    dictionary::dictionary<std::wstring> dic;

    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        std::wstring word = L"casa";
        auto canonical = L"casar";
        dic.add(word, canonical, lex);

        auto ret = dic.get_flexion_from_canonical(canonical);
        EXPECT_EQ(ret.size(), 1);
        EXPECT_STREQ(ret[0].c_str(), word.c_str());
    }
}

TEST(Dictionary, GetWordByCanonical2)
{

    dictionary::dictionary<std::wstring> dic;
    std::wstring word = L"casa";
    std::wstring word2 = L"casado";
    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        auto canonical = L"casar";
        dic.add(word, canonical, lex);

        auto ret = dic.get_flexion_from_canonical(canonical);
        EXPECT_EQ(ret.size(), 1);
        EXPECT_STREQ(ret[0].c_str(), word.c_str());
    }

    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        auto canonical = L"casar";
        dic.add(word2, canonical, lex);

        auto ret = dic.get_flexion_from_canonical(canonical);
        EXPECT_EQ(ret.size(), 2);
        EXPECT_STREQ(ret[0].c_str(), word.c_str());
        EXPECT_STREQ(ret[1].c_str(), word2.c_str());
    }
}

TEST(Dictionary, GetWordByCanonical3)
{

    dictionary::dictionary<std::wstring> dic;
    std::wstring word = L"casa";
    std::wstring word2 = L"casado";
    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        auto canonical = L"casar";
        dic.add(word, canonical, lex);

        auto ret = dic.get_flexion_from_canonical(canonical);
        EXPECT_EQ(ret.size(), 1);
        EXPECT_STREQ(ret[0].c_str(), word.c_str());
    }

    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        auto canonical = L"casar";
        dic.add(word2, canonical, lex);

        auto ret = dic.get_flexion_from_canonical(canonical);
        EXPECT_EQ(ret.size(), 2);
        EXPECT_STREQ(ret[0].c_str(), word.c_str());
        EXPECT_STREQ(ret[1].c_str(), word2.c_str());
    }
}