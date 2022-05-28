#include "delas/delas_parser.h"
#include "core/types.h"
#include "delas/type.h"
#include "core/tokens.hpp"
#include <iostream>

namespace lexicon::delas
{

DelasToLexicon::_LexiconType DelasToLexicon::parser_word(const DelasType& word)
{
    const auto it = CategoryType.find(word._wclass);

    DelasToLexicon::_LexiconType ret;

    if (it != CategoryType.end())
    {
        // categoria
        ret.category->categoria = it;
        ret.entry  = word._word;
        ret.lemma = word._canonical;

        if (*it == L"NUM")
        {
            fillNum(ret, word);
        }
        else if (*it == L"ART")
        {
            fillArt(ret, word);
        }
        else if (*it == L"N")
        {
            fillN(ret, word);
        }
        else if (*it == L"V")
        {
            fillV(ret, word);
        }
        else if (*it == L"A")
        {
            fillA(ret, word);
        }
        else if (*it == L"ABREV")
        {
            fillABREV(ret, word);
        }
        else if (*it == L"PRO")
        {
            fillPRO(ret, word);
        }
        else if (*it == L"PREP" || *it == L"CONJ" || *it == L"ADV" || *it == L"PFX" || *it == L"SIGL" ||
                 *it == L"INTERJ")
        {
        }
        else
        {
        }
    }
    else
    {
        auto xpos = word._wclass.find(L"X");
        if (xpos != std::wstring::npos)
        {
            ret.category->categoria = CategoryType.find(L"CON");
            ret.entry = word._word;
            ret.lemma = word._canonical;
            auto prefix = word._wclass.substr(0, xpos);
            auto suffix = word._wclass.substr(xpos + 1);

            auto iprefix = CategoryType.find(prefix);
            auto isuffix = CategoryType.find(suffix);

            if (iprefix == CategoryType.end())
            {
            }
            else
            {
                ret.category->prefix = iprefix;
            }

            if (isuffix == CategoryType.end())
            {
                if (suffix == L"DET+Art" || suffix == L"DET")
                {
                    ret.category->suffix = CategoryType.find(L"ART");
                }
                else if (suffix == L"DET+Num")
                {
                    ret.category->suffix = CategoryType.find(L"NUM");
                }
                else
                {
                }

                if (!word._trace.empty())
                {
                    auto itipo = tipo.find(word._trace);
                    if (itipo != tipo.end())
                    {
                        ret.category->tipo = itipo;
                    }
                }

                for (int i = 0; i < word._flexion.size(); ++i)
                {
                    for (int j = 0; j < word._flexion[i].size(); ++j)
                    {
                        auto itraco = traco.find(word._flexion[i].substr(j, 1));
                        if (itraco == traco.end())
                        {
                        }
                        ret.category->traco.emplace_back(itraco);
                    }
                }
            }
            else
            {
                ret.category->suffix = isuffix;
            }
        }
        else
        {
        }
    }
    return ret;
}

void DelasToLexicon::fillNum(DelasToLexicon::_LexiconType &lex, const DelasType &word)
{
    const auto t = tipo.find(word._trace);

    if (word._flexion.size() >= 1)
    {
        for (int j = 0; j < word._flexion.size(); ++j)
        {
            if (word._flexion[j].empty())
            {
                continue;
            }
            auto tp = word._flexion[j].substr(0, 1);
            const auto rtp = tipo.find(tp);
            if (rtp == tipo.end())
            {
                if (tp == L"L")
                {
                    lex.category->tipo = tipo.find(L"Col");
                }
                else if (tp == L"F")
                {
                    lex.category->tipo = tipo.find(L"X");
                }
            }
            else
            {
                lex.category->tipo = rtp;
            }
            for (int i = 1; i < word._flexion[j].size(); ++i)
            {
                const auto t = traco.find(word._flexion[j].substr(i, 1));
                if (t == traco.end())
                {
                }
                else
                {
                    lex.category->traco.emplace_back(t);
                }
            }
        }
    }
}

void DelasToLexicon::fillArt(DelasToLexicon::_LexiconType &lex, const DelasType &word)
{
    const auto t = tipo.find(word._trace);

    if (t == tipo.end())
    {
    }

    lex.category->tipo = t;

    if (word._flexion.size() >= 1)
    {
        for (int j = 0; j < word._flexion.size(); ++j)
        {
            if (word._flexion[j].empty())
            {
                continue;
            }
            for (int i = 0; i < word._flexion[j].size(); ++i)
            {
                const auto t = traco.find(word._flexion[j].substr(i, 1));
                if (t == traco.end())
                {
                }
                else
                {
                    lex.category->traco.emplace_back(t);
                }
            }
        }
    }
}

void DelasToLexicon::fillV(DelasToLexicon::_LexiconType &lex, const DelasType &word) 
{
    if (word._flexion.size() >= 1)
    {
        for (int j = 0; j < word._flexion.size(); ++j)
        {
            if (word._flexion[j].empty())
            {
                continue;
            }

            const auto temp = tempo.find(word._flexion[j].substr(0, 1));
            if (temp == tempo.end())
            {
            }
            else
            {
                lex.category->tempo = temp;
            }
            for (int i = 1; i < word._flexion[j].size(); ++i)
            {
                const auto t = traco.find(word._flexion[j].substr(i, 1));
                if (t == traco.end())
                {
                }
                else
                {
                    lex.category->traco.emplace_back(t);
                }
            }
        }
    }
    // fill tipo
    const auto t = tipo.find(word._trace);
    if (t != tipo.end())
    {
        lex.category->tipo = t;
    }
}

void DelasToLexicon::fillN(DelasToLexicon::_LexiconType &lex, const DelasType &word) 
{
    // fill  traco
    if (word._flexion.size() >= 1)
    {
        for (int i = 0; i < word._flexion[0].size(); ++i)
        {
            const auto t = traco.find(word._flexion[0].substr(i, 1));
            if (t == traco.end())
            {
            }
            else
            {
                lex.category->traco.emplace_back(t);
            }
        }
    }
    // fill tipo
    const auto t = tipo.find(word._trace);
    if (t != tipo.end())
    {
        lex.category->tipo = t;
    }
}

void DelasToLexicon::fillA(DelasToLexicon::_LexiconType &lex, const DelasType &word)
{
    for (int i = 0; i < word._flexion.size(); ++i)
    {
        for (int j = 0; j < word._flexion[i].size(); ++j)
        {
            auto grau = word._flexion[i].substr(0, 1);
            auto itraco = traco.find(grau);
            if (itraco == traco.end())
            {
            }
        }
    }
}

void DelasToLexicon::fillABREV(DelasToLexicon::_LexiconType &lex, const DelasType &word)
{
}

void DelasToLexicon::fillPRO(DelasToLexicon::_LexiconType &lex, const DelasType &word)
{
    auto itipo = tipo.find(word._trace);

    lex.category->tipo = itipo;

    for (int i = 0; i < word._flexion.size(); ++i)
    {
        int j = 1;
        auto iforma = forma.find(word._flexion[i].substr(0, 1));
        if (iforma == forma.end())
        {
            //    std::wcout << word._flexion[i].substr(0, 1);
            j = 0;
        }

        for (; j < word._flexion[i].size(); ++j)
        {
            auto iflexion = traco.find(word._flexion[i].substr(i, 1));
        }
    }
}
};
