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

bool redhand::engine_config::versionIsGreaterThan(unsigned int major, unsigned int minor, unsigned int patch){
    if (redhand_version_major > major){
        return true;
    }

    if(redhand_version_major == major && redhand_version_minor > minor){
        return true;
    }

    if(redhand_version_major == major && redhand_version_minor == minor && redhand_version_patch >= patch){
        return true;
    }

    return false;

}

bool redhand::engine_config::doesMatchVersion(unsigned int major, unsigned int minor, unsigned int patch){
    return major == redhand_version_major && minor == redhand_version_minor && patch == redhand_version_patch;
}

bool redhand::game_object_properties::check() {
    std::shared_mutex errorMutex;
    bool Error = false;
    auto prop = *this;

    if (prop.triangle_indices.size() == 0) {
        std::cerr << "ERROR::REDHAND::GAME_OBJECT::NO_TRIANGLES" << std::endl;
        Error = true;
    }

    auto less = std::async(std::launch::async, [&]() {
        for (unsigned int i = 0; i < prop.points_coordinates.size(); i++) {
            if (prop.points_coordinates.at(i).x < 0.0f || prop.points_coordinates.at(i).y < 0.0f) {
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                auto lock = std::scoped_lock(errorMutex);
                Error = true;
                break;
            }

            auto lock = std::shared_lock(errorMutex);
            if (Error) {
                break;
            };
        }
    });

    auto more = std::async(std::launch::async, [&]() {
        for (unsigned int i = 0; i < prop.points_coordinates.size(); i++) {
            if (prop.points_coordinates.at(i).x > 1.0f || prop.points_coordinates.at(i).y > 1.0f) {
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                auto lock = std::scoped_lock(errorMutex);
                Error = true;
                break;
            }

            auto lock = std::shared_lock(errorMutex);
            if (Error) {
                break;
            };
        }
    });

    less.wait();
    more.wait();

    return Error;
}

std::vector<float> redhand::game_object_properties::getData(){
    std::vector<float> data;
    auto object_properties = *this;

    //Reserve the needed size in the vector
    data.reserve(
        object_properties.points_coordinates.size() * 3   //size of points
        + object_properties.points_coordinates.size() * 3 //size of colors
        + object_properties.points_coordinates.size() * 2 //size of texels
    );

    //copy everything to the data vector
    for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
        data.push_back(object_properties.points_coordinates.at(i).x);
        data.push_back(object_properties.points_coordinates.at(i).y);
        data.push_back(0.0f);
    }

    if (object_properties.point_colors.size() != object_properties.points_coordinates.size()) {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
        }
    } else {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(object_properties.point_colors.at(i).r);
            data.push_back(object_properties.point_colors.at(i).g);
            data.push_back(object_properties.point_colors.at(i).b);
        }
    }

    if (object_properties.texture_coordinates.size() == object_properties.points_coordinates.size()) {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(object_properties.texture_coordinates.at(i).x);
            data.push_back(object_properties.texture_coordinates.at(i).y);
        };
    } else {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(object_properties.points_coordinates.at(i).x);
            data.push_back(object_properties.points_coordinates.at(i).y);
        }
    }

    return data;
}

std::vector<unsigned int> redhand::game_object_properties::getIndicies(){
    std::vector<unsigned int> trigs;
    auto object_properties = *this;

    trigs.reserve(object_properties.triangle_indices.size() * 3);
    for (unsigned int i = 0; i < object_properties.triangle_indices.size(); i++) {
        trigs.push_back(object_properties.triangle_indices.at(i).at(0));
        trigs.push_back(object_properties.triangle_indices.at(i).at(1));
        trigs.push_back(object_properties.triangle_indices.at(i).at(2));
    }

    return trigs;
}