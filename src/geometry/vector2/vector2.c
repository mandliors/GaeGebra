#include "vector2.h"

#include <math.h>

Vector2 vector2_create(double x, double y) { return (Vector2){x, y}; }
Vector2 vector2_from_polar(double angle, double length) { return (Vector2){ cos(angle) * length, sin(angle) * length}; }
Vector2 vector2_from_point(SDL_Point point) { return (Vector2){point.x, point.y}; }

Vector2 vector2_zero() { return (Vector2){0, 0}; }
Vector2 vector2_one() { return (Vector2){1, 1}; }
Vector2 vector2_up() { return (Vector2){0, 1}; }
Vector2 vector2_down() { return (Vector2){0, -1}; }
Vector2 vector2_left() { return (Vector2){-1, 0}; }
Vector2 vector2_right() { return (Vector2){1, 0}; }

Vector2 vector2_add(Vector2 a, Vector2 b) { return (Vector2){a.x + b.x, a.y + b.y}; }
Vector2 vector2_subtract(Vector2 a, Vector2 b) { return (Vector2){a.x - b.x, a.y - b.y}; }
Vector2 vector2_scale(Vector2 a, double b) { return (Vector2){a.x * b, a.y * b}; }
Vector2 vector2_negate(Vector2 a) { return (Vector2){-a.x, -a.y}; }
Vector2 vector2_multiply(Vector2 a, Vector2 b) { return (Vector2){a.x * b.x, a.y * b.y}; }
Vector2 vector2_divide(Vector2 a, Vector2 b) { return (Vector2){a.x / b.x, a.y / b.y}; }
double vector2_dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }
double vector2_cross(Vector2 a, Vector2 b) { return a.x * b.y - a.y * b.x; }
double vector2_length(Vector2 a) { return sqrt(vector2_dot(a, a)); }
double vector2_distance(Vector2 a, Vector2 b) { return vector2_length(vector2_subtract(a, b)); }
double vector2_angle(Vector2 a) { return atan2(a.y, a.x); }
Vector2 vector2_normalize(Vector2 a) { return vector2_scale(a, 1.0 / vector2_length(a)); }
Vector2 vector2_rotate90(Vector2 a) { return (Vector2){-a.y, a.x}; }
Vector2 vector2_rotate(Vector2 a, double angle) { return vector2_from_polar(vector2_angle(a) + angle, vector2_length(a)); }
Vector2 vector2_reflect(Vector2 a, Vector2 normal) { return vector2_subtract(a, vector2_scale(normal, 2 * vector2_dot(a, normal))); }