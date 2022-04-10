#include <algorithm>
#include <benchmark/benchmark.h>

#include "delaf_dictionary.h"
#include "dictionary.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <cwctype>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

static const std::wstring text =
    L"O caso de uso é a forma mais eficaz de saber se uma ideia pode ser realmente boa "
    L"para seu negócio Muito se fala "
    L"na importância de arriscar mas para fazer isso com chances reais de sucesso é preciso saber quais são os riscos "
    L"e "
    L"como eles podem ser equilibrados pelos possíveis benefícios O caso de uso permite que as informações sejam "
    L"apresentadas de forma sólida e estruturada para que os tomadores de decisão considerem seu projeto como um todo "
    L"e "
    L"sejam convencidos pela proposta ou apontem possíveis melhorias Não importa que cargo você ocupa na empresa ou o "
    L"setor no qual está alocado se quer receber apoio e até patrocínio para colocar sua ideia em prática essa etapa é "
    L"super importante";

static void BM_search_by_word(benchmark::State &state)
{
    auto delaf_dic = dictionary::delaf_dictionary("delaf_linux.dic");
    if (!delaf_dic.open())
        return;
    if (!delaf_dic.parser())
        return;
    std::wstring delim = L" ";
    std::vector<std::wstring> out;
    dictionary::tokenize(text, delim, out);
    unsigned long long i = 0;
    const auto size = out.size();

    dictionary::dictionary<std::wstring> dic = delaf_dic.getDictionary();

    for (auto _ : state)
    {
        dic.get_lex_unities_by_word(out[i]);
        i = i + 1 == size ? 0 : ++i;
    }
}
// Register the function as a benchmark
BENCHMARK(BM_search_by_word);

BENCHMARK_MAIN();