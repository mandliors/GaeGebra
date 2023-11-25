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

/**
 * @brief Sets the default font for the renderer
 * 
 * @param font The font to set as default
 */
void renderer_set_default_font(Font* font);
/**
 * @brief Sets a clip rect for the renderer
 * 
 * @param x The x coordinate of the clip rect
 * @param y The y coordinate of the clip rect
 * @param width The width of the clip rect
 * @param height The height of the clip rect
 */
void renderer_set_clip_rect(int x, int y, int width, int height);
/**
 * @brief Resets the clip rect of the renderer
 */
void renderer_reset_clip_rect();

/**
 * @brief Creates a new framebuffer
 * 
 * @param width The width of the framebuffer
 * @param height The height of the framebuffer
 * @return Texture* Returns the framebuffer
 */
Texture* renderer_create_framebuffer(int width, int height);
/**
 * @brief Resizes a framebuffer
 * 
 * @param framebuffer The framebuffer to resize
 * @param width The new width of the framebuffer
 * @param height The new height of the framebuffer
 */
void renderer_resize_framebuffer(Texture* framebuffer, int width, int height);
/**
 * @brief Binds a framebuffer
 * 
 * @param framebuffer The framebuffer to destroy
 */
void renderer_bind_framebuffer(Texture* framebuffer);

/**
 * @brief Clears the screen with a color
 * 
 * @param color The color to clear the screen with
 */
void renderer_clear(Color color);
/**
 * @brief Draws a pixel
 * 
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @param color The color of the pixel
 */
void renderer_draw_pixel(int x, int y, Color color);
/**
 * @brief Draws a line
 * 
 * @param x1 The x coordinate of the first point
 * @param y1 The y coordinate of the first point
 * @param x2 The x coordinate of the second point
 * @param y2 The y coordinate of the second point
 * @param thickness The thickness of the line
 * @param color The color of the line
 */
void renderer_draw_line(int x1, int y1, int x2, int y2, int thickness, Color color);
/**
 * @brief Draws a rectangle (not filled)
 * 
 * @param x The x coordinate of the rectangle
 * @param y The y coordinate of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 */
void renderer_draw_rect(int x, int y, int width, int height, Color color);
/**
 * @brief Draws a filled rectangle
 * 
 * @param x The x coordinate of the rectangle
 * @param y The y coordinate of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 */
void renderer_draw_filled_rect(int x, int y, int width, int height, Color color);
/**
 * @brief Draws a circle (not filled)
 * 
 * @param x The x coordinate of the circle
 * @param y The y coordinate of the circle
 * @param radius The radius of the circle
 * @param color The color of the circle
 */
void renderer_draw_circle(int x, int y, int radius, Color color);
/**
 * @brief Draws a filled circle
 * 
 * @param x The x coordinate of the circle
 * @param y The y coordinate of the circle
 * @param radius The radius of the circle
 * @param color The color of the circle
 */
void renderer_draw_filled_circle(int x, int y, int radius, Color color);
/**
 * @brief Draws an ellipse (not filled, axis aligned)
 * 
 * @param x The x coordinate of the ellipse
 * @param y The y coordinate of the ellipse
 * @param rx The x radius of the ellipse
 * @param ry The y radius of the ellipse
 * @param color The color of the ellipse
 */
void renderer_draw_ellipse(int x, int y, int rx, int ry, Color color);
/**
 * @brief Draws a filled ellipse (axis aligned)
 * 
 * @param x The x coordinate of the ellipse
 * @param y The y coordinate of the ellipse
 * @param rx The x radius of the ellipse
 * @param ry The y radius of the ellipse
 * @param color The color of the ellipse
 */
void renderer_draw_filled_ellipse(int x, int y, int rx, int ry, Color color);
/**
 * @brief Draws a triangle (not filled)
 * 
 * @param x1 The x coordinate of the first point
 * @param y1 The y coordinate of the first point
 * @param x2 The x coordinate of the second point
 * @param y2 The y coordinate of the second point
 * @param x3 The x coordinate of the third point
 * @param y3 The y coordinate of the third point
 * @param color The color of the triangle
 */
void renderer_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
/**
 * @brief Draws a filled triangle
 * 
 * @param x1 The x coordinate of the first point
 * @param y1 The y coordinate of the first point
 * @param x2 The x coordinate of the second point
 * @param y2 The y coordinate of the second point
 * @param x3 The x coordinate of the third point
 * @param y3 The y coordinate of the third point
 * @param color The color of the triangle
 */
void renderer_draw_filled_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
/**
 * @brief Draws a rounded rectangle (not filled)
 * 
 * @param x The x coordinate of the rectangle
 * @param y The y coordinate of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param radius The radius of the corners
 * @param color The color of the rectangle
 */
void renderer_draw_rounded_rect(int x, int y, int width, int height, int radius, Color color);
/**
 * @brief Draws a filled rounded rectangle
 * 
 * @param x The x coordinate of the rectangle
 * @param y The y coordinate of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param radius The radius of the corners
 * @param color The color of the rectangle
 */
void renderer_draw_filled_rounded_rect(int x, int y, int width, int height, int radius, Color color);
/**
 * @brief Draws a polygon (not filled)
 * 
 * @param vx The x coordinates of the points
 * @param vy The y coordinates of the points
 * @param n The number of points
 * @param color The color of the polygon
 */
void renderer_draw_polygon(const short* vx, const short* vy, int n, Color color);
/**
 * @brief Draws a filled polygon
 * 
 * @param vx The x coordinates of the points
 * @param vy The y coordinates of the points
 * @param n The number of points
 * @param color The color of the polygon
 */
void renderer_draw_filled_polygon(const short* vx, const short* vy, int n, Color color);
/**
 * @brief Draws an arc (not filled)
 * 
 * @param x The x coordinate of the arc
 * @param y The y coordinate of the arc
 * @param radius The radius of the arc
 * @param start The start angle of the arc
 * @param end The end angle of the arc
 * @param color The color of the arc
 */
void renderer_draw_arc(int x, int y, int radius, int start, int end, Color color);
/**
 * @brief Draws a filled arc (aka pie)
 * 
 * @param x The x coordinate of the arc
 * @param y The y coordinate of the arc
 * @param radius The radius of the arc
 * @param start The start angle of the arc
 * @param end The end angle of the arc
 * @param color The color of the arc
 */
void renderer_draw_pie(int x, int y, int radius, int start, int end, Color color);
/**
 * @brief Draws a bezier curve
 * 
 * @param vx The x coordinates of the points
 * @param vy The y coordinates of the points
 * @param n The number of points
 * @param s The number of segments
 * @param color The color of the bezier curve
 */
void renderer_draw_bezier(const short* vx, const short* vy, int n, int s, Color color);
/**
 * @brief Draws a texture
 * 
 * @param texture The texture to draw
 * @param x The x coordinate of the texture
 * @param y The y coordinate of the texture
 * @param width The width of the texture
 * @param height The height of the texture
 */
void renderer_draw_texture(Texture* texture, int x, int y, int width, int height);
/**
 * @brief Draws a text
 * 
 * @param text The text to draw
 * @param x The x coordinate of the text
 * @param y The y coordinate of the text
 * @param color The color of the text
 */
void renderer_draw_text(const char* text, int x, int y, Color color);
/**
 * @brief Returns the size of a text
 * 
 * @param text The text to get the size of
 * @return SDL_Point The size of the text
 */
SDL_Point renderer_query_text_size(const char* text);

/**
 * @brief Sets the target renderer for the application (should not be called manually)
 * 
 * @param renderer The renderer to set as target
 */
void _renderer_set_target(SDL_Renderer* renderer);
