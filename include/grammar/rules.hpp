#ifndef __RULES__
#define __RULES__
#include <memory>
#include <ostream>
/*
2 APPENDIX C • STATISTICAL CONSTITUENCY PARSING
N a set of non-terminal symbols (or variables)
Σ a set of terminal symbols (disjoint from N)
R a set of rules or productions, each of the form A → β [p],
where A is a non-terminal,
β is a string of symbols from the infinite set of strings (Σ ∪ N)∗,
and p is a number between 0 and 1 expressing P(β |A)
S a designated start symbol
That is, a PCFG differs from a standard CFG by augmenting each rule in R with
a conditional probability:
A → β [p] (C.1)
Here p expresses the probability that the given non-terminal A will be expanded
to the sequence β . That is, p is the conditional probability of a given expansion β
given the left-hand-side (LHS) non-terminal A. We can represent this probability as
P(A → β )
or as
P(A → β |A)
or as
P(RHS|LHS)
Thus, if we consider all the possible expansions of a non-terminal, the sum of their
probabilities must be 1: ∑
β
P(A → β ) = 1
*/
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

}; // namespace grammar::cfg
#endif