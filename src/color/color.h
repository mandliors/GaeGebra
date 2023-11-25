#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

typedef SDL_Color Color;

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

/**
 * @brief Converts a hex color to a Color struct
 * 
 * @param hex Hex color
 * @return Color The color
 */
Color color_from_hex(int hex);
/**
 * @brief Creates a color from RGB values
 * 
 * @param r The red value
 * @param g The green value
 * @param b The blue value
 * @return Color The color
 */
Color color_from_rgb(int r, int g, int b);
/**
 * @brief Creates a color from RGBA values
 * 
 * @param r The red value
 * @param g The green value
 * @param b The blue value
 * @param a The alpha value
 * @return Color The color
 */
Color color_from_rgba(int r, int g, int b, int a);
/**
 * @brief Creates a color from HSV values
 * 
 * @param h The hue value
 * @param s The saturation value
 * @param v The value value
 * @return Color The color
 */
Color color_from_hsv(double h, double s, double v);
/**
 * @brief Creates a color from a grayscale value
 * 
 * @param value The grayscale value
 * @return Color The color
 */
Color color_from_grayscale(int value);
/**
 * @brief Fades a color by a certain amount
 * 
 * @param color The color to fade
 * @param fade The amount to fade by (0.0 - 1.0)
 * @return Color The faded color
 */
Color color_fade(Color color, double fade);
/**
 * @brief Shifts a color by a certain amount
 * 
 * @param color The color to shift
 * @param shift The amount to shift by (-255 - 255)
 * @return Color The shifted color
 */
Color color_shift(Color color, int shift);
/**
 * @brief Shifts a color by a certain amount with a different algorithm (only used by ui)
 * 
 * @param color The color to shift
 * @param shift The amount to shift by (-255 - 255)
 * @return Color The shifted color
 */
Color color_clever_shift(Color color, int shift);