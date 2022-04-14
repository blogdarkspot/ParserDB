#ifndef __CETEN_FOLHA__
#define __CETEN_FOLHA__

#include <cstring>
#include <cwctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace visl_cg
{

struct lex_unit_ceten
{
    std::wstring verbete;
    std::wstring lemma;
    std::wstring category;
    std::wstring syntatic_function_tags;
    std::vector<std::wstring> morphology_inflexion;
    std::vector<std::wstring> secundary_information;
    std::wstring pontuacao;
    int pos;
    int parent;
};

struct corpus_unit
{
    corpus_unit(int size) : _size(size)
    {
        _tree = new int *[size + 1];
        for (int i = 0; i <= size; ++i)
        {
            _tree[i] = new int[size + 1];
            std::memset(_tree[i], 0, sizeof(int) * (size + 1));
        }
        _lexicon.resize(size + 1);
    }

    void add_lex_unit_ceten(std::shared_ptr<lex_unit_ceten> unit)
    {
        _tree[unit->parent][unit->pos] = 1;
        if (unit->parent == 0)
            _roots.emplace_back(unit->pos);

        _lexicon[unit->pos] = unit;
    }

    ~corpus_unit()
    {
        for (int i = 0; i <= _size; ++i)
        {
            delete[] _tree[i];
        }
        delete[] _tree;
    }

    int **_tree;
    int _size;
    std::vector<int> _roots;
    std::vector<std::shared_ptr<lex_unit_ceten>> _lexicon;
};

class decoder
{

  public:
    bool skip_line(const std::wstring &line)
    {
        return false;
    }
    bool is_upper_case(const std::wstring &s)
    {
        bool ret = true;
        for (const auto &c : s)
        {
            if (!std::iswupper(c))
            {
                ret = false;
                break;
            }
        }
        return ret;
    }

    enum class STATE_MACHINE
    {
        VERBETE,
        CANONICAL,
        SECUNDARY_INFO,
        CATEGORY,
        MORPHOLOGY,
        SYNTATIC_FUNCTION,
        TREE,
        UNKOWN
    };

    void next_stage(STATE_MACHINE &current_stage, const std::wstring &line, int pos)
    {
        if (line[pos] == L'[')
            current_stage = STATE_MACHINE::CANONICAL;
        else if (line[pos] == L'<')
            current_stage = STATE_MACHINE::SECUNDARY_INFO;
        else if (line[pos] == L'@')
            current_stage = STATE_MACHINE::SYNTATIC_FUNCTION;
        else if (line[pos] == L'#')
            current_stage = STATE_MACHINE::TREE;
        else if (std::iswupper(line[pos]))
        {
            if (current_stage == STATE_MACHINE::CATEGORY)
            {
                current_stage = STATE_MACHINE::MORPHOLOGY;
            }
            else
            {
                current_stage = STATE_MACHINE::CATEGORY;
            }
        }
        else
        {
            current_stage = STATE_MACHINE::UNKOWN;
        }
    }
    std::shared_ptr<lex_unit_ceten> parser_line(std::wstring line)
    {
        int pos = 0;
        auto ret = std::make_shared<lex_unit_ceten>();
        bool category = false;
        bool morphology_inflexion = false;

        STATE_MACHINE state = STATE_MACHINE::VERBETE;

        while (pos < line.size())
        {
            switch (state)
            {
            case STATE_MACHINE::VERBETE: {
                auto tmp = line.npos;
                if (pos == 0)
                    tmp = line.find_first_of(L'\t', pos);
                if (tmp == line.npos)
                {
                    break;
                }
                auto sub = line.substr(pos, tmp);
                ret->verbete = sub;
                pos += (tmp + 1);
                next_stage(state, line, pos);
                break;
            }
            case STATE_MACHINE::CANONICAL: {
                auto tmp = line.npos;
                tmp = line.find_first_of(L']', pos);
                if (tmp == line.npos)
                {
                    break;
                }
                auto sub = line.substr(pos + 1, tmp - pos - 1);
                pos += (tmp + 2);
                next_stage(state, line, pos);
                break;
            }
            case STATE_MACHINE::SECUNDARY_INFO: {
                auto tmp = line.npos;
                tmp = line.find_first_of(pos, L'>');

                if (tmp != line.npos)
                {
                }
                else if (tmp == line.npos)
                {
                    if (line.find_first_of(L"<DERS", pos) != line.npos ||
                        line.find_first_of(L"<DERP", pos) != line.npos)
                    {
                        auto f = line.find_first_of(L'-', pos);
                        auto s = line.find_first_of(L' ', f);
                        auto sub = line.substr(pos, s - pos);
                        ret->secundary_information.push_back(sub);
                        pos += tmp + 1;
                        next_stage(state, line, pos);
                        break;
                    }
                }
                break;
            }
            case STATE_MACHINE::CATEGORY: {
                auto tmp = line.npos;
                tmp = line.find_first_of(pos, L' ');
                auto cat = line.substr(pos, tmp - pos);
                if (!is_upper_case(cat))
                    break;
                ret->category = cat;
                pos += tmp + 1;
            }
            case STATE_MACHINE::MORPHOLOGY: {
                break;
            }
            case STATE_MACHINE::SYNTATIC_FUNCTION: {
                break;
            }
            case STATE_MACHINE::TREE: {
                break;
            }
            case STATE_MACHINE::UNKOWN: {
                break;
            }
            }
        }

        return ret;
    }

    std::shared_ptr<corpus_unit> foreach_line(const std::vector<std::wstring> &lines)
    {
        auto ret = std::make_shared<corpus_unit>(lines.size());
        for (const auto &line : lines)
        {
            if (line.size() == 0)
                continue;
            ret->add_lex_unit_ceten(parser_line(line));
        }
        return ret;
    }

    bool load_file(std::string path)
    {

        _fstream.open(path);
        std::locale utf8{"en_US.UTF-8"};
        _fstream.imbue(utf8);
        return _fstream.is_open() && _fstream.good();
    };

    bool parser_file()
    {
        std::stack<std::wstring> stack;
        auto ret = true;
        std::wstring line;
        bool start = false;
        std::vector<std::wstring> lines;
        while (ret && std::getline(_fstream, line))
        {
            if (line == L"<s>" || line == L"<s frag>" || line == L"<a>" || line == L"<t>" || line == L"<p>" ||
                line == L"<caixa>" || line == L"<situacao>" || line == L"<li>" || line.find(L"<ext") != line.npos)
            {
                // stack.push(line);
                continue;
            }

            if (line == L"</s>" || line == L"</a>" || line == L"</t>" || line == L"</p>" || line == L"</caixa>" ||
                line == L"</situacao>" || line == L"</li>" || line == L"</ext>")
            {
                //  stack.pop();
                if (lines.size() > 0)
                {
                    auto corpus_u = foreach_line(lines);
                    _corpus.push_back(corpus_u);
                    lines.clear();
                }
                continue;
            }
            if (line.size() > 0)
                lines.push_back(line);
        }
        return true;
    }

    ~decoder()
    {
        if (_fstream.is_open())
        {
            _fstream.close();
        }
    }

  private:
    std::wifstream _fstream;
    std::vector<std::shared_ptr<corpus_unit>> _corpus;
};
}; // namespace visl_cg
#endif