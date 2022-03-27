#ifndef __DICTIONARY__
#define __DICTIONARY__

#include "lexico.hpp"
#include <iostream>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

using namespace std;

namespace dictionary
{

template <typename _Key> class dictionary
{

    struct storage
    {
        std::shared_ptr<lexico> data;
        std::size_t canonical;
        std::size_t word;
    };

  public:
    void add(_Key word, _Key canonical, const std::shared_ptr<lexico> desc)
    {
        auto sto = std::make_shared<storage>();
        sto->data = desc;

        auto hashWord = hash(word);
        auto hashCanonical = hash(canonical);

        auto iWord = _library_by_word.find(hashWord);
        auto iCanonical = _library_by_canonical.find(hashCanonical);

        if (iWord == _library_by_word.end())
        {
            _word_desc.emplace_back(word);
            sto->word = _word_desc.size() - 1;
        }
        else
        {
            sto->word = iWord->second[0]->word;
        }

        if (iCanonical == _library_by_canonical.end())
        {
            _canonical_desc.emplace_back(canonical);
            sto->canonical = _canonical_desc.size() - 1;
        }
        else
        {
            sto->canonical = iCanonical->second[0]->canonical;
        }

        _library_by_word[hashWord].emplace_back(sto);
        _library_by_canonical[hashCanonical].emplace_back(sto);
    }

    bool remove(_Key word)
    {
    }

    void split_contraction_word(_Key word)
    {
        auto hashWord = hash(word);
        const auto canonicalWord = _library_by_word.find(hashWord);
        if (canonicalWord != _library_by_word.end())
        {
            if (canonicalWord->second[0]->canonical < _canonical_desc.size())
            {
                const auto &canonicalDesc = _canonical_desc[canonicalWord->second[0]->canonical];
                std::wcout << canonicalDesc << std::endl;
            }
        }
    }

    vector<std::shared_ptr<lexico>> search_by_word(_Key word)
    {

        auto hashWord = hash(word);
        vector<std::shared_ptr<lexico>> ret;
        for (const auto &lex : _library_by_word[hashWord])
        {
            ret.emplace_back(lex->data);
        }
        return ret;
    }

    vector<std::shared_ptr<lexico>> search_by_canonical(_Key canonical)
    {
        auto hashCanonical = hash(canonical);
        vector<std::shared_ptr<lexico>> ret;
        for (const auto &lex : _library_by_canonical[hashCanonical])
        {
            ret.emplace_back(lex->data);
        }
        return ret;
    }

    const _Key get_canonical(_Key word)
    {

        auto iword = _library_by_word.find(word);

        if (iword == _library_by_word.end())
        {
            return _Key();
        }

        return _canonical_desc[iword->canonical];
    }

    const std::vector<_Key> get_flexion_from_canonical(_Key canonical)
    {
        auto hashCanonical = hash(canonical);
        const auto &iCanonical = _library_by_canonical.find(hashCanonical);
        std::vector<_Key> ret;
        for (const auto &value : iCanonical->second)
        {
            ret.emplace_back(_word_desc[value->word]);
        }
        return ret;
    }

  private:
    std::size_t hash(const _Key &value)
    {
        return std::hash<_Key>{}(value);
    }

    unordered_map<std::size_t, vector<std::shared_ptr<storage>>> _library_by_word;
    unordered_map<std::size_t, vector<std::shared_ptr<storage>>> _library_by_canonical;
    vector<_Key> _canonical_desc;
    vector<_Key> _word_desc;
};
} // namespace dictionary
#endif