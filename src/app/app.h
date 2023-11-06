#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "../window/window.h"
#include "../utils/vector/vector.h"

typedef struct AppData
{
    Vector* windows;
    Uint32 target_frame_time;
    Uint32 last_frame_start;
    Uint32 frame_start;
    double delta_time;
} AppData;

void app_init();
void app_update();
void app_render();
void app_request_close();
void app_close();
void app_set_target_fps(Uint32 fps);

void app_set_target(Window* window);
Vector* app_get_windows();
double app_get_time();
double app_get_delta_time();

//internal functions
void _app_add_window(Window* window);
