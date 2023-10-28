#include <assert.h>

#include "fonts.h"

#include "../debugmalloc.h"

Font* font_load(const char* path, int size)
{
	Font* font = (Font*)malloc(sizeof(Font));
	assert(font); //couldn't allocate memory
	font->font = TTF_OpenFont(path, size);
	assert(font->font); //couldn't load font
	font->size = size;
	return font;
}
void font_unload(Font* font)
{
	TTF_CloseFont(font->font);
	free(font);
}