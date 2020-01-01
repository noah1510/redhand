/** This function converts an angle form degree to radians.
 * @param val The angle in degrees
 * @return float The ange in radians
 */

float degToRad(float val){
    return val/180*3.1415;
}

/** This function calculates a sine with an angle in degrees
 * 
 * @param deg The angle in Degrees
 * @return float The sine of that angle
 */
float sinDeg(float deg){
    return sin(degToRad(deg));
}

/** This function calculates a cosine with an angle in degrees
 * 
 * @param deg The angle in Degrees
 * @return float The cosine of that angle
 */
float cosDeg(float deg){
    return cos(degToRad(deg));
}