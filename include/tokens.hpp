#ifndef __TOKENS__
#define __TOKENS__

#include "string_def.hpp"
#include <unordered_set>

#include <cwctype>
#include <string>
#include <vector>

namespace dictionary
{
const static std::unordered_set<std::wstring> dcategoria = {L"A",      L"ADV",   L"CONJ", L"ART",  L"NUM",
                                                            L"SGL",    L"ABREV", L"V",    L"PRO",  L"DEN",
                                                            L"INTERJ", L"N",     L"CON",  L"PREP", L"PFX"};

const static std::unordered_set<std::wstring> tipo = {L"c",   L"o",   L"M",   L"X",   L"Def", L"Ind",
                                                      L"Col", L"Dem", L"Rel", L"Int", L"Tra", L"Pos",
                                                      L"Pes", L"Cd",  L"Sub", L"Cor", L"Pr"};

const static std::unordered_set<std::wstring> traco{L"m", L"f", L"p", L"s", L"A", L"D", L"S", L"1", L"2", L"3"};

const static std::unordered_set<std::wstring> forma{L"A", L"d", L"N", L"O", L"R"};

const static std::unordered_set<std::wstring> tempo{L"W", L"G", L"K", L"P", L"I", L"J", L"F",
                                                    L"Q", L"B", L"T", L"U", L"Y", L"C", L"S"};

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

static const std::unordered_set<std::wstring>::const_iterator categoria_enum_to_it(const categoria &cat)
{
    switch (cat)
    {
    case categoria::A: {
        return dcategoria.find(L"A");
    }
    case categoria::ADV: {
        return dcategoria.find(L"ADV");
    }
    case categoria::CONJ: {
        return dcategoria.find(L"CONJ");
    }
    case categoria::ART: {
        return dcategoria.find(L"ART");
    }
    case categoria::NUM: {
        return dcategoria.find(L"NUM");
    }
    case categoria::SGL: {
        return dcategoria.find(L"SGL");
    }
    case categoria::ABREV: {
        return dcategoria.find(L"ABREV");
    }
    case categoria::V: {
        return dcategoria.find(L"V");
    }
    case categoria::PRO: {
        return dcategoria.find(L"PRO");
    }
    case categoria::DEN: {
        return dcategoria.find(L"DEN");
    }
    case categoria::INTERJ: {
        return dcategoria.find(L"INTERJ");
    }
    case categoria::N: {
        return dcategoria.find(L"N");
    }
    case categoria::CON: {
        return dcategoria.find(L"CON");
    }
    case categoria::PREP: {
        return dcategoria.find(L"PREP");
    }
    case categoria::PFX: {
        return dcategoria.find(L"PFX");
    }
    default:
        return dcategoria.end();
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