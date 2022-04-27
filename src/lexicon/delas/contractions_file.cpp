#include "delas/contractions_file.h"
#include "delas/type.h"
#include <iostream>
#include <string>

namespace lexicon::delas
{

bool ContractionsFile::open()
{
    try
    {
        _contracoes.open(_path, std::wofstream::out);
        std::locale utf8{"en_US.UTF-8"};
        _contracoes.imbue(utf8);
    }
    catch (const std::ios_base::failure &e)
    {
        // TODO: Log error
    }
    return _contracoes.is_open() && _contracoes.good();
}

std::shared_ptr<ContractionType> ContractionsFile::get_next()
{
    std::shared_ptr<ContractionType> ret;
    std::wstring line;
    if(std::getline(_contracoes, line))
    {
        ret = parse_line(line);
    }
    return ret;
}

const std::vector<std::shared_ptr<ContractionType>> ContractionsFile::get_all()
{
        std::vector<std::shared_ptr<ContractionType>> ret;
        std::wstring line;
        while (std::getline(_contracoes, line))
        {
            auto con = parse_line(line);
            if(con == nullptr)
            {
                ret.clear();
                std::cerr << "corrupted contraction file\n";
                break;
            }
            ret.emplace_back(con);
        }
        return ret;
}

std::shared_ptr<ContractionType> ContractionsFile::parse_line(const std::wstring& line)
{
        vector<wstring> split;
        size_t pos = 0;
        do
        {
            auto tmp = line.find_first_of(L" ", pos);
            if (tmp != line.npos)
            {
                split.emplace_back(line.substr(pos, tmp - pos));
            }
            else
            {
                split.emplace_back(line.substr(pos));
                break;
            }
            pos = tmp + 1;
        } while (true);

        if(split.size() != 5)
        {
            return nullptr;
        }
        return std::make_shared<ContractionType>(split[0], split[1], split[2], split[3], split[4]);
}

};