#ifdef _WIN32
    #include <SDL_ttf.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL_ttf.h>
#endif

#include "app.h"
#include "../window/window.h"

#include "../debugmalloc.h"

AppData app_data;

void app_init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("unable to start SDL: %s", SDL_GetError());
        exit(1);
    }
    if (TTF_Init() == -1)
    {
        SDL_Log("unable to initialize SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }
    app_data.windows = vector_create(0);
    app_data.active_window = NULL;
    app_data.target_frame_time = 1000 / 60;
    app_data.frame_start = SDL_GetTicks();
}
void app_update()
{
    app_data.frame_start = SDL_GetTicks();
    for (int i = 0; i < app_data.windows->size; i++)
        _window_reset((Window*)vector_get(app_data.windows, i));

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        for (int i = 0; i < app_data.windows->size; i++)
        {
            Window* window = (Window*)vector_get(app_data.windows, i);
            if (event.window.windowID == SDL_GetWindowID(window->window))
            {
                _window_handle_event(window, &event);
                break;
            }
        }
    }
    for (int i = 0; i < app_data.windows->size; i++)
        _window_update((Window*)vector_get(app_data.windows, i));
}
void app_render()
{
    for (int i = 0; i < app_data.windows->size; i++)
    {
		Window* window = (Window*)vector_get(app_data.windows, i);
        _window_render(window);
		SDL_RenderPresent(window->renderer);
	}
    Uint32 frame_time = SDL_GetTicks() - app_data.frame_start;
    if (frame_time < app_data.target_frame_time)
        SDL_Delay(app_data.target_frame_time - frame_time);
}
void app_request_close()
{
    for (int i = 0; i < app_data.windows->size; i++)
        ((Window*)vector_get(app_data.windows, i))->close_requested = true;
}
void app_close()
{
    for (int i = 0; i < app_data.windows->size; i++)
    {
        Window* window = (Window*)vector_get(app_data.windows, i);
        _window_close(window);
        free(window);
    }
    app_data.active_window = NULL;
    vector_free(app_data.windows);
    TTF_Quit();
    SDL_Quit();
}
void app_set_target_fps(int fps)
{
    app_data.target_frame_time = 1000 / fps;
}

Window* app_get_active_window()
{
    return app_data.active_window;
}
Vector* app_get_windows()
{
    return app_data.windows;
}

void _app_add_window(Window* window)
{
    vector_push_back(app_data.windows, (void*)window);
    _app_set_active_window(window);
}
void _app_set_active_window(Window* window)
{
	app_data.active_window = window;
}