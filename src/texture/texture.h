#ifndef TEXTURE_H
#define TEXTURE_H

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
void texture_unload(Texture* texture);

#endif