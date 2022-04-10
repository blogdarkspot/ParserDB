#include "delaf_dictionary.h"
#include "dictionary.hpp"
#include "tokens.hpp"
#include "gtest/gtest.h"
#include <string>

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
    auto _dic = delaf_dic.getDictionary();

    auto ret = _dic.split_contraction_word(L"nisso");
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

        auto ret = dic.get_lex_unities_by_canonical(canonical);
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

        auto ret = dic.get_lex_unities_by_canonical(canonical);
        EXPECT_EQ(ret.size(), 1);
        EXPECT_STREQ(dic.get_desc_word(ret[0]).c_str(), word.c_str());
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

        auto ret = dic.get_lex_unities_by_canonical(canonical);
        EXPECT_EQ(ret.size(), 1);
        EXPECT_STREQ(dic.get_desc_word(ret[0]).c_str(), word.c_str());
    }

    {
        auto lex = std::make_shared<lexico>();
        lex->categoria = dictionary::categoria_enum_to_it(dictionary::categoria::V);
        auto canonical = L"casar";
        dic.add(word2, canonical, lex);

        auto ret = dic.get_lex_unities_by_canonical(canonical);
        EXPECT_EQ(ret.size(), 2);
        EXPECT_STREQ(dic.get_desc_word(ret[0]).c_str(), word.c_str());
        EXPECT_STREQ(dic.get_desc_word(ret[1]).c_str(), word2.c_str());
    }
}

TEST(Dictionary, IntegrationWithDelafGetWordGetCanonicalAndContr)
{
    auto delaf_dic = dictionary::delaf_dictionary("delaf_linux.dic");
    EXPECT_TRUE(delaf_dic.open());
    ASSERT_TRUE(delaf_dic.parser());
    auto dic = delaf_dic.getDictionary();
    auto verbete = L"casa";
    std::unordered_set<wstring> canonical = {L"casa", L"casar"};
    std::unordered_set<wstring> verbetes = {L"casa", L"casar"};
    std::unordered_set<wstring> tempo = {L"P", L"Y"};
    std::unordered_set<wstring> categoria = {L"N", L"V"};
    std::unordered_set<wstring> traco = {L"f", L"s", L"3", L"2"};
    // get verbete casa do dicionário preenchido pelo delaf
    {
        auto ret = dic.get_lex_unities_by_word(L"casa");
        // tem que conter
        EXPECT_EQ(ret.size(), 3);
        for (const auto &unit : ret)
        {
            EXPECT_TRUE(verbetes.find(dic.get_desc_word(unit)) != verbetes.end());
            EXPECT_TRUE(canonical.find(dic.get_desc_lemma(unit)) != canonical.end());
            EXPECT_TRUE(categoria.find(*(unit->data->categoria)) != categoria.end());
            for (const auto &t : unit->data->traco)
            {
                EXPECT_TRUE(traco.find(*(t)) != traco.end());
            }

            if (*(unit->data->categoria) == L"V")
            {
                EXPECT_TRUE(tempo.find(*(unit->data->tempo)) != tempo.end());
            }
        }
    }
    delaf_dic.close();
}