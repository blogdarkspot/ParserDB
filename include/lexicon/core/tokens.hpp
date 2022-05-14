#ifndef __TOKENS__
#define __TOKENS__

#include <unordered_set>

#include <cwctype>
#include <string>
#include <vector>
#include <algorithm>

namespace lexicon
{
const static std::unordered_set<std::wstring> CategoryType = {L"A",      L"ADV",   L"CONJ", L"ART",  L"NUM",
                                                            L"SGL",    L"ABREV", L"V",    L"PRO",  L"DEN",
                                                            L"INTERJ", L"N",     L"CON",  L"PREP", L"PFX"};

const static std::unordered_set<std::wstring> tipo = {L"c",   L"o",   L"M",   L"X",   L"Def", L"Ind", L"Col",
                                                      L"Dem", L"Rel", L"Int", L"Tra", L"Pos", L"F",   L"L",
                                                      L"Pes", L"Cd",  L"Sub", L"Cor", L"Pr"};

const static std::unordered_set<std::wstring> traco{L"m", L"f", L"p", L"s", L"A", L"D", L"S", L"1", L"2", L"3"};

const static std::unordered_set<std::wstring> forma{L"A", L"d", L"N", L"O", L"R"};

const static std::unordered_set<std::wstring> tempo{L"W", L"G", L"K", L"P", L"I", L"J", L"F",
                                                    L"Q", L"B", L"T", L"U", L"Y", L"C", L"S"};

enum class Tipo
{
};

enum class Forma
{
    ACUSATIVA,
    DATIVA,
    NORMATIVA,
    OBLIQUA,
    REFLEXA
};

enum class Tempo
{
    INFINITIVO,
    GERUNDIO,
    PARTICIPIO,
    PRESENTE_INDICATIVO,
    PRETERITO_IMPERFEITO_INDICATIVO,
    PRETERITO_PERFEITO_INDICATIVO,
    FUTURO_DO_PRESENTE_DO_INDICATIVO,
    PRETERITO_MAIS_QUE_PERFEITO_DO_INDICATIVO,
    PRESENTE_DO_SUBJUNTIVO,
    IMPERFEITO_DO_SUBJUNTIVO,
    FUTURO_SUBJUNTIVO,
    IMPERATIVO,
    FUTURO_DO_PRETERITO
};

enum class Traco
{
    MASCULINO,
    FEMININO,
    PLURAL,
    SINGULAR,
    AUMENTATIVO,
    DIMINUTIVO,
    SUPERLATIVO,
    PRI_PESSOA,
    SEC_PESSOA,
    TER_PESSOA
};

enum class categoria
{
    A,
    ADV,
    CONJ,
    ART,
    NUM,
    SGL,
    ABREV,
    V,
    PRO,
    DEN,
    INTERJ,
    N,
    CON,
    PREP,
    PFX
};

static const std::unordered_set<std::wstring>::const_iterator tempo_enum_to_it(const Tempo &time)
{

    switch (time)
    {
    case Tempo::INFINITIVO: {
        return tempo.find(L"W");
    }
    case Tempo::GERUNDIO: {
        return tempo.find(L"G");
    }
    case Tempo::PARTICIPIO: {
        return tempo.find(L"K");
    }
    case Tempo::PRESENTE_INDICATIVO: {
        return tempo.find(L"P");
    }
    case Tempo::PRETERITO_IMPERFEITO_INDICATIVO: {
        return tempo.find(L"I");
    }
    case Tempo::PRETERITO_PERFEITO_INDICATIVO: {
        return tempo.find(L"J");
    }
    case Tempo::FUTURO_DO_PRESENTE_DO_INDICATIVO: {
        return tempo.find(L"F");
    }
    case Tempo::PRETERITO_MAIS_QUE_PERFEITO_DO_INDICATIVO: {
        return tempo.find(L"Q");
    }
    case Tempo::PRESENTE_DO_SUBJUNTIVO: {
        return tempo.find(L"S");
    }
    case Tempo::IMPERFEITO_DO_SUBJUNTIVO: {
        return tempo.find(L"T");
    }
    case Tempo::FUTURO_SUBJUNTIVO: {
        return tempo.find(L"U");
    }
    case Tempo::IMPERATIVO: {
        return tempo.find(L"Y");
    }
    case Tempo::FUTURO_DO_PRETERITO: {
        return tempo.find(L"C");
    }
    default:
        return tempo.end();
    }
};

static const std::unordered_set<std::wstring>::const_iterator traco_enum_to_it(const Traco &tra)
{
    switch (tra)
    {
    case Traco::MASCULINO: {
        return traco.find(L"m");
    }
    case Traco::FEMININO: {
        return traco.find(L"f");
    }
    case Traco::PLURAL: {
        return traco.find(L"p");
    }
    case Traco::SINGULAR: {
        return traco.find(L"s");
    }
    case Traco::AUMENTATIVO: {
        return traco.find(L"A");
    }
    case Traco::DIMINUTIVO: {
        return traco.find(L"D");
    }
    case Traco::SUPERLATIVO: {
        return traco.find(L"S");
    }
    case Traco::PRI_PESSOA: {
        return traco.find(L"1");
    }
    case Traco::SEC_PESSOA: {
        return traco.find(L"2");
    }
    case Traco::TER_PESSOA: {
        return traco.find(L"3");
    }
    default:
        return traco.end();
    }
};

static const std::unordered_set<std::wstring>::const_iterator categoria_enum_to_it(const categoria &cat)
{
    switch (cat)
    {
    case categoria::A: {
        return CategoryType.find(L"A");
    }
    case categoria::ADV: {
        return CategoryType.find(L"ADV");
    }
    case categoria::CONJ: {
        return CategoryType.find(L"CONJ");
    }
    case categoria::ART: {
        return CategoryType.find(L"ART");
    }
    case categoria::NUM: {
        return CategoryType.find(L"NUM");
    }
    case categoria::SGL: {
        return CategoryType.find(L"SGL");
    }
    case categoria::ABREV: {
        return CategoryType.find(L"ABREV");
    }
    case categoria::V: {
        return CategoryType.find(L"V");
    }
    case categoria::PRO: {
        return CategoryType.find(L"PRO");
    }
    case categoria::DEN: {
        return CategoryType.find(L"DEN");
    }
    case categoria::INTERJ: {
        return CategoryType.find(L"INTERJ");
    }
    case categoria::N: {
        return CategoryType.find(L"N");
    }
    case categoria::CON: {
        return CategoryType.find(L"CON");
    }
    case categoria::PREP: {
        return CategoryType.find(L"PREP");
    }
    case categoria::PFX: {
        return CategoryType.find(L"PFX");
    }
    default:
        return CategoryType.end();
    }
}

static void tokenize(std::wstring const &str, const std::wstring &delim, std::vector<std::wstring> &out)
{
    std::wstring tmp = str;
    size_t pos = 0;
    while ((pos = tmp.find(L" ")) != std::wstring::npos)
    {
        auto token = tmp.substr(0, pos);
        std::transform(token.begin(), token.end(), token.begin(), std::towlower);
        out.emplace_back(token);
        tmp.erase(0, pos + delim.length());
    }
}

}; // namespace dictionary
#endif