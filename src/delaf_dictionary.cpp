#include "delaf_dictionary.h"
#include "tokens.hpp"
#include <fstream>
#include <iostream>
#include <locale>
#include <string>

namespace dictionary
{

bool delaf_dictionary::open()
{
    try
    {
        _fstream.open(_path);
        _contracoes.open("contracoes.txt", std::wofstream::out);
        std::locale utf8{"en_US.UTF-8"};
        _fstream.imbue(utf8);
        _contracoes.imbue(utf8);
    }
    catch (const std::ios_base::failure &e)
    {
        // TODO: Log error
    }
    return _fstream.is_open() && _fstream.good() && _contracoes.is_open() && _contracoes.good();
};

void delaf_dictionary::close()
{
    _fstream.close();
}

bool delaf_dictionary::parser()
{
    auto ret = true;
    std::wstring line;
    while (ret && std::getline(_fstream, line))
    {
        try
        {
            auto word = parser_line(line);
            if (word._word != L"")
            {
                auto lex = _controller.parser_word(word);
                if (lex.categoria != nullptr)
                {
                    _dic.add(word._word, word._canonical, std::make_shared<lexico>(lex));
                }
            }
        }
        catch (const std::exception &e)
        {
            ret = false;
        }
    }
    while (ret && std::getline(_contracoes, line))
    {
        vector<wstring> split;
        size_t pos = 0;
        do
        {
            auto tmp = line.find_first_of(L" ", pos);
            if (tmp != line.npos)
            {
                split.emplace_back(line.substr(pos, tmp - pos));
            }
            else
            {
                split.emplace_back(line.substr(pos));
                break;
            }
            pos = tmp + 1;
        } while (true);
        _dic.add_contraction(split[0], split[1], split[2], split[3], split[4]);
    }
    _contracoes.close();
    return ret;
}

// Palavra,Canonica.Classe+traco:<flexao1>:<flexao2>:...<flexaoN>

const delaf_word delaf_dictionary::parser_line(const std::wstring &line) const
{
    size_t pos = 0;

    auto word = parser_word(line, pos);
    auto canonical = parser_canonical(line, pos);
    auto wclass = parser_class(line, pos);
    if (wclass == L"")
    {
        return delaf_word(L"", L"", L"", L"", {});
    }
    auto trace = parser_trace(line, pos);
    auto flexion = parser_flexion(line, pos);
    auto delaf_w = delaf_word(word, canonical, wclass, trace, flexion);

    return delaf_w;
}

const std::wstring delaf_dictionary::parser_word(const std::wstring &line, std::size_t &pos) const
{
    auto tmp = line.find_first_of(wchar_t(','));
    auto word = line.substr(pos, tmp - pos);
    pos = tmp + 1;
    return word;
}

const std::wstring delaf_dictionary::parser_canonical(const std::wstring &line, std::size_t &pos) const
{
    auto tmp = line.find_first_of(wchar_t('.'), pos);
    auto canonical = line.substr(pos, tmp - pos);
    pos = tmp + 1;
    return canonical;
}
const std::wstring delaf_dictionary::parser_class(const std::wstring &line, std::size_t &pos) const
{
    auto tmp = line.find_first_of(wchar_t(':'), pos);
    auto wclass = line.substr(pos, tmp - pos);

    if (wclass.find(L"DET+Art", 0, 7) == 0)
    {
        // Magic number "DET+Art".size() == 7
        if (wclass.size() > 7)
        {
            auto t = line.find_last_of(wchar_t('+'));
            tmp = t;
        }
        wclass = L"ART";
    }
    else if (wclass.find(L"DET+Num", 0, 7) == 0)
    {

        // Magic number "DET+Num".size() == 7
        if (wclass.size() > 7)
        {
            auto t = line.find_last_of(wchar_t('+'));
            tmp = t;
        }
        wclass = L"NUM";
    }
    else if (wclass == L"V+PRO")
    {
        wclass = L"";
    }
    else
    {
        auto t = line.find_last_of(wchar_t('+'));
        if (t != std::wstring::npos)
        {
            wclass = line.substr(pos, t - pos);
            tmp = t;
        }
    }
    pos = tmp + 1;

    return wclass;
}

const std::wstring delaf_dictionary::parser_trace(const std::wstring &line, std::size_t &pos) const
{
    std::wstring trace;
    if (line[pos - 1] == wchar_t('+'))
    {
        auto tmp = line.find_first_of(wchar_t(':'), pos);
        trace = line.substr(pos, tmp - pos);
        pos = tmp + 1;
    }
    return trace;
}

const std::vector<std::wstring> delaf_dictionary::parser_flexion(const std::wstring &line, std::size_t &pos) const
{
    std::vector<std::wstring> ret;
    if (pos < line.size())
    {
        size_t tmp = 0;
        do
        {
            tmp = line.find_first_of(wchar_t(':'), pos);
            if (tmp == std::wstring::npos)
            {
                if (line[pos - 1] == L':')
                {
                    std::wstring val = line.substr(pos);
                    ret.emplace_back(val);
                }
                pos = std::wstring::npos;
            }
            else
            {
                auto val = line.substr(pos, tmp - pos);
                ret.emplace_back(val);
                pos = tmp + 1;
            }

        } while (tmp != std::string::npos && pos < line.size());
    }
    return ret;
}

lexico dictionary_controller::parser_word(const delaf_word &word)
{
    if (word._wclass == L"PREPXPRO")
    {
        int i = 0;
    }
    const auto it = dcategoria.find(word._wclass);
    lexico ret;

    if (it != dcategoria.end())
    {
        // categoria
        ret.categoria = it;

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
            if (*it == L"PREP")
            {
                _preps << word._word << std::endl;
            }
            if (word._flexion.size() > 0 || !word._trace.empty())
            {
                std::cout << "aqui" << std::endl;
            }
        }
        else
        {
            std::cout << "Unkown" << std::endl;
        }
    }
    else
    {
        auto xpos = word._wclass.find(L"X");
        if (xpos != std::wstring::npos)
        {
            list_con.insert(word._word);
            ret.categoria = dcategoria.find(L"CON");
            auto prefix = word._wclass.substr(0, xpos);
            auto suffix = word._wclass.substr(xpos + 1);

            auto iprefix = dcategoria.find(prefix);
            auto isuffix = dcategoria.find(suffix);

            if (iprefix == dcategoria.end())
            {
                std::wcout << prefix << std::endl;
            }
            else
            {
                ret.prefix = iprefix;
            }

            if (isuffix == dcategoria.end())
            {
                if (suffix == L"DET+Art" || suffix == L"DET")
                {
                    ret.suffix = dcategoria.find(L"ART");
                }
                else if (suffix == L"DET+Num")
                {
                    ret.suffix = dcategoria.find(L"NUM");
                }
                else
                {
                    std::wcout << suffix << std::endl;
                }

                if (!word._trace.empty())
                {
                    auto itipo = tipo.find(word._trace);
                    if (itipo != tipo.end())
                    {
                        ret.tipo = itipo;
                    }
                }

                for (int i = 0; i < word._flexion.size(); ++i)
                {
                    for (int j = 0; j < word._flexion[i].size(); ++j)
                    {
                        auto itraco = traco.find(word._flexion[i].substr(j, 1));
                        if (itraco == traco.end())
                        {
                            std::wcout << "aqui" << std::endl;
                        }
                        ret.traco.emplace_back(itraco);
                    }
                }
            }
            else
            {
                ret.suffix = isuffix;
            }
        }
        else
        {
            std::wcout << word._wclass << std::endl;
        }
    }
    return ret;
}

