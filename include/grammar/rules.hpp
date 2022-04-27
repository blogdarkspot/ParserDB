#ifndef __RULES__
#define __RULES__
#include <memory>
#include <ostream>

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
    unsigned int total = 0;
    unsigned int times = 0;

    ProbabilisticRule(_StringT left, _ContainerT<_StringT> right, bool is_terminal)
        : rule<_StringT, _ContainerT>(left, right, is_terminal)
    {
    }

    friend std::wostream &operator<<(std::wostream &os, const ProbabilisticRule &r)
    {
        os << static_cast<rule<_StringT, _ContainerT>>(r);
        return os;
    }
};

}; // namespace grammar::cfg
#endif