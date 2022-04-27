#ifndef __DICTIONARY__
#define __DICTIONARY__

#include "types.h"
#include <iostream>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace lexicon
{

template <typename _Key> class database
{

  public:
    void add_contraction(wstring key, wstring prefix, wstring class_prefix, wstring suffix, wstring class_sufix)
    {
        ++_total_contractions;
        auto sufx = make_pair(suffix, class_sufix);
        auto prefx = make_pair(prefix, class_prefix);
        auto tmp = make_pair(prefx, sufx);
        auto it = _contractions.find(key);
        if (it != _contractions.end())
        {
            it->second.push_back(tmp);
        }
        else
        {
            vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>> vec;
            vec.push_back(tmp);
            _contractions[key] = vec;
        }
    }

    void add(const std::shared_ptr<LexiconType<_Key>> lex)
    {
        ++_total_lexicon;
        auto new_value = std::make_shared<LexiconType<std::size_t>>();

        auto hashWord = hash(lex->entry);
        auto hashCanonical = hash(lex->lemma);

        auto iWord = _library_by_word.find(hashWord);
        auto iCanonical = _library_by_canonical.find(hashCanonical);

        new_value->category = lex->category;

        if (iWord == _library_by_word.end())
        {
            _word_desc.emplace_back(lex->entry);
            new_value->entry = _word_desc.size() - 1;
        }
        else
        {
            new_value->entry = iWord->second[0]->entry;
        }

        if (iCanonical == _library_by_canonical.end())
        {
            _canonical_desc.emplace_back(lex->lemma);
            new_value->lemma = _canonical_desc.size() - 1;
        }
        else
        {
            new_value->lemma = iCanonical->second[0]->lemma;
        }

        _library_by_word[hashWord].emplace_back(new_value);
        _library_by_canonical[hashCanonical].emplace_back(new_value);
    }
    vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>> split_contraction_word(_Key word)
    {
        auto it = _contractions.find(word);
        if (it != _contractions.end())
        {
            return it->second;
        }
        else
        {
            return {};
        }
    }

    vector<std::shared_ptr<LexiconType<std::size_t>>> get_lex_unities_by_word(_Key word)
    {

        auto hashWord = hash(word);
        vector<std::shared_ptr<LexiconType<std::size_t>>> ret;
        for (const auto &lex : _library_by_word[hashWord])
        {
            ret.emplace_back(lex);
        }
        return ret;
    }

    void calculate_probability(std::shared_ptr<LexiconType<std::size_t>> value)
    {
        auto word = get_desc_word(value);
        auto hashWord = hash(word);
        auto lexicons = _library_by_word[hashWord];
        ++value->calls;
        size_t tot = 0;

        for(auto &val : lexicons)
        {
            tot += val->calls;
        }

        for(auto &val : lexicons)
        {
            if(tot != 0 && val->calls != 0)
            {
                val->p = double (val->calls / tot);
            }
        }

    }

    vector<std::shared_ptr<LexiconType<std::size_t>>> get_lex_unities_by_canonical(_Key canonical)
    {
        auto hashCanonical = hash(canonical);
        vector<std::shared_ptr<LexiconType<std::size_t>>> ret;
        for (const auto &lex : _library_by_canonical[hashCanonical])
        {
            ret.emplace_back(lex);
        }
        return ret;
    }

    wstring get_desc_word(std::shared_ptr<LexiconType<std::size_t>> unit)
    {
        if (unit->entry < _word_desc.size())
        {
            return _word_desc[unit->entry];
        }
        return L"";
    }

    wstring get_desc_lemma(std::shared_ptr<LexiconType<std::size_t>> unit)
    {
        if (unit->entry < _canonical_desc.size())
        {
            return _canonical_desc[unit->entry];
        }
        return L"";
    }

    size_t get_total_entry ()
    {
        return _word_desc.size();
    }

    size_t get_total_lemma()
    {
        return _canonical_desc.size();
    }

    size_t get_total_contractions()
    {
        return _total_contractions;
    }

    size_t get_total_lexicon()
    {
        return _total_lexicon;
    }

    void clear_db()
    {
        _library_by_canonical.clear();
        _library_by_word.clear();
        _canonical_desc.clear();
        _word_desc.clear();
        hash_canonicals.clear();
        hash_words.clear();
        _contractions.clear();
        _total_lexicon = 0;
        _total_contractions = 0;
    }

  private:
    std::size_t hash(const _Key &value)
    {
        return std::hash<_Key>{}(value);
    }

    unordered_map<std::size_t, vector<std::shared_ptr<LexiconType<std::size_t>>>> _library_by_word;
    unordered_map<std::size_t, vector<std::shared_ptr<LexiconType<std::size_t>>>> _library_by_canonical;
    vector<_Key> _canonical_desc;
    vector<_Key> _word_desc;

    std::unordered_set<std::size_t> hash_canonicals;
    std::unordered_set<std::size_t> hash_words;
    std::unordered_map<wstring, vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>>> _contractions;
    size_t _total_lexicon = 0;
    size_t _total_contractions = 0;
};
} // namespace dictionary
#endif