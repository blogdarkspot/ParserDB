#include "lexicon.h"

namespace lexicon
{

struct lexicon::_impl_lexicon
{
    bool load()
    {
        return true;
    }

    void file(const string &path, const string &encode, const string &format)
    {

    }
    private:

};

void lexicon::file(const string &path, const string &encode, const string &format)
{
    _ptr->file(path, encode, format);
}

bool lexicon::load()
{
    return _ptr->load();
}
}; // namespace lexicon