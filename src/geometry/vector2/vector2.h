#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

/**
 * @brief A 2D vector, used for coordinate geometry
 */
typedef struct Vector2
{
    double x, y;
} Vector2;

/**
 * @brief Creates a vector from x and y values
 * 
 * @param x The x value
 * @param y The y value
 * @return Vector2 The vector
 */
Vector2 vector2_create(double x, double y);
/**
 * @brief Creates a vector from polar coordinates
 * 
 * @param angle The angle
 * @param length The length
 * @return Vector2 The vector
 */
Vector2 vector2_from_polar(double angle, double length);
/**
 * @brief Creates a vector from am SDL_Point
 * 
 * @param point The point
 * @return Vector2 The vector
 */
Vector2 vector2_from_point(SDL_Point point);

/**
 * @brief Returns a vector with x and y values of 0
 * 
 * @return Vector2 The vector
 */
Vector2 vector2_zero();
/**
 * @brief Returns a vector with x and y values of 1
 * 
 * @return Vector2 The vector
 */
Vector2 vector2_one();
/**
 * @brief Returns a vector with x value of 0 and y value of 1
 * 
 * @return Vector2 The vector
 */
Vector2 vector2_up();
/**
 * @brief Returns a vector with x value of 0 and y value of -1
 * 
 * @return Vector2 The vector
 */
Vector2 vector2_down();
/**
 * @brief Returns a vector with x value of -1 and y value of 0
 * 
 * @return Vector2 The vector
 */
Vector2 vector2_left();
/**
 * @brief Returns a vector with x value of 1 and y value of 0
 * 
 * @return Vector2 The vector
 */
Vector2 vector2_right();

/**
 * @brief Adds two vectors
 * 
 * @param a The first vector
 * @param b The second vector
 * @return Vector2 The sum of the two vectors
 */
Vector2 vector2_add(Vector2 a, Vector2 b);
/**
 * @brief Subtracts two vectors
 * 
 * @param a The first vector
 * @param b The second vector
 * @return Vector2 The difference of the two vectors
 */
Vector2 vector2_subtract(Vector2 a, Vector2 b);
/**
 * @brief Scales a vector by a scalar
 * 
 * @param a The vector
 * @param b The scale
 * @return Vector2 The scaled vector
 */
Vector2 vector2_scale(Vector2 a, double b);
/**
 * @brief Negates a vector
 * 
 * @param a The vector
 * @return Vector2 The negated vector
 */
Vector2 vector2_negate(Vector2 a);
/**
 * @brief Multiplies two vectors component-wise
 * 
 * @param a The first vector
 * @param b The second vector
 * @return Vector2 The product of the two vectors
 */
Vector2 vector2_multiply(Vector2 a, Vector2 b);
/**
 * @brief Divides two vectors component-wise
 * 
 * @param a The first vector
 * @param b The second vector
 * @return Vector2 The quotient of the two vectors
 */
Vector2 vector2_divide(Vector2 a, Vector2 b);
/**
 * @brief Calculates the dot product of two vectors
 * 
 * @param a The first vector
 * @param b The second vector
 * @return double The dot product of the two vectors
 */
double vector2_dot(Vector2 a, Vector2 b);
/**
 * @brief Calculates the cross product of two vectors
 * 
 * @param a The first vector
 * @param b The second vector
 * @return double The cross product of the two vectors
 */
double vector2_cross(Vector2 a, Vector2 b);
/**
 * @brief Calculates the length of a vector
 * 
 * @param a The vector
 * @return double The length of the vector
 */
double vector2_length(Vector2 a);
/**
 * @brief Calculates the distance between two vectors
 * 
 * @param a The first vector
 * @param b The second vector
 * @return double The distance between the two vectors
 */
double vector2_distance(Vector2 a, Vector2 b);
/**
 * @brief Calculates the angle of a vector
 * 
 * @param a The vector
 * @return double The angle of the vector
 */
double vector2_angle(Vector2 a);
/**
 * @brief Normalizes a vector
 * 
 * @param a The vector
 * @return Vector2 The normalized vector
 */
Vector2 vector2_normalize(Vector2 a);
/**
 * @brief Rotates a vector by 90 degrees
 * 
 * @param a The vector
 * @return Vector2 The rotated vector
 */
Vector2 vector2_rotate90(Vector2 a);
/**
 * @brief Rotates a vector by an angle
 * 
 * @param a The vector
 * @param angle The angle
 * @return Vector2 The rotated vector
 */
Vector2 vector2_rotate(Vector2 a, double angle);
/**
 * @brief Reflects a vector over a normal
 * 
 * @param a The vector
 * @param normal The normal
 * @return Vector2 The reflected vector
 */
Vector2 vector2_reflect(Vector2 a, Vector2 normal);