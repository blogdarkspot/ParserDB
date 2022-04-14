#ifndef __INPUT_RULES__
#define __INPUT_RULES__

#include "parser/rules.hpp"
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
    container_rules_utf8 parser(const std::vector<std::wstring> &rules, bool &error)
    {
        container_rules_utf8 ret;

        for (int i = 0; i < rules.size(); ++i)
        {
            auto pos = rules[i].find_first_of(L"-");

            std::wstring left;
            std::vector<std::wstring> right;

            if (pos == rules[i].npos)
            {
                ret.clear();
                error = true;
                break;
            }
            if (rules[i][pos + 1] != L'>')
            {
                ret.clear();
                error = true;
                break;
            }

            left = rules[i].substr(0, pos - 1);
            pos += 3;

            do
            {
                auto tmp = rules[i].find_first_of(L' ', pos);

                if (tmp == rules[i].npos)
                {
                    right.emplace_back(rules[i].substr(pos));
                    break;
                }
                else
                {
                    auto s = rules[i].substr(pos, tmp - pos);
                    right.emplace_back(s);
                    pos += (tmp - pos) + 1;
                }
            } while (true);

            auto rule = std::make_shared<::grammar::cfg::rule<std::wstring, std::vector>>(left, right, false);
            ret.emplace_back(rule);
        }
        return ret;
    }

  private:
};
}; // namespace grammar::cfg
#endif