#include "math.h"

#include "../../debugmalloc.h"

double deg_to_rad(double deg)
{
    return deg * (PI / 180);
}
double rad_to_deg(double rad)
{
    return rad * (180 / PI);
}
double clamp(double value, double min, double max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
double lerp(double a, double b, double t)
{
    return a + (b - a) * t;
}
double map(double x, double min1, double max1, double min2, double max2)
{
    return (x - min1) / (max1 - min1) * (max2 - min2) + min2;
}

bool check_collision_point_rect(int px, int py, int rx, int ry, int rw, int rh)
{
    if (px < rx) return false;
    if (px > rx + rw) return false;
    if (py < ry) return false;
    if (py > ry + rh) return false;
    return true;
}