#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include <memory>

namespace collections 
{
template <typename _Ty> class binary_tree
{
    binary_tree() = default;

    void set_parent(std::shared_ptr<_Ty> parent)
    {
        _parent = parent;
    }

    void set_left(std::shared_ptr<_Ty> left)
    {
        _left = left;
    }

    void set_right(std::shared_ptr<_Ty> right)
    {
        _right = right;
    }

    std::shared_ptr<_Ty> get_parent()
    {
        return _parent;
    }

    std::shared_ptr<_Ty> get_left()
    {
        return _left;
    }

    std::shared_ptr<_Ty> get_right()
    {
        return _right;
    }
    
    _Ty get_data()
    {
        return _data;
    }

  private:
    std::shared_ptr<binary_tree<_Ty>> _left;
    std::shared_ptr<binary_tree<_Ty>> _right;
    std::shared_ptr<binary_tree<_Ty>> _parent;
    _Ty _data;
};
}
#endif