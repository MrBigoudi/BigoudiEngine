#pragma once

namespace be{

/**
 * The pi constant
*/
double const PI = 3.14159265359;

/**
 * Degrees to radians conversion
 * @param angle The angle in degrees
 * @return The angle in radians
 * @note Double precision
*/
double radians(double degree);

/**
 * Degrees to radians conversion
 * @param angle The angle in degrees
 * @return The angle in radians
 * @note Single precision
*/
float radians(float degree);

/**
 * Radians to degrees conversion
 * @param angle The angle in radians
 * @return The angle in degrees
 * @note Double precision
*/
double degrees(double radians);

/**
 * Radians to degrees conversion
 * @param angle The angle in radians
 * @return The angle in degrees
 * @note Single precision
*/
float degrees(float radians);

};