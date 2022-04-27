#ifndef __CONTRACTIONS_FILE__
#define __CONTRACTIONS_FILE__

#include <string>
#include <fstream>
#include <memory>
#include "delas/type.h"

namespace lexicon::delas

{
using namespace std;


class ContractionsFile
{
  public:
    ContractionsFile() = default;
    ContractionsFile(const string &path, string encode = "utf-8");

    bool open();
    std::shared_ptr<ContractionType> get_next();
    const std::vector<std::shared_ptr<ContractionType>> get_all();

    void set_path(const std::string& path)
    {
      _path = path;
    }

  
    private:
    std::shared_ptr<ContractionType> parse_line(const std::wstring& line);

    std::string _path;
    std::wifstream _contracoes;

};
} // namespace lexicon::delas
#endif