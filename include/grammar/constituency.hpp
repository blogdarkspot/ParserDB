#ifndef __UNIT__
#define __UNIT__

#include <memory>
#include <utility>

namespace grammar::cfg
{

template<typename _StringType>
struct symbol
{
    _StringType value;
    double probability;
    bool is_terminal;
    std::shared_ptr<symbol<_StringType>> left = nullptr;
    std::shared_ptr<symbol<_StringType>> right = nullptr;
};

template<typename _StringType>
struct lexicon : symbol<_StringType>
{
    _StringType lex;
};

template<typename _StringType>
struct constituency : public symbol<_StringType>
{

    void set_left(std::shared_ptr<constituency<_StringType>> l)
    {
        symbol<_StringType>::left = l;
    }

    void set_right(std::shared_ptr<constituency<_StringType>> r)
    {
        symbol<_StringType>::right = r;
    }

    /*
    bool operator< (const constituency<_StringType>& rhs)
    {
        return left < rhs.left && right < rhs.right && symbol::value != rhs.value;
    }*/
};
};

/*
template <typename _Ty, template <class...> class _ContainerTy> struct unit
{
    using unit_ptr = std::shared_ptr<unit<_Ty, _ContainerTy>>;

    

    unit &operator+=(const std::shared_ptr<rule_description<_Ty>> &value)
    {
        no_terminals.insert(value);
        return *this;
    }

    unit &operator=(const std::shared_ptr<rule_description<_Ty>> &value)
    {
        no_terminals.clear();
        no_terminals.insert(value);
        return *this;
    }

    unit &operator=(const unit<_Ty, _ContainerTy> rhs)
    {
        this->no_terminals = rhs.no_terminals;
        return *this;
    }

    _ContainerTy<std::shared_ptr<rule_description<_Ty>>> get()
    {
        return no_terminals;
    }

    friend std::wostream &operator<<(std::wostream &os, const unit<_Ty, _ContainerTy> &r)
    {
        if (!r.no_terminals.empty())
        {
            for (const auto &value : r.no_terminals)
            {
                os << value << L",";
            }
        }
        else
        {
            os << L"X";
        }

        return os;
    }
  private:
    _ContainerTy<std::shared_ptr<rule_description<_Ty>>> no_terminals;
};*/
#endif