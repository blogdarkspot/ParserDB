#ifndef __CONTRACTIONS_FILE__
#define __CONTRACTIONS_FILE__

#include <string>

namespace lexicon::delas

{
using namespace std;

class ContractionsFile
{
  public:
    ContractionsFile(const string &path, string encode = "utf-8");
    //std::shared_ptr<DelasType> get_next();
    //const std::vector<std::shared_ptr<DelasType>> get_all();

};
} // namespace lexicon::delas
#endif