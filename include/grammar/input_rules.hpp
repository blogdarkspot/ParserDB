#ifndef __INPUT_RULES__
#define __INPUT_RULES__

#include "rules.hpp"
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

namespace grammar::cfg
{

using container_rules_utf8 = std::vector<std::shared_ptr<rule>>;

class file
{
  public:
    file() = default;

    file(const std::string &path) : _path(path)
    {
    }

    bool open()
    {
        try
        {
            _fstream.open(_path);
            std::locale utf8{"en_US.UTF-8"};
            _fstream.imbue(utf8);
        }
        catch (const std::ios_base::failure &e)
        {
            // TODO: Log error
        }
        return _fstream.is_open() && _fstream.good(); //&& _contracoes.is_open() && _contracoes.good();
    }

    void set_path(const std::string &path)
    {
      _path = path;
    }
    // A -> B C "description"

    std::vector<std::shared_ptr<ProbabilisticRule>> get_all()
    {
        std::vector<std::shared_ptr<ProbabilisticRule>> ret;
        std::wstring line;
        while (std::getline(_fstream, line))
        {
            std::vector<std::wstring> out;  
            tokenize(line, out);


            std::vector<std::wstring> rhs;

            double probability = 0.0;

            for(int i = 2; i < out.size(); ++i)
            {
                if(out[i] == L"|")
                {
                    if(i + 1 < out.size())
                    {
                        probability = std::stod(out[i+1]);
                        break;
                    }
                }
                rhs.emplace_back(out[i]);
            }
            auto rule = std::make_shared<ProbabilisticRule>(out[0], rhs, false);
            rule->set_probability(probability);
            ret.emplace_back(rule);
            
        }
        return ret;
    }

    void tokenize(std::wstring const &str, std::vector<std::wstring> &out)
    {
        std::wstring tmp = str;
        size_t pos = 0;
        while ((pos = tmp.find(L" ")) != std::wstring::npos)
        {
            auto token = tmp.substr(0, pos);
            out.emplace_back(token);
            tmp.erase(0, pos + std::wstring(L" ").length());
        }
        if (tmp.size() > 0)
        {
            out.emplace_back(tmp);
        }
    }

    std::shared_ptr<ProbabilisticRule> parser(const std::wstring &rule)
    {
        return _parser(rule);
    }

    container_rules_utf8 parser(const std::vector<std::wstring> &rules, bool &error)
    {
        
        container_rules_utf8 ret;
        return ret;
    }

  private:

    std::wifstream _fstream;
    std::string _path;

    std::shared_ptr<grammar::cfg::ProbabilisticRule> _parser(const std::wstring &rule)
    {
        auto pos = rule.find_first_of(L"-");

        std::wstring left;
        std::vector<std::wstring> right;

        if (pos == rule.npos)
        {
            return nullptr;
        }
        if (rule[pos + 1] != L'>')
        {
            return nullptr;
        }

        left = rule.substr(0, pos - 1);
        pos += 3;

        do
        {
            auto tmp = rule.find_first_of(L' ', pos);

            if (tmp == rule.npos)
            {
                right.emplace_back(rule.substr(pos));
                break;
            }
            else
            {
                auto s = rule.substr(pos, tmp - pos);
                right.emplace_back(s);
                pos += (tmp - pos) + 1;
            }
        } while (true);

        return std::make_shared<::grammar::cfg::ProbabilisticRule>(left, right, false);
    }
};
}; // namespace grammar::cfg
#endif