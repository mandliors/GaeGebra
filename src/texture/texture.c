#include "texture.h"
#include "../utils/vector/vector.h"

static Vector* textures;

Texture* texture_load(SDL_Renderer* renderer, const char* path)
{
	Texture* texture = (Texture*)malloc(sizeof(Texture));
	if (texture == NULL)
	{
		printf("failed to allocate memory for texture\n");
		exit(1);
	}
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
void _texture_add(Texture* texture)
{
	vector_push_back(textures, texture);
}
void _texture_close()
{
	for (size_t i = 0; i < vector_size(textures); i++)
	{
		Texture* texture = vector_get(textures, i);
		SDL_DestroyTexture(texture->texture);
		free(texture);
	}
	vector_destroy(textures);
}