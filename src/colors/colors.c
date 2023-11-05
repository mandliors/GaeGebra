#include "colors.h"

Color color_from_hex(int hex)
{
    return (Color)
    {
        (hex >> 16) & 0xFF,
        (hex >> 8) & 0xFF,
        hex & 0xFF,
        255
    };
}
Color color_from_rgb(int r, int g, int b)
{
    return (Color) { r, g, b, 255 };
}
Color color_from_rgba(int r, int g, int b, int a)
{
    return (Color) { r, g, b, a };
}
Color color_from_hsv(double h, double s, double v)
{
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double r, g, b;
    if (h < 60)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    return (Color)
    {
        (r + m) * 255,
        (g + m) * 255,
        (b + m) * 255,
        255
    };
}
Color color_from_grayscale(int value)
{
    return (Color) { value, value, value, 255 };
}
Color color_fade(Color color, double fade)
{
    return (Color)
    {
        color.r,
        color.g,
        color.b,
        (int)(color.a * fade)
    };
}
Color color_shift(Color color, int shift)
{
    return (Color)
    {
        (color.r + shift + 256) % 256,
        (color.g + shift + 256) % 256,
        (color.b + shift + 256) % 256,
        color.a
    };
}