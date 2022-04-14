#ifndef __MATRIX__
#define __MATRIX__

#include <memory>

namespace utils::colections
{
template <typename _Ty> class matrix
{

  public:
    matrix(int rows, int cols) : _rows(rows), _cols(cols)
    {
        _data.reset(new std::shared_ptr<_Ty>[rows], [](std::shared_ptr<_Ty> *p) { delete[] p; });
        for (int i = 0; i < rows; ++i)
        {
            _data.get()[i].reset(new _Ty[cols], [](_Ty *p) { delete[] p; });
        }
    }

    _Ty *operator[](const int &index)
    {
        if (_data.get()[index].get() == nullptr)
        {
        }

        return _data.get()[index].get();
    }

    matrix &operator+=(const matrix &other)
    {
        return *this;
    }

    std::shared_ptr<std::shared_ptr<_Ty>> _data;
    std::uint64_t _rows;
    std::uint64_t _cols;
};
} // namespace utils::colections

#endif