/**
 * @file math.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Auxillary functions
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include <cmath>

namespace redhand {

    /** This function calculates a sine with an angle in degrees
    * 
    * @param deg The angle in Degrees
    * @return float The sine of that angle
    */
    float sinDeg(float deg);

    /** This function calculates a cosine with an angle in degrees
    * 
    * @param deg The angle in Degrees
    * @return float The cosine of that angle
    */
    float cosDeg(float deg);

} // namespace redhand