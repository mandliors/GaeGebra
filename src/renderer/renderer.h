#ifndef RENDERER_H
#define RENDERER_H

#ifdef _WIN32
    #include <SDL.h>
    #include <SDL2_gfxPrimitives>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL2_gfxPrimitives.h>
#endif

#include "../colors/colors.h"
#include "../fonts/fonts.h"
#include "../texture/texture.h"
#include "../defines.h"

void renderer_set_target(SDL_Renderer* target);
void renderer_set_default_font(Font* font);
void renderer_clear(Color color);

void draw_pixel(int x, int y, Color color);
void draw_line(int x1, int y1, int x2, int y2, int thickness, Color color);
void draw_rect(int x, int y, int width, int height, Color color);
void draw_filled_rect(int x, int y, int width, int height, Color color);
void draw_circle(int x, int y, int radius, Color color);
void draw_filled_circle(int x, int y, int radius, Color color);
void draw_ellipse(int x, int y, int rx, int ry, Color color);
void draw_filled_ellipse(int x, int y, int rx, int ry, Color color);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void draw_filled_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void draw_rounded_rect(int x, int y, int width, int height, int radius, Color color);
void draw_filled_rounded_rect(int x, int y, int width, int height, int radius, Color color);
void draw_polygon(const short* vx, const short* vy, int n, Color color);
void draw_filled_polygon(const short* vx, const short* vy, int n, Color color);
void draw_arc(int x, int y, int radius, int start, int end, Color color);
void draw_pie(int x, int y, int radius, int start, int end, Color color);
void draw_filled_pie(int x, int y, int radius, int start, int end, Color color);
void draw_bezier(const short* vx, const short* vy, int n, int s, Color color);
void draw_texture(Texture* texture, int x, int y, int width, int height);
void draw_text(const char* text, int x, int y, Color color);
Vector2 query_text_size(const char* text);

#endif