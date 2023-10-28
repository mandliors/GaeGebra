#ifndef FONTS_H
#define FONTS_H

#ifdef _WIN32
    #include <SDL_ttf.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL_ttf.h>
#endif

#include "../debugmalloc.h"

typedef struct Font
{
	TTF_Font* font;
	int size;
} Font;

Font* font_load(const char* path, int size);
void font_unload(Font* font);

#endif