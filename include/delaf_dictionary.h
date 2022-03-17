/**
MIT License

Copyright (c) 2022 Fábio da Silva Santana

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#ifndef __DIC_FILE_H__
#define __DIC_FILE_H__

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dictionary
{

const std::unordered_set<std::wstring> dcategoria = {L"A",   L"ADV", L"CONJ",   L"ART", L"NUM", L"SGL",  L"ABREV", L"V",
                                                     L"PRO", L"DEN", L"INTERJ", L"N",   L"CON", L"PREP", L"PFX"};

const std::unordered_set<std::wstring> tipo = {L"c",   L"o",   L"M",   L"X",   L"Def", L"Ind", L"Col", L"Dem", L"Rel",
                                               L"Int", L"Tra", L"Pos", L"Pes", L"Cd",  L"Sub", L"Cor", L"Pr"};

const std::unordered_set<std::wstring> traco{L"m", L"f", L"p", L"s", L"A", L"D", L"S", L"1", L"2", L"3"};

const std::unordered_set<std::wstring> forma{L"A", L"d", L"N", L"O", L"R"};

const std::unordered_set<std::wstring> tempo{L"W", L"G", L"K", L"P", L"I", L"J", L"F",
                                             L"Q", L"B", L"T", L"U", L"Y", L"C", L"S"};

struct lexico
{
    std::unordered_set<std::wstring>::const_iterator categoria;
    std::unordered_set<std::wstring>::const_iterator tipo;
    std::vector<std::unordered_set<std::wstring>::const_iterator> traco;
    std::unordered_set<std::wstring>::const_iterator forma;
    std::unordered_set<std::wstring>::const_iterator tempo;
    std::unordered_set<std::wstring>::const_iterator prefix;
    std::unordered_set<std::wstring>::const_iterator suffix;
};

template <typename _Key, typename _Value> class dictionary
{
  public:
    void add(_Key word, _Value desc)
    {
        _library_by_word[word].emplace_back(desc);
        //   _library_by_canonical[desc._canonical].emplace_back(word);
    }

    bool remove(_Key word)
    {
    }

    _Value search(_Key word)
    {
    }

    void print()
    {
        std::cout << "Dicitionary size " << _library_by_word.size() << std::endl;
        int i = 0;
        for (const auto &value : _library_by_word)
        {
            if (i == 100)
            {
                break;
            }
            std::wstring val;
            std::wstring cat;
            for (const auto &data : value.second)
            {
                for (const auto &tra : data.traco)
                    val += *tra + L" ";

                cat += *(data.categoria) + L" ";
            }
            std::wcout << value.first << L" " << val << L" " << cat << std::endl;
            ++i;
        }
    }

  private:
    std::unordered_map<_Key, std::vector<_Value>> _library_by_word;
    std::unordered_map<_Key, std::vector<_Key>> _library_by_canonical;
};

struct delaf_word
{
    delaf_word(const std::wstring &word, const std::wstring &canonical, const std::wstring &wclass,
               const std::wstring &trace, const std::vector<std::wstring> &flexion)
        : _word(std::move(word)), _canonical(std::move(canonical)), _wclass(std::move(wclass)),
          _trace(std::move(trace)), _flexion(std::move(flexion))
    {
    }
    const std::wstring _word;
    const std::wstring _canonical;
    const std::wstring _wclass;
    const std::wstring _trace;
    const std::vector<std::wstring> _flexion;
};

class dictionary_controller
{
  public:
    lexico parser_word(const delaf_word &word) const;
    void fillNum(lexico &lex, const delaf_word &word) const;
    void fillArt(lexico &lex, const delaf_word &word) const;
    void fillV(lexico &lex, const delaf_word &word) const;
    void fillN(lexico &lex, const delaf_word &word) const;
    void fillA(lexico &lex, const delaf_word &word) const;
    void fillABREV(lexico &lex, const delaf_word &word) const;
    void fillPRO(lexico &lex, const delaf_word &word) const;
};

class delaf_dictionary
{

  public:
    delaf_dictionary(const std::string &path) : _path(path)
    {
    }

    bool open();
    void close();
    bool parser();

    const std::vector<delaf_word> &get_words() const
    {
        return _words;
    }

    void print_values()
    {
        /*
        std::vector<std::vector<std::wstring>> print;
        std::size_t  max_x = 3, max_y = 0;
        {
          std::vector<std::wstring> tmp;
          for( const auto& value : _trace)
          {
              tmp.emplace_back(value);
          }
          max_y = tmp.size();
           print.emplace_back(std::move(tmp));
        }

        {
          std::vector<std::wstring> tmp;
          for( const auto& value : _class)
          {
              tmp.emplace_back(value);
          }
          if(max_y < tmp.size())
          {
             max_y = tmp.size();
          }
           print.emplace_back(std::move(tmp));
        }

        {
          std::vector<std::wstring> tmp;
          for( const auto& value : _flexion)
          {
              tmp.emplace_back(value);
          }
          if(max_y < tmp.size())
          {
             max_y = tmp.size();
          }
           print.emplace_back(std::move(tmp));
        }
        for(int y = 0; y < max_y; ++y)
        {
                std::wstring value = L"";
                for(int x = 0; x < max_x; ++x)
                {
                    if(y < print[x].size() )
                          value +=  print[x][y]  + L"\t";
                    else
                            value += L"\t\t\t";
                }
                std::wcout << value << std::endl;
        }*/

        _dic.print();
    }

  private:
    const delaf_word parser_line(const std::wstring &) const;
    const std::wstring parser_word(const std::wstring &line, std::size_t &pos) const;
    const std::wstring parser_canonical(const std::wstring &line, std::size_t &pos) const;
    const std::wstring parser_class(const std::wstring &line, std::size_t &pos) const;
    const std::wstring parser_trace(const std::wstring &line, std::size_t &pos) const;
    const std::vector<std::wstring> parser_flexion(const std::wstring &line, std::size_t &pos) const;

    std::string _path;
    std::vector<delaf_word> _words;
    std::unordered_set<std::wstring> _class;
    std::unordered_set<std::wstring> _trace;
    std::unordered_set<std::wstring> _flexion;
    std::wifstream _fstream;
    dictionary<std::wstring, lexico> _dic;
    dictionary_controller _controller;
};
} // namespace dictionary
#endif
