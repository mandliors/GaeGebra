#ifdef _WIN32
    #include <SDL2_gfxPrimitives.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL2_gfxPrimitives.h>
#endif

#include <assert.h>

#include "window.h"
#include "../app/app.h"
#include "../input/input.h"

#include "../debugmalloc.h"

Window* window_create(const char* title, int width, int height, int flags)
{
    Window* window = (Window*)malloc(sizeof(Window));
    assert(window); //couldn't allocate memory    
    window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (window->window == NULL)
    {
        SDL_Log("unable to create window: %s", SDL_GetError());
        exit(1);
    }
    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
    if (window->renderer == NULL)
    {
        SDL_Log("unable to create renderer: %s", SDL_GetError());
        exit(1);
    }
    window->close_requested = false;
    _input_init(&window->input_data);
    _ui_init(&window->ui_data, width, height);
    _app_add_window(window);

    SDL_RenderClear(window->renderer);
    return window;
}
void window_show(Window* window)
{
	SDL_ShowWindow(window->window);
}
void window_hide(Window* window)
{
	SDL_HideWindow(window->window);
}
void window_focus(Window* window)
{
    _app_set_active_window(window);
    SDL_RaiseWindow(window->window);
}
UIContainer* window_get_main_container(Window* window)
{
    return window->ui_data.main_container;
}

void _window_reset(Window* window)
{
    window->close_requested = false;
    _input_reset(&window->input_data);
}
void _window_handle_event(Window* window, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT)
    {
        if (event->window.event == SDL_WINDOWEVENT_CLOSE)
        {
            window_hide(window);
            window->close_requested = true;
        }
        else if (event->window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
            _app_set_active_window(window);
    }
    _input_handle_event(&window->input_data, event);
    _ui_handle_event(&window->ui_data, event);
}
void _window_update(Window* window)
{
    _ui_update(&window->ui_data);
}
void _window_render(Window* window)
{
    _ui_render(&window->ui_data);
}
void _window_close(Window* window)
{
    _input_close(&window->input_data);
    _ui_close(&window->ui_data);

    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
}
