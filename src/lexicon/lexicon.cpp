#include "lexicon.h"
#include "delas/delas_file.h"
#include "delas/contractions_file.h"
#include "delas/delas_parser.h"
#include "core/database.hpp"
#include <memory>

namespace lexicon
{

struct lexicon::_impl_lexicon
{
    _impl_lexicon()
        : _delas_file(new ::lexicon::delas::DelasFile()), _contractions(new ::lexicon::delas::ContractionsFile()),
          _db(new ::lexicon::database<std::wstring>())
    {
    }

    bool load()
    {
        bool ret = true;
        ret = _delas_file->open();

        if(ret)
        {
            std::cout << "file delaf open\n";
           ret = _contractions->open(); 
        }

        if(ret)
        {
            std::cout << "file contracoes open\n";
            while(!_delas_file->eof())
            {
                auto word = _delas_file->get_next();
                if(word == nullptr)
                {
                    continue;
                }
                auto lexico = delas::DelasToLexicon::parser_word(*word);
                _db->add(std::make_shared<LexiconType<std::wstring>>(lexico));
                _is_delaf_loading = true;
            }

            while(auto contraction = _contractions->get_next())
            {
                _db->add_contraction(contraction->_word, contraction->_prefix_word, contraction->_prefix_class, 
                                    contraction->_suffix_word, contraction->_suffix_class);
                _is_contractions_loading = true;
            }
        }
        return ret;
    }

    vector<std::shared_ptr<LexiconType<std::size_t>>> get_word_info(const std::wstring& entry)
    {
        return _db->get_lex_unities_by_word(entry);
    }

    vector<std::shared_ptr<LexiconType<std::size_t>>> get_lemma_flexions(const std::wstring& lemma)
    {
        return _db->get_lex_unities_by_canonical(lemma);
    }

    std::wstring get_word_desc(std::shared_ptr<LexiconType<std::size_t>> entry)
    {
        return _db->get_desc_word(entry);
    }

    std::wstring get_lemma_desc(std::shared_ptr<LexiconType<std::size_t>> lemma)
    {
        return _db->get_desc_lemma(lemma);
    }

    void file(const string &path)
    {
        _delas_file->set_path(path);
    }

    void set_contractions_path(const string &path)
    {
        _contractions->set_path(path);
    }

    vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>> split_contraction(std::wstring word) 
    {
        return _db->split_contraction_word(word);
    }

    DelafInfo get_delaf_info()
    {
        DelafInfo ret;
        ret.is_delaf_loading = _is_delaf_loading;
        ret.is_contractions_loading = _is_contractions_loading;
        ret.total_entry = _db->get_total_entry();
        ret.total_lemma = _db->get_total_lemma();
        ret.total_contractions = _db->get_total_contractions();
        ret.total_lexicon = _db->get_total_lexicon();
        return ret;
    }
    private:
    std::shared_ptr<::lexicon::delas::DelasFile> _delas_file;
    std::shared_ptr<::lexicon::delas::ContractionsFile> _contractions;
    std::shared_ptr<::lexicon::database<std::wstring>> _db;
    bool _is_delaf_loading = false;
    bool _is_contractions_loading = false;
};

lexicon::lexicon() : _ptr(new _impl_lexicon())
{
}

vector<std::shared_ptr<LexiconType<std::size_t>>> lexicon::get_word_info(const std::wstring& entry)
{
    return _ptr->get_word_info(entry);
}

vector<std::shared_ptr<LexiconType<std::size_t>>> lexicon::get_lemma_flexions(const std::wstring& lemma)
{
    return _ptr->get_lemma_flexions(lemma);
}

std::wstring lexicon::get_word_desc(std::shared_ptr<LexiconType<std::size_t>> entry)
{
    return _ptr->get_word_desc(entry);
}

std::wstring lexicon::get_lemma_desc(std::shared_ptr<LexiconType<std::size_t>> lemma)
{
    return _ptr->get_lemma_desc(lemma);
}

void lexicon::file(const string &path)
{
    _ptr->file(path);
}

void lexicon::set_contractions_path(const string &path) {
    _ptr->set_contractions_path(path);
}

bool lexicon::load()
{
    return _ptr->load();
}

vector<pair<pair<wstring, wstring>, pair<wstring, wstring>>> lexicon::split_contraction(std::wstring word) 
{
    return _ptr->split_contraction(word);
}
DelafInfo lexicon::get_delaf_info()
{
    return _ptr->get_delaf_info();
}

}; // namespace lexicon