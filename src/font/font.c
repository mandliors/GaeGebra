#include "font.h"
#include "../utils/vector/vector.h"

Vector* fonts;

Font* font_load(const char* path, int size)
{
	Font* font = (Font*)malloc(sizeof(Font));
	if (font == NULL)
	{
		printf("couldn't allocate memory for font\n");
		exit(1);
	}
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL)
	{
		printf("couldn't load font from path: %s\n", path);
		exit(1);
	}
	font->size = size;
	vector_push_back(fonts, font);
	return font;
}

void _font_init()
{
	fonts = vector_create(0);
}
void _font_close()
{
	for (size_t i = 0; i < vector_size(fonts); i++)
	{
		Font* font = vector_get(fonts, i);
		TTF_CloseFont(font->font);
		free(font);
	}
	vector_free(fonts);
}