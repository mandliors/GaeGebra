#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "../window/window.h"
#include "../utils/vector/vector.h"

/**
 * @brief Contains the application data, like windows and target fps. There is only one instance of this struct, and should not be modified directly
 */
typedef struct AppData
{
    Vector* windows;
    Uint32 target_frame_time;
    Uint32 last_frame_start;
    Uint32 frame_start;
    double delta_time;
} AppData;

/**
 * @brief Initializes the application and SDL
 */
void app_init();
/**
 * @brief Updates the windows and handles events
 */
void app_update();
/**
 * @brief Renders the windows and waits for the target frame time
 */
void app_render();
/**
 * @brief This should be called to safely close the application
 */
void app_request_close();
/**
 * @brief Closes the application cleans up resources (SDL stuff)
 */
void app_close();
/**
 * @brief Sets the target fps
 * 
 * @param fps The target fps
 */
void app_set_target_fps(Uint32 fps);

/**
 * @brief Sets the target window for rendering, input handling and UI. A target should be set before doing any of these things
 * 
 * @param window The target window
 */
void app_set_target(Window* window);
/**
 * @brief Returns the target window
 * 
 * @return Window* The target window
 */
Window* app_get_target();
/**
 * @brief Returns the added windows
 * 
 * @return Vector* The windows (should not be modified or freed)
 */
Vector* app_get_windows();
/**
 * @brief Returns the time since the application started
 * 
 * @return double The elapsed time
 */
double app_get_time();
/**
 * @brief Returns the time since the last frame
 * 
 * @return double The delta time
 */
double app_get_delta_time();

/**
 * @brief Adds a window to the application (this is an internal function, should not be called directly)
 * 
 * @param window The window to add
 */
void _app_add_window(Window* window);
