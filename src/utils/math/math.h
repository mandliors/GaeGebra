#pragma once

#include <stdbool.h>

#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647692
#define HALF_PI 1.57079632679489661923

/**
 * @brief Converts degrees to radians
 * 
 * @param deg The angle in degrees
 * @return double The angle in radians
 */
double deg_to_rad(double deg);
/**
 * @brief Converts radians to degrees
 * 
 * @param rad The angle in radians
 * @return double The angle in degrees
 */
double rad_to_deg(double rad);
/**
 * @brief Clamps a value between a minimum and maximum value
 * 
 * @param x The value to clamp
 * @param min The minimum value
 * @param max The maximum value
 * @return double The clamped value
 */
double clamp(double x, double min, double max);
/**
 * @brief Linearly interpolates between two values
 * 
 * @param a The first value
 * @param b The second value
 * @param t The interpolation value
 * @return double The interpolated value
 */
double lerp(double a, double b, double t);
/**
 * @brief Maps a value from one range to another
 * 
 * @param x The value to map
 * @param min1 The minimum value of the first range
 * @param max1 The maximum value of the first range
 * @param min2 The minimum value of the second range
 * @param max2 The maximum value of the second range
 * @return double The mapped value
 */
double map(double x, double min1, double max1, double min2, double max2);

/**
 * @brief Checks if a point is inside a rectangle
 * 
 * @param px The x coordinate of the point
 * @param py The y coordinate of the point
 * @param rx The x coordinate of the rectangle
 * @param ry The y coordinate of the rectangle
 * @param rw The width of the rectangle
 * @param rh The height of the rectangle
 * @return true If the point is inside the rectangle
 * @return false If the point is outside the rectangle
 */
bool check_collision_point_rect(int px, int py, int rx, int ry, int rw, int rh);
