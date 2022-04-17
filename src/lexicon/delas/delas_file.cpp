#include "delas/delas_file.h"
#include "delas/type.h"
#include <fstream>
#include <iostream>
#include <locale>
#include <string>

namespace lexicon::delas 
{

bool DelasFile::open()
{
    try
    {
        _fstream.open(_path);
      //  _contracoes.open("contracoes.txt", std::wofstream::out);
        std::locale utf8{"en_US.UTF-8"};
        _fstream.imbue(utf8);
     //   _contracoes.imbue(utf8);
    }
    catch (const std::ios_base::failure &e)
    {
        // TODO: Log error
    }
    return _fstream.is_open() && _fstream.good() && _contracoes.is_open() && _contracoes.good();
};

void DelasFile::close()
{
    _fstream.close();
}

/*
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
    _contracoes.close();*/

const std::vector<std::shared_ptr<DelasType>> DelasFile::get_all()
{
    std::vector<std::shared_ptr<DelasType>> ret;
    std::wstring line;
    while (std::getline(_fstream, line))
    {
        try
        {
            auto entry = parser_line(line);
            if(entry != nullptr)
            {
                ret.emplace_back(entry);
            }

        }
        catch (const std::exception &e)
        {
            ret.clear();
        }
    }
    
    return ret;
}

std::shared_ptr<DelasType> DelasFile::get_next()
{
    std::shared_ptr<DelasType> ret;

    std::wstring line;

    if(std::getline(_fstream, line))
    {
        ret = parser_line(line);
    }
    return ret;
}

std::shared_ptr<DelasType> DelasFile::parser_line(const std::wstring &line)
{
    size_t pos = 0;

    auto word = parser_word(line, pos);
    auto canonical = parser_canonical(line, pos);
    auto wclass = parser_class(line, pos);
    if (wclass == L"")
    {
        return nullptr;
    }
    auto trace = parser_trace(line, pos);
    auto flexion = parser_flexion(line, pos);
    return std::make_shared<DelasType>(word, canonical, wclass, trace, flexion);
}

const std::wstring DelasFile::parser_word(const std::wstring &line, std::size_t &pos) const
{
    auto tmp = line.find_first_of(wchar_t(','));
    auto word = line.substr(pos, tmp - pos);
    pos = tmp + 1;
    return word;
}

const std::wstring DelasFile::parser_canonical(const std::wstring &line, std::size_t &pos) const
{
    auto tmp = line.find_first_of(wchar_t('.'), pos);
    auto canonical = line.substr(pos, tmp - pos);
    pos = tmp + 1;
    return canonical;
}
const std::wstring DelasFile::parser_class(const std::wstring &line, std::size_t &pos) const
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

const std::wstring DelasFile::parser_trace(const std::wstring &line, std::size_t &pos) const
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

const std::vector<std::wstring> DelasFile::parser_flexion(const std::wstring &line, std::size_t &pos) const
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



}; // namespace dictionary
