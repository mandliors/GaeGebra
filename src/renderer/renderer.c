#include "renderer.h"
#include "../font/font.h"

SDL_Renderer* target_renderer;
Font* default_font;

void renderer_set_default_font(Font* font)
{
	default_font = font;
}
void renderer_set_clip_rect(int x, int y, int width, int height)
{
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderSetClipRect(target_renderer, &rect);
}
void renderer_reset_clip_rect()
{
	SDL_RenderSetClipRect(target_renderer, NULL);
}
void renderer_clear(Color color)
{
	SDL_SetRenderDrawColor(target_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(target_renderer);
}
void renderer_draw_pixel(int x, int y, Color color)
{
	pixelRGBA(target_renderer, x, y, color.r, color.g, color.b, color.a);
}
void renderer_draw_line(int x1, int y1, int x2, int y2, int thickness, Color color)
{
	thickLineRGBA(target_renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
}
void renderer_draw_rect(int x, int y, int width, int height, Color color)
{
	rectangleRGBA(target_renderer, x, y, x + width, y + height, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_rect(int x, int y, int width, int height, Color color)
{
	boxRGBA(target_renderer, x, y, x + width, y + height, color.r, color.g, color.b, color.a);
}
void renderer_draw_circle(int x, int y, int radius, Color color)
{
	aacircleRGBA(target_renderer, x, y, radius, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_circle(int x, int y, int radius, Color color)
{
	filledCircleRGBA(target_renderer, x, y, radius, color.r, color.g, color.b, color.a);
}
void renderer_draw_ellipse(int x, int y, int rx, int ry, Color color)
{
	aaellipseRGBA(target_renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_ellipse(int x, int y, int rx, int ry, Color color)
{
	filledEllipseRGBA(target_renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
}
void renderer_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
{
	aatrigonRGBA(target_renderer, x1, y1, x2, y2, x3, y3, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
{
	filledTrigonRGBA(target_renderer, x1, y1, x2, y2, x3, y3, color.r, color.g, color.b, color.a);
}
void renderer_draw_rounded_rect(int x, int y, int width, int height, int radius, Color color)
{
	roundedRectangleRGBA(target_renderer, x, y, x + width, y + height, radius, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_rounded_rect(int x, int y, int width, int height, int radius, Color color)
{
	roundedBoxRGBA(target_renderer, x, y, x + width, y + height, radius, color.r, color.g, color.b, color.a);
}
void renderer_draw_polygon(const short* vx, const short* vy, int n, Color color)
{
	aapolygonRGBA(target_renderer, vx, vy, n, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_polygon(const short* vx, const short* vy, int n, Color color)
{
	filledPolygonRGBA(target_renderer, vx, vy, n, color.r, color.g, color.b, color.a);
}
void renderer_draw_arc(int x, int y, int radius, int start, int end, Color color)
{
	arcRGBA(target_renderer, x, y, radius, start, end, color.r, color.g, color.b, color.a);
}
void renderer_draw_pie(int x, int y, int radius, int start, int end, Color color)
{
	pieRGBA(target_renderer, x, y, radius, start, end, color.r, color.g, color.b, color.a);
}
void renderer_draw_filled_pie(int x, int y, int radius, int start, int end, Color color)
{
	filledPieRGBA(target_renderer, x, y, radius, start, end, color.r, color.g, color.b, color.a);
}
void renderer_draw_bezier(const short* vx, const short* vy, int n, int s, Color color)
{
	bezierRGBA(target_renderer, vx, vy, n, s, color.r, color.g, color.b, color.a);
}
void renderer_draw_texture(Texture* texture, int x, int y, int width, int height)
{
	SDL_Rect dest = { x, y, width, height };
	SDL_RenderCopy(target_renderer, texture->texture, NULL, &dest);
}
void renderer_draw_text(const char* text, int x, int y, Color color)
{
	if (default_font == NULL)
		return;
	SDL_Surface* surface = TTF_RenderUTF8_Blended(default_font->font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(target_renderer, surface);
	SDL_Rect dest = { x, y, surface->w, surface->h };
	SDL_RenderCopy(target_renderer, texture, NULL, &dest);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
SDL_Point renderer_query_text_size(const char* text)
{
	SDL_Point size = { 0, 0 };
	if (default_font == NULL)
		return size;		
	TTF_SizeUTF8(default_font->font, text, &size.x, &size.y);
	return size;
}

void _renderer_set_target(SDL_Renderer* renderer)
{
	target_renderer = renderer;
}