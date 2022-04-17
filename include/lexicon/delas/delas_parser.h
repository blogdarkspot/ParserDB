#ifndef __DELAS_PARSER__
#define __DELAS_PARSER__

#include "core/types.h"
#include "type.h"

namespace lexicon::delas
{
class DelasToLexicon
{
    using _LexiconType = lexicon::LexiconType<std::wstring>;

  public:
    static _LexiconType parser_word(const DelasType &word);

  private:
    static void fillNum(_LexiconType &lex, const DelasType &word);
    static void fillArt(_LexiconType &lex, const DelasType &word);
    static void fillV(_LexiconType &lex, const DelasType &word);
    static void fillN(_LexiconType &lex, const DelasType &word);
    static void fillA(_LexiconType &lex, const DelasType &word);
    static void fillABREV(_LexiconType &lex, const DelasType &word);
    static void fillPRO(_LexiconType &lex, const DelasType &word);
};

}; // namespace lexicon::delas
#endif