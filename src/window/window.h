#ifndef WINDOW_H
#define WINDOW_H

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include <stdbool.h>
#include "../input/input.h"
#include "../ui/ui.h"
#include "../ui/ui_elements/ui_elements.h"

typedef struct Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    InputData input_data;
    UIData ui_data;
    bool close_requested;
} Window;

Window* window_create(const char* title, int width, int height, int flags);
void window_show(Window* window);
void window_hide(Window* window);
void window_focus(Window* window);
UIContainer* window_get_main_container(Window* window);

//API functions
void _window_reset(Window* window);
void _window_handle_event(Window* window, SDL_Event* event);
void _window_update(Window* window);
void _window_render(Window* window);
void _window_close(Window* window);

#endif
