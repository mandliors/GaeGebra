#include <assert.h>
#include "texture.h"

#include "../debugmalloc.h"

Texture* texture_load(SDL_Renderer* renderer, const char* path)
{
	Texture* texture = (Texture*)malloc(sizeof(Texture));
	assert(texture); //couldn't allocate memory
	SDL_Texture* tex = IMG_LoadTexture(renderer, path);
	SDL_QueryTexture(tex, NULL, NULL, &texture->width, &texture->height);
	texture->texture = tex;
	return texture;
}
void texture_unload(Texture* texture)
{
	SDL_DestroyTexture(texture->texture);
	free(texture);
}