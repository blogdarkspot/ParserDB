#ifndef __LEXICON_TYPES__
#define __LEXICON_TYPES__

#include<memory>
#include<vector>
#include<unordered_set>
#include<string>

namespace lexicon {

struct Category {
  std::unordered_set<std::wstring>::const_iterator categoria;
  std::unordered_set<std::wstring>::const_iterator tipo;
  std::vector<std::unordered_set<std::wstring>::const_iterator> traco;
  std::unordered_set<std::wstring>::const_iterator forma;
  std::unordered_set<std::wstring>::const_iterator tempo;
  std::unordered_set<std::wstring>::const_iterator prefix;
  std::unordered_set<std::wstring>::const_iterator suffix;
};

template<typename T>
struct LexiconType {

  LexiconType() : category(new Category())
  {

  }

  T entry;
  T lemma;
  size_t calls = 0;
  std::shared_ptr<size_t> total;
  double p = 1.0;
  std::shared_ptr<Category>  category;
};

}; // namespace lexicon
#endif