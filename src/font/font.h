#pragma once

#ifdef _WIN32
    #include <SDL_ttf.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL_ttf.h>
#endif

/**
 * @brief Holds an TTF_Font and its size
 */
typedef struct Font
{
	TTF_Font* font;
	int size;
} Font;

/**
 * @brief Loads a font from a file (freed automatically when the program closes)
 * 
 * @param path The path to the font file
 * @param size The size of the font
 * @return Font* The font
 */
Font* font_load(const char* path, int size);

/**
 * @brief Creates the font vector that contains all the loaded fonts (should not be called directly, it is needed for the _font_close function)
 */
void _font_init();
/**
 * @brief Destroys the fonts and the font vector (should not be called directly)
 */
void _font_close();
