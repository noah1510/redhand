#include "redhand/math.hpp"

using namespace redhand;

float redhand::sinDeg(float deg){
    return std::sin(deg/180*3.1415);
}

float redhand::cosDeg(float deg){
    return std::cos(deg/180*3.1415);
}