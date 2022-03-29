#include "delaf_dictionary.h"
#include "dictionary.hpp"
#include "tokens.hpp"
#include "gtest/gtest.h"
#include <exception>
#include <string>
#include <type_traits>

class delaf_test : public ::testing::Test
{

  protected:
    void SetUp()
    {
        _delaf = new dictionary::delaf_dictionary("delaf_linux.dic");
        _isOpen = _delaf->open();

        if (!_isOpen)
            throw("Cannot Open File");
        _isParserd = _delaf->parser();

        if (!_isParserd)
            throw("Parsed Failed");
        _lexico = _delaf->getDictionary();
    }

    void TearDown()
    {
        _delaf->close();
        delete _delaf;
    }

    dictionary::dictionary<std::wstring> _lexico;
    dictionary::delaf_dictionary *_delaf;
    bool _isOpen;
    bool _isParserd;

    const std::wstring text =
        L"O caso de uso é a forma mais eficaz de saber se uma ideia pode ser realmente boa "
        L"para seu negócio Muito se fala "
        L"na importância de arriscar mas para fazer isso com chances reais de sucesso é preciso saber quais são os "
        L"riscos "
        L"e "
        L"como eles podem ser equilibrados pelos possíveis benefícios O caso de uso permite que as informações sejam "
        L"apresentadas de forma sólida e estruturada para que os tomadores de decisão considerem seu projeto como um "
        L"todo "
        L"e "
        L"sejam convencidos pela proposta ou apontem possíveis melhorias Não importa que cargo você ocupa na empresa "
        L"ou o "
        L"setor no qual está alocado se quer receber apoio e até patrocínio para colocar sua ideia em prática essa "
        L"etapa é "
        L"super importante";
};

/*
TEST_F(delaf_test, search_by_word)
{
    std::vector<std::wstring> lexico_units;
    dictionary::tokenize(text, L" ", lexico_units);

    for (const auto &unit : lexico_units)
    {
        auto traces = _lexico.search_by_word(unit);
        EXPECT_TRUE(traces.size() > 0);
    }
}*/

TEST_F(delaf_test, get_canonical)
{
    std::vector<std::wstring> lexico_units;
    dictionary::tokenize(text, L" ", lexico_units);

    for (const auto &unit : lexico_units)
    {
        const auto canonical = _lexico.get_canonicals(unit);
        EXPECT_FALSE(canonical.empty());
        EXPECT_TRUE(canonical.size() > 0);
        std::wstring desc;
        for (const auto &c : canonical)
        {
            desc += c + L" ";
        }
        desc += std::to_wstring(canonical.size());
    }
}