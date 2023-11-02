#include <assert.h>

#include "texture.h"
#include "../utils/vector/vector.h"

Vector* textures;

Texture* texture_load(SDL_Renderer* renderer, const char* path)
{
	Texture* texture = (Texture*)malloc(sizeof(Texture));
	assert(texture); //couldn't allocate memory
	SDL_Texture* tex = IMG_LoadTexture(renderer, path);
	SDL_QueryTexture(tex, NULL, NULL, &texture->width, &texture->height);
	texture->texture = tex;
	vector_push_back(textures, texture);
	return texture;
}

void _texture_init()
{
	textures = vector_create(0);
}
void _texture_close()
{
	for (size_t i = 0; i < textures->size; i++)
	{
		Texture* texture = vector_get(textures, i);
		SDL_DestroyTexture(texture->texture);
		free(texture);
	}
	vector_free(textures);
}