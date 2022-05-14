#ifndef __LEXICON_LEXICON__
#define __LEXICON_LEXICON__

#include <memory>
#include <string>
#include "core/types.h"
#include <vector>

namespace lexicon
{

using namespace std;

struct DelafInfo
{
    int total_entry;
    int total_lemma;
    int total_contractions;
    int total_lexicon;
    bool is_delaf_loading;
    bool is_contractions_loading;
};

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
    void file(const string &path);

    void set_contractions_path(const string &path);

    /**
     * load de lexicon in memory using the file was pass in file(string, string, string)
     * return true if load was a success, false otherwise
     */
    bool load();

    vector<std::shared_ptr<LexiconType<std::size_t>>> get_word_info(const std::wstring &entry);

    vector<std::shared_ptr<LexiconType<std::size_t>>> get_lemma_flexions(const std::wstring &lemma);

    std::wstring get_word_desc(std::shared_ptr<LexiconType<std::size_t>> entry);

    std::wstring get_lemma_desc(std::shared_ptr<LexiconType<std::size_t>> lemma);

    vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>> split_contraction(std::wstring);

    DelafInfo get_delaf_info();

  private:
    
    std::shared_ptr<_impl_lexicon> _ptr;
};
}; // namespace lexicon
#endif