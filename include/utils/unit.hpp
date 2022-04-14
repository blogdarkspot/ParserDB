#ifndef __UNIT__
#define __UNIT__

#include <memory>

template <typename _Ty, template <class...> class _ContainerTy> struct unit
{
    using unit_ptr = std::shared_ptr<unit<_Ty, _ContainerTy>>;

    unit &operator+=(const _Ty &value)
    {
        no_terminals.insert(value);
        return *this;
    }

    unit &operator=(const _Ty &value)
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

    _ContainerTy<_Ty> get()
    {
        return no_terminals;
    }

  private:
    _ContainerTy<_Ty> no_terminals;
};
#endif