#pragma once

#ifdef _WIN32
    #include <SDL_ttf.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL_ttf.h>
#endif

typedef struct Font
{
	TTF_Font* font;
	int size;
} Font;

Font* font_load(const char* path, int size);

//internal functions
void _font_init();
void _font_close();
