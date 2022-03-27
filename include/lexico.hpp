#ifndef __LEXICO__
#define __LEXICO__

#include "string_def.hpp"
#include <unordered_set>
#include <vector>

struct lexico
{
    std::unordered_set<string>::const_iterator categoria;
    std::unordered_set<string>::const_iterator tipo;
    std::vector<std::unordered_set<string>::const_iterator> traco;
    std::unordered_set<string>::const_iterator forma;
    std::unordered_set<string>::const_iterator tempo;
    std::unordered_set<string>::const_iterator prefix;
    std::unordered_set<string>::const_iterator suffix;
};
#endif