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
#include <set>

namespace corpus::folha
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
    corpus_unit() 
    {
    }

    void add_lex_unit_ceten(std::shared_ptr<lex_unit_ceten> unit)
    {
        _lexicon.emplace_back(unit);
    }

    ~corpus_unit()
    {
    }

    std::vector<std::shared_ptr<lex_unit_ceten>> _lexicon;
};

static std::set<std::wstring> WORD_TAGS = { L"N", L"PROP", L"SPEC", L"DET", L"PERS", L"ADJ", L"ADV", L"V", L"NUM", L"PRON", 
                                            L"PRP", L"KS", L"KC", L"IN", L"EC" };
static std::set<std::wstring> SECUNDARY_INFO = { L"<artd>", L"<arti>", L"<quant>", L"<dem>", L"<poss>", L"<refl>", L"<diff>", L"<ident>",
    L"<rel>", L"<interr>" };
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

    void tokenize(std::wstring const &str, const std::wstring &delim, std::vector<std::wstring> &out)
    {
        std::wstring tmp = str;
        size_t pos = 0;
        while ((pos = tmp.find_first_of(L" \t")) != std::wstring::npos)
        {
            auto token = tmp.substr(0, pos);
            if(token.size() == 0)
            {
                tmp.erase(0, pos + delim.length());
                continue;
            }


           // std::transform(token.begin(), token.end(), token.begin(), std::towupper);
            out.emplace_back(token);
            tmp.erase(0, pos + delim.length());
        }
        if(tmp.size() > 0)
        {
            out.emplace_back(tmp);
        }
    }


    std::shared_ptr<lex_unit_ceten> parser_line(std::wstring line)
    {
        auto ret = std::make_shared<lex_unit_ceten>();

        std::vector<std::wstring> tokens;
        tokenize(line, L" ", tokens);
        size_t tot = tokens.size();
        if (tokens.size() > 0 && tokens[tokens.size() - 1][0] == L'#')
        {
            if(tokens[0][0] == L'$') {
                ret->category = L"$";
                return ret;
            }

            ret->verbete = tokens[0];
            if(tokens[0].empty() || tokens[0] == L"")
            {
                std::cout << "" << std::endl;
            }

            auto lemmaPos = 0;
            while(lemmaPos < tokens[1].size()  && (tokens[1][lemmaPos] == L' ' || tokens[1][lemmaPos] == L'\t'))
            {
                ++lemmaPos;
            }
            if(lemmaPos < tokens[1].size())
            {
                ret->lemma = tokens[1].substr(lemmaPos);
            }

            for(int i = 2; i < tokens.size(); ++i)
            {
                if(SECUNDARY_INFO.find(tokens[i]) != SECUNDARY_INFO.end())
                {
                    ret->secundary_information.emplace_back(tokens[i]);
                    continue;
                }

                if(WORD_TAGS.find(tokens[i]) != WORD_TAGS.end())
                {
                    ret->category = tokens[i];
                }
            }
            if (ret->category == L"")
            {
                return nullptr;
            }
        }
        else
        {
            return nullptr;
        }

        return ret;
    }

    std::shared_ptr<corpus_unit> foreach_line(const std::vector<std::wstring> &lines)
    {
        auto ret = std::make_shared<corpus_unit>();
        for (const auto &line : lines)
        {
            auto res = parser_line(line);
            if(res == nullptr)
            {
                return nullptr;
            }
            if(res->category == L"$")
            {
                continue;
            }
            ret->add_lex_unit_ceten(res);
        }
        return ret;
    }

    bool load_file(std::string path)
    {

        _fstream.open(path);
        std::locale utf8{"en_US.UTF-8"};
        _fstream.imbue(utf8);
        _debug.open("debug.txt");
        _debug.imbue(utf8);
        return _fstream.is_open() && _fstream.good() && _debug.is_open() && _debug.good();
    };

    bool parser_file()
    {
        std::stack<std::wstring> stack;
        auto ret = true;
        std::wstring line;
        bool start = false;
        std::vector<std::wstring> lines;
        int next_line = 1;
        std::wstring token_id = L"<ext id=";
        int tot = 10000;
        while (tot >= 0 && ret && std::getline(_fstream, line))
        {
        
            if(line.find(L"#" + std::to_wstring(next_line))  != line.npos)
            {
                lines.emplace_back(line);
                ++next_line;
                continue;
            }
            else {
                next_line = 1;
                if (lines.size() > 0)
                {
                    auto corpus_u = foreach_line(lines);
                    if(corpus_u != nullptr)
                        _corpus.push_back(corpus_u);
                    lines.clear();
                    --tot;
                }
            }
        }
        return true;
    }


    std::vector<std::shared_ptr<corpus_unit>> get_corpus()
    {
        return _corpus;
    };

    ~decoder()
    {
        if (_fstream.is_open())
        {
            _fstream.close();
        }
    }

  private:
    std::wifstream _fstream;
    std::wofstream _debug;
    std::vector<std::shared_ptr<corpus_unit>> _corpus;
};
}; // namespace visl_cg
#endif