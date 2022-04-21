#ifndef __INPUT_RULES__
#define __INPUT_RULES__

#include "rules.hpp"
#include <memory>
#include <string>
#include <vector>

namespace grammar::cfg
{

using container_rules_utf8 = std::vector<std::shared_ptr<rule<std::wstring, std::vector>>>;

class file
{
  public:
    file() = default;

    // A -> B C "description"

    std::shared_ptr<ProbabilisticRule<std::wstring, std::vector>> parser(const std::wstring &rule)
    {
        return _parser(rule);
    }

    container_rules_utf8 parser(const std::vector<std::wstring> &rules, bool &error)
    {
        
        container_rules_utf8 ret;
/*
        for (int i = 0; i < rules.size(); ++i)
        {
            auto rule = _parser(rules[i]);
            if(rule == nullptr)
            {
                ret.clear();
                error = true;
                break;
            }
            ret.emplace_back(rule);
        }
        */
        return ret;
    }

  private:
    std::shared_ptr<grammar::cfg::ProbabilisticRule<std::wstring, std::vector>> _parser(const std::wstring &rule)
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

        return std::make_shared<::grammar::cfg::ProbabilisticRule<std::wstring, std::vector>>(left, right, false);
    }
};
}; // namespace grammar::cfg
#endif