#include "trigonometry.hpp"

namespace be{

/**
 * Degrees to radians conversion
 * @param angle The angle in degrees
 * @return The angle in radians
 * @note Double precision
*/
double radians(double degree){
    return degree * PI / 180.;
}

/**
 * Degrees to radians conversion
 * @param angle The angle in degrees
 * @return The angle in radians
 * @note Single precision
*/
float radians(float degree){
    return degree * PI / 180.;
}

/**
 * Radians to degrees conversion
 * @param angle The angle in radians
 * @return The angle in degrees
 * @note Double precision
*/
double degrees(double radians){
    return radians * 180. / PI;
}

/**
 * Radians to degrees conversion
 * @param angle The angle in radians
 * @return The angle in degrees
 * @note Single precision
*/
float degrees(float radians){
    return radians * 180. / PI;
}

};