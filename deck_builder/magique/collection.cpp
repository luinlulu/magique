//
// Created by Don Goodman-Wilson on 13/11/2017.
//

#include "collection.h"
#include <fstream>
#include <sstream>

namespace magique
{

collection::collection(std::string filename, const catalog &catalog) : catalog_{catalog}
{
    //load up from a CSV
    std::ifstream file{filename};

    std::string row;
    getline(file, row); //throw away header row;
    while (getline(file, row))
    {
        std::istringstream tokenS(row);
        std::string count_str;
        uint16_t count;
        std::string name;
        getline(tokenS, count_str, ',');
        count = stoul(count_str);
        getline(tokenS, name, '"'); //clear first " char
        getline(tokenS, name, '"');
//        name = name.substr(1,name.length()-1);

        bool is_land = false;
//        std::cout << name << std::endl;
        for (const auto &type : catalog.at(name).types)
        {
            if (type == card::type::land || type == card::type::basic_land)
            {
                is_land = true;
                break;
            }
        }
        if (!is_land)
        {
            while (count)
            {
                cards_.push_back(name);
                --count;
            }
        }
    }


}

card collection::at(uint64_t loc) const
{
//    std::cout << (int)loc << " " << cards_.size() << " " << cards_[loc] << std::endl;
    return catalog_.at(cards_[loc]);
}

uint64_t collection::index_at(std::string name, int64_t index_past) const
{
    int64_t i{0};
    for(i = 0 ; i < cards_.size(); ++i)
    {
        if ( (cards_[i] == name) && (i > index_past) )
            return i;
    }

    std::out_of_range e{"No card " + name};
    throw(e);
}

uint64_t collection::count()
{
    return cards_.size();
}

} // namespace magique