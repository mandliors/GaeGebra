#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

typedef struct Vector2
{
    double x, y;
} Vector2;

Vector2 vector2_create(double x, double y);
Vector2 vector2_from_polar(double angle, double length);
Vector2 vector2_from_point(SDL_Point point);

Vector2 vector2_zero();
Vector2 vector2_one();
Vector2 vector2_up();
Vector2 vector2_down();
Vector2 vector2_left();
Vector2 vector2_right();

Vector2 vector2_add(Vector2 a, Vector2 b);
Vector2 vector2_subtract(Vector2 a, Vector2 b);
Vector2 vector2_scale(Vector2 a, double b);
Vector2 vector2_negate(Vector2 a);
Vector2 vector2_multiply(Vector2 a, Vector2 b);
Vector2 vector2_divide(Vector2 a, Vector2 b);
double vector2_dot(Vector2 a, Vector2 b);
double vector2_length(Vector2 a);
double vector2_distance(Vector2 a, Vector2 b);
double vector2_angle(Vector2 a);
Vector2 vector2_normalize(Vector2 a);
Vector2 vector2_rotate90(Vector2 a);
Vector2 vector2_rotate(Vector2 a, double angle);
Vector2 vector2_reflect(Vector2 a, Vector2 normal);