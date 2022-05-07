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
};
};
#endif