void dictionary_controller::fillNum(lexico &lex, const delaf_word &word) const
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
                    lex.tipo = tipo.find(L"Col");
                }
                else if (tp == L"F")
                {
                    lex.tipo = tipo.find(L"X");
                }
            }
            else
            {
                lex.tipo = rtp;
            }
            for (int i = 1; i < word._flexion[j].size(); ++i)
            {
                const auto t = traco.find(word._flexion[j].substr(i, 1));
                if (t == traco.end())
                {
                    std::wcout << L"traco not found " << word._flexion[0] << L" pos  " << i << std::endl;
                }
                else
                {
                    lex.traco.emplace_back(t);
                }
            }
        }
    }
}
void dictionary_controller::fillArt(lexico &lex, const delaf_word &word) const
{
    const auto t = tipo.find(word._trace);

    if (t == tipo.end())
    {
        std::cout << "error" << std::endl;
    }

    lex.tipo = t;

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
                    std::wcout << L"traco not found " << word._flexion[0] << L" pos  " << i << std::endl;
                }
                else
                {
                    lex.traco.emplace_back(t);
                }
            }
        }
    }
}
void dictionary_controller::fillV(lexico &lex, const delaf_word &word) const
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
                std::cout << "tempo invalido" << std::endl;
            }
            else
            {
                lex.tempo = temp;
            }
            for (int i = 1; i < word._flexion[j].size(); ++i)
            {
                const auto t = traco.find(word._flexion[j].substr(i, 1));
                if (t == traco.end())
                {
                    std::wcout << L"traco not found " << word._flexion[j] << L" pos  " << i << std::endl;
                }
                else
                {
                    lex.traco.emplace_back(t);
                }
            }
        }
    }
    // fill tipo
    const auto t = tipo.find(word._trace);
    if (t != tipo.end())
    {
        lex.tipo = t;
    }
}
void dictionary_controller::fillN(lexico &lex, const delaf_word &word) const
{
    // fill  traco
    if (word._flexion.size() >= 1)
    {
        for (int i = 0; i < word._flexion[0].size(); ++i)
        {
            const auto t = traco.find(word._flexion[0].substr(i, 1));
            if (t == traco.end())
            {
                std::wcout << L"traco not found " << word._flexion[0] << L" pos  " << i << std::endl;
            }
            else
            {
                lex.traco.emplace_back(t);
            }
        }
    }
    // fill tipo
    const auto t = tipo.find(word._trace);
    if (t != tipo.end())
    {
        lex.tipo = t;
    }
}
void dictionary_controller::fillA(lexico &lex, const delaf_word &word) const
{
    for (int i = 0; i < word._flexion.size(); ++i)
    {
        for (int j = 0; j < word._flexion[i].size(); ++j)
        {
            auto grau = word._flexion[i].substr(0, 1);
            auto itraco = traco.find(grau);
            if (itraco == traco.end())
            {
                std::wcout << L"Traço não encontrado" << grau << std::endl;
            }
        }
    }
}

void dictionary_controller::fillABREV(lexico &lex, const delaf_word &word) const
{
}

void dictionary_controller::fillPRO(lexico &lex, const delaf_word &word) const
{
    auto itipo = tipo.find(word._trace);

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

}; // namespace dictionary
