#ifndef __RULES__
#define __RULES__
#include <memory>
#include <ostream>
#include <map>
#include <assert.h>

namespace grammar::cfg
{

template <class _Ty, template <class...> class ContainerT>
class rule : public std::enable_shared_from_this<rule<_Ty, ContainerT>>
{
  public:
    using rule_sptr = std::shared_ptr<rule<_Ty, ContainerT>>;

    rule(_Ty left, ContainerT<_Ty> right, bool is_terminal)
        : _left_side(left), _right_side(right), _is_terminal(is_terminal)
    {
    }

    std::shared_ptr<rule<_Ty, ContainerT>> get_ptr()
    {
        return this->shared_from_this();
    }

    rule(ContainerT<_Ty> right)
    {
    }

    _Ty get_left_side()
    {
        return _left_side;
    }

    ContainerT<_Ty> get_right_side()
    {
        return _right_side;
    }

    bool is_terminal()
    {
        return _is_terminal;
    }

    bool is_valid(ContainerT<_Ty> right)
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

    friend std::wostream &operator<<(std::wostream &os, const rule<_Ty, ContainerT> &r)
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
    _Ty _left_side;
    ContainerT<_Ty> _right_side;
    int _role_size;
    bool _is_terminal;
};


template <class _StringT, template <class...> class _ContainerT>
struct ProbabilisticRule : public rule<_StringT, _ContainerT>
{


    ProbabilisticRule(_StringT left, _ContainerT<_StringT> right, bool is_terminal)
        : rule<_StringT, _ContainerT>(left, right, is_terminal)
    {
    }

    friend std::wostream &operator<<(std::wostream &os, const ProbabilisticRule &r)
    {
        auto p = r.get_probability();
        os << L"probability : " << p << L" " << static_cast<rule<_StringT, _ContainerT>>(r);
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