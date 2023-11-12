#pragma once

#ifdef _WIN32
    #include <SDL.h>
    #include <SDL2_gfxPrimitives.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL2_gfxPrimitives.h>
#endif

#include "../color/color.h"
#include "../font/font.h"
#include "../texture/texture.h"

void renderer_set_default_font(Font* font);
void renderer_set_clip_rect(int x, int y, int width, int height);
void renderer_reset_clip_rect();

Texture* renderer_create_framebuffer(int width, int height);
void renderer_resize_framebuffer(Texture* framebuffer, int width, int height);
void renderer_bind_framebuffer(Texture* framebuffer);

void renderer_clear(Color color);
void renderer_draw_pixel(int x, int y, Color color);
void renderer_draw_line(int x1, int y1, int x2, int y2, int thickness, Color color);
void renderer_draw_rect(int x, int y, int width, int height, Color color);
void renderer_draw_filled_rect(int x, int y, int width, int height, Color color);
void renderer_draw_circle(int x, int y, int radius, Color color);
void renderer_draw_filled_circle(int x, int y, int radius, Color color);
void renderer_draw_ellipse(int x, int y, int rx, int ry, Color color);
void renderer_draw_filled_ellipse(int x, int y, int rx, int ry, Color color);
void renderer_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void renderer_draw_filled_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void renderer_draw_rounded_rect(int x, int y, int width, int height, int radius, Color color);
void renderer_draw_filled_rounded_rect(int x, int y, int width, int height, int radius, Color color);
void renderer_draw_polygon(const short* vx, const short* vy, int n, Color color);
void renderer_draw_filled_polygon(const short* vx, const short* vy, int n, Color color);
void renderer_draw_arc(int x, int y, int radius, int start, int end, Color color);
void renderer_draw_pie(int x, int y, int radius, int start, int end, Color color);
void renderer_draw_filled_pie(int x, int y, int radius, int start, int end, Color color);
void renderer_draw_bezier(const short* vx, const short* vy, int n, int s, Color color);
void renderer_draw_texture(Texture* texture, int x, int y, int width, int height);
void renderer_draw_text(const char* text, int x, int y, Color color);
SDL_Point renderer_query_text_size(const char* text);

void _renderer_set_target(SDL_Renderer* renderer);
