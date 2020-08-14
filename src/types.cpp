#include "redhand/types.hpp"
#include "redhand/helper.hpp"

bool redhand::engine_config::versionIsGreaterThan(std::string version){
    auto strs = helper::split_string(version, '.');
    if (strs.size() != 3){
        return false;
    }

    auto maj = std::stoi(strs[0]);
    auto min = std::stoi(strs[1]);
    auto pat = std::stoi(strs[2]);

    if(maj < 0 || min < 0 || pat < 0){
        return false;
    }

    return versionIsGreaterThan(maj,min,pat);
}
