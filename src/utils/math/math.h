#ifndef MATH_H
#define MATH_H

#include <stdbool.h>

#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647692
#define HALF_PI 1.57079632679489661923

double deg_to_rad(double deg);
double rad_to_deg(double rad);
double clamp(double x, double min, double max);
double lerp(double a, double b, double t);
double map(double x, double min1, double max1, double min2, double max2);

bool check_collision_point_rect(int px, int py, int rx, int ry, int rw, int rh);

#endif