#pragma once

#ifdef _WIN32
    #include <SDL.h>
	#include <SDL_image.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

/**
 * @brief The texture struct that holds the texture data (SDL_Texture*, width, height)
 */
typedef struct Texture
{
	SDL_Texture* texture;
	int width;
	int height;
} Texture;

/**
 * @brief Loads a texture from a file (freed automatically when the program closes)
 * 
 * @param renderer The renderer to load the texture with
 * @param path The path to the file
 * @return Texture* Returns the loaded texture
 */
Texture* texture_load(SDL_Renderer* renderer, const char* path);

/**
 * @brief Creates the texture vector that contains all the loaded textures (should not be called directly, it is needed for the _texture_close function)
 */
void _texture_init();
/**
 * @brief Adds a texture to the texture vector (should not be called manually)
 * 
 * @param texture The texture to add
 */
void _texture_add(Texture* texture);
/**
 * @brief Destroys the textures and the texture vector (should not be called directly)
 */
void _texture_close();
