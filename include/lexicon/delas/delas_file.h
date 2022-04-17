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

#include "type.h"

namespace lexicon::delas
{

class DelasFile
{

  public:
    DelasFile(const std::string &path) : _path(path)
    {
    }

    bool open();
    void close();

    std::shared_ptr<DelasType> get_next();

    const std::vector<std::shared_ptr<DelasType>> get_all();

  private:

    std::shared_ptr<DelasType> parser_line(const std::wstring &line);
    const std::wstring parser_word(const std::wstring &line, std::size_t &pos) const;
    const std::wstring parser_canonical(const std::wstring &line, std::size_t &pos) const;
    const std::wstring parser_class(const std::wstring &line, std::size_t &pos) const;
    const std::wstring parser_trace(const std::wstring &line, std::size_t &pos) const;
    const std::vector<std::wstring> parser_flexion(const std::wstring &line, std::size_t &pos) const;

    std::string _path;
    std::unordered_set<std::wstring> _class;
    std::unordered_set<std::wstring> _trace;
    std::unordered_set<std::wstring> _flexion;
    std::wifstream _fstream;
    std::wifstream _contracoes;
};
} // namespace dictionary
#endif
