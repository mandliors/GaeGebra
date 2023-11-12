#ifdef _WIN32
    #include <SDL_ttf.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL_ttf.h>
#endif

#include "app.h"
#include "../window/window.h"
#include "../renderer/renderer.h"
#include "../input/input.h"
#include "../ui/ui.h"

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
    app_data.target_frame_time = 1000 / 60;
    app_data.last_frame_start = SDL_GetTicks();
    app_data.frame_start = SDL_GetTicks();
    app_data.delta_time = 0.0;
    _font_init();
    _texture_init();
}
void app_update()
{
    app_data.frame_start = SDL_GetTicks();
    app_data.delta_time = (double)(app_data.frame_start - app_data.last_frame_start) / 1000.0;
    app_data.last_frame_start = app_data.frame_start;

    for (size_t i = 0; i < vector_size(app_data.windows); i++)
        _window_reset((Window*)vector_get(app_data.windows, i));

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        for (size_t i = 0; i < vector_size(app_data.windows); i++)
        {
            Window* window = (Window*)vector_get(app_data.windows, i);
            if (event.window.windowID == SDL_GetWindowID(window->window))
            {
                _window_handle_event(window, &event);
                break;
            }
        }
    }
    for (size_t i = 0; i < vector_size(app_data.windows); i++)
        _window_update((Window*)vector_get(app_data.windows, i));
}
void app_render()
{
    for (size_t i = 0; i < vector_size(app_data.windows); i++)
    {
		Window* window = (Window*)vector_get(app_data.windows, i);
        _window_render(window);
	}
    Uint32 frame_time = SDL_GetTicks() - app_data.frame_start;
    if (frame_time < app_data.target_frame_time)
        SDL_Delay(app_data.target_frame_time - frame_time);
}
void app_request_close()
{
    for (size_t i = 0; i < vector_size(app_data.windows); i++)
        ((Window*)vector_get(app_data.windows, i))->close_requested = true;
}
void app_close()
{
    _font_close();
    _texture_close();
    for (size_t i = 0; i < vector_size(app_data.windows); i++)
    {
        Window* window = (Window*)vector_get(app_data.windows, i);
        _window_close(window);
        free(window);
    }
    vector_free(app_data.windows);
    TTF_Quit();
    SDL_Quit();
}
void app_set_target_fps(Uint32 fps)
{
    app_data.target_frame_time = 1000 / fps;
}

void app_set_target(Window* window)
{
    _renderer_set_target(window->renderer);
    _input_set_target(&window->input_data);
    _ui_set_target(&window->ui_data);
}
Vector* app_get_windows()
{
    return app_data.windows;
}
double app_get_time()
{
    return (double)SDL_GetTicks() / 1000.0;
}
double app_get_delta_time()
{
    return (double)app_data.delta_time;
}

void _app_add_window(Window* window)
{
    vector_push_back(app_data.windows, (void*)window);
    app_set_target(window);
}