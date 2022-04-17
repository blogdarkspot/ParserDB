#ifndef __LEXICON_LEXICON__
#define __LEXICON_LEXICON__

#include <memory>
#include <string>

namespace lexicon
{

    using namespace std;

    class lexicon
    {
        struct _impl_lexicon;
        public:
        lexicon();
        ~lexicon() = default;

        /**
        * path = full file path to file
        * encode = enconde type ASCII or UTF-8 <default UTF-8>
        * format = format of file  Ceten or Delas
        */
        void file(const string& path, const string& encode = "utf-8", const string& format = "Delas");

        /**
        * load de lexicon in memory using the file was pass in file(string, string, string)
        * return true if load was a success, false otherwise
        */
        bool load();

        private:
        std::shared_ptr<_impl_lexicon> _ptr;
    };
};
#endif