#include "renderer.h"
#include "../texture/texture.h"
#include "../font/font.h"

static SDL_Renderer* target_renderer;
static Font* default_font;

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

Texture* renderer_create_framebuffer(int width, int height)
{
	SDL_Texture* framebuffer = SDL_CreateTexture(target_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height);
	
	Texture* texture = (Texture*)malloc(sizeof(Texture));
	texture->texture = framebuffer;
	texture->width = width;
	texture->height = height;
	_texture_add(texture);

	return texture;
}
void renderer_resize_framebuffer(Texture* framebuffer, int width, int height)
{
	SDL_DestroyTexture(framebuffer->texture);
	framebuffer->texture = SDL_CreateTexture(target_renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width, height);
	framebuffer->width = width;
	framebuffer->height = height;
}
void renderer_bind_framebuffer(Texture* framebuffer)
{
	if (framebuffer == NULL)
		SDL_SetRenderTarget(target_renderer, NULL);
	else
		SDL_SetRenderTarget(target_renderer, framebuffer->texture);
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
	if (thickness == 1)
	{
		aalineRGBA(target_renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
		return;
	}

    double dx = x2 - x1;
    double dy = y2 - y1;
    double length = sqrt(dx * dx + dy * dy);
    if (length > 0.0)
	{
        dx /= length;
        dy /= length;
    }
    double ox = thickness * 0.5 * dy;
    double oy = thickness * 0.5 * (-dx);

    double x1a = x1 + ox; double x2a = x2 + ox;
    double y1a = y1 + oy; double y2a = y2 + oy;
    double x1b = x1 - ox; double x2b = x2 - ox;
    double y1b = y1 - oy; double y2b = y2 - oy;

	aalineRGBA(target_renderer, x1a, y1a, x2a, y2a, color.r, color.g, color.b, color.a);
	aalineRGBA(target_renderer, x1b, y1b, x2b, y2b, color.r, color.g, color.b, color.a);
	filledPolygonRGBA(target_renderer, (const short[]){ x1a, x2a, x2b, x1b }, (const short[]){ y1a, y2a, y2b, y1b }, 4, color.r, color.g, color.b, color.a);
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
	aacircleRGBA(target_renderer, x, y, radius, color.r, color.g, color.b, color.a);
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