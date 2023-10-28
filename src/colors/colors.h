#ifndef COLORS_H
#define COLORS_H

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "../defines.h"

#define WHITE (Color) { 255, 255, 255, 255 }
#define BLACK (Color) { 0, 0, 0, 255 }
#define GRAY (Color) { 128, 128, 128, 255 }
#define DARK_GRAY (Color) { 40, 40, 40, 255 }
#define RED (Color) { 255, 0, 0, 255 }
#define GREEN (Color) { 0, 255, 0, 255 }
#define BLUE (Color) { 0, 0, 255, 255 }
#define YELLOW (Color) { 255, 255, 0, 255 }
#define MAGENTA (Color) { 255, 0, 255, 255 }
#define CYAN (Color) { 0, 255, 255, 255 }
#define TRANSPARENT (Color) { 0, 0, 0, 0 }

Color color_from_hex(int hex);
Color color_from_rgb(int r, int g, int b);
Color color_from_rgba(int r, int g, int b, int a);
Color color_from_hsv(float h, float s, float v);
Color color_from_hsl(float h, float s, float l);
Color color_fade(Color color, double fade);
Color color_shift(Color color, int shift);

#endif