#ifndef __DELAS_TYPE__
#define __DELAS_TYPE__

#include <string>
#include <vector>

namespace lexicon::delas {

struct DelasType 
{
    DelasType(const std::wstring &word, const std::wstring &canonical, const std::wstring &wclass,
               const std::wstring &trace, const std::vector<std::wstring> &flexion)
        : _word(std::move(word)), _canonical(std::move(canonical)), _wclass(std::move(wclass)),
          _trace(std::move(trace)), _flexion(std::move(flexion))
    {
    }
    const std::wstring _word;
    const std::wstring _canonical;
    const std::wstring _wclass;
    const std::wstring _trace;
    const std::vector<std::wstring> _flexion;
};

};
#endif