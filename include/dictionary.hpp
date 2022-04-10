#ifndef __DICTIONARY__
#define __DICTIONARY__

#include "lexico.hpp"
#include <iostream>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace dictionary
{

template <typename _Key> class dictionary
{

    struct lex_unit
    {
        std::shared_ptr<lexico> data;
        std::size_t canonical;
        std::size_t word;
    };

  public:
    void add_contraction(wstring key, wstring prefix, wstring class_prefix, wstring suffix, wstring class_sufix)
    {
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

    void add(_Key word, _Key canonical, const std::shared_ptr<lexico> desc)
    {
        auto sto = std::make_shared<lex_unit>();
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

    vector<std::shared_ptr<lex_unit>> get_lex_unities_by_word(_Key word)
    {

        auto hashWord = hash(word);
        vector<std::shared_ptr<lex_unit>> ret;
        for (const auto &lex : _library_by_word[hashWord])
        {
            ret.emplace_back(lex);
        }
        return ret;
    }

    vector<std::shared_ptr<lex_unit>> get_lex_unities_by_canonical(_Key canonical)
    {
        auto hashCanonical = hash(canonical);
        vector<std::shared_ptr<lex_unit>> ret;
        for (const auto &lex : _library_by_canonical[hashCanonical])
        {
            ret.emplace_back(lex);
        }
        return ret;
    }

    wstring get_desc_word(std::shared_ptr<lex_unit> unit)
    {
        if (unit->word < _word_desc.size())
        {
            return _word_desc[unit->word];
        }
        return L"";
    }

    wstring get_desc_lemma(std::shared_ptr<lex_unit> unit)
    {
        if (unit->canonical < _canonical_desc.size())
        {
            return _canonical_desc[unit->canonical];
        }
        return L"";
    }

    wstring get_class(std::shared_ptr<lex_unit> unit)
    {
        return L"";
    }

  private:
    std::size_t hash(const _Key &value)
    {
        return std::hash<_Key>{}(value);
    }

    unordered_map<std::size_t, vector<std::shared_ptr<lex_unit>>> _library_by_word;
    unordered_map<std::size_t, vector<std::shared_ptr<lex_unit>>> _library_by_canonical;
    vector<_Key> _canonical_desc;
    vector<_Key> _word_desc;

    std::unordered_set<std::size_t> hash_canonicals;
    std::unordered_set<std::size_t> hash_words;
    std::unordered_map<wstring, vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>>> _contractions;
};
} // namespace dictionary
#endif