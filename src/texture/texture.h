#pragma once

#ifdef _WIN32
    #include <SDL.h>
	#include <SDL_image.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

typedef struct Texture
{
	SDL_Texture* texture;
	int width;
	int height;
} Texture;

Texture* texture_load(SDL_Renderer* renderer, const char* path);

//internal functions
void _texture_init();
void _texture_close();
