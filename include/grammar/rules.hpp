#ifndef __RULES__
#define __RULES__
#include <memory>
#include <ostream>
#include <map>
#include <assert.h>
#include <vector>
#include <string>

namespace grammar::cfg
{

class rule : public std::enable_shared_from_this<rule>
{
  public:
    using rule_sptr = std::shared_ptr<rule>;

    rule(std::wstring left, std::vector<std::wstring> right, bool is_terminal)
        : _left_side(left), _right_side(right), _is_terminal(is_terminal)
    {
    }

    std::shared_ptr<rule> get_ptr()
    {
        return this->shared_from_this();
    }

    rule(std::vector<std::wstring> right)
    {
    }

    std::wstring get_left_side()
    {
        return _left_side;
    }

    std::vector<std::wstring> get_right_side()
    
    {
        return _right_side;
    }

    bool is_terminal()
    {
        return _is_terminal;
    }

    bool is_valid(std::vector<std::wstring> right)
    {
        bool ret = true;
        for (int i = 0; i < right.size(); ++i)
        {
            if (_right_side[i] != right[i])
            {
                ret = false;
                break;
            }
        }
        return ret;
    }
    ~rule()
    {
    }

    friend std::wostream &operator<<(std::wostream &os, const rule &r)
    {
        os << r._left_side << " -> ";
        unsigned int i = 0;
        for (const auto &right : r._right_side)
        {
            os << right << " ";
        }
        return os;
    }

  private:
    std::wstring _left_side;
    std::vector<std::wstring> _right_side;
    int _role_size;
    bool _is_terminal;
};


struct ProbabilisticRule : public rule
{


    ProbabilisticRule(std::wstring left, std::vector<std::wstring> right, bool is_terminal)
        : rule(left, right, is_terminal)
    {
    }

    friend std::wostream &operator<<(std::wostream &os, const ProbabilisticRule &r)
    {
        auto p = r.get_probability();
        os << L"probability : " << p << L" " << (rule)(r);
        return os;
    }


    void increment_count()
    {
        assert(tot_lhs != nullptr);
        *tot_lhs += 1;
        ++count;
    }

    const double get_probability() const
    {
        if(*tot_lhs == 0)
        {
            return 1.0 / tot_lhs.use_count();
        }

        return  (count * 1.0) / *tot_lhs;
    }

    void set_count_lhs(std::shared_ptr<size_t> p)
    {
        tot_lhs = p;
    }

    std::shared_ptr<size_t> get_count_lhs()
    {
        return tot_lhs;
    }

    private:

    std::shared_ptr<size_t> tot_lhs = nullptr;
    size_t count = 0;
};

}; // namespace grammar::cfg
#endif