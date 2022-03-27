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

#include "dictionary.hpp"
#include "lexico.hpp"

namespace dictionary
{

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
    lexico parser_word(const delaf_word &word);
    void fillNum(lexico &lex, const delaf_word &word) const;
    void fillArt(lexico &lex, const delaf_word &word) const;
    void fillV(lexico &lex, const delaf_word &word) const;
    void fillN(lexico &lex, const delaf_word &word) const;
    void fillA(lexico &lex, const delaf_word &word) const;
    void fillABREV(lexico &lex, const delaf_word &word) const;
    void fillPRO(lexico &lex, const delaf_word &word) const;

    std::unordered_set<std::wstring> list_con;
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
        for (const auto &value : _controller.list_con)
        {
            _dic.split_contraction_word(value);
        }
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
    dictionary<std::wstring> _dic;
    dictionary_controller _controller;
};
} // namespace dictionary
#endif
