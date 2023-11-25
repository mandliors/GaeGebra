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
#include "../ui/ui_element/ui_element.h"

/**
 * @brief The Window struct, contains an SDL_Window, an SDL_Renderer and other window specific data,
 * like the input data and the UI data, and a flag to check if the window is requested to be closed
 */
typedef struct Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    InputData input_data;
    UIData ui_data;
    bool close_requested;
} Window;

/**
 * @brief Creates a new Window
 * 
 * @param title The title of the window
 * @param width The width of the window
 * @param height The height of the window
 * @param flags The flags of the window
 * @return Window* The created window
 */
Window* window_create(const char* title, int width, int height, int flags);
/**
 * @brief Shows a window
 * 
 * @param window The window to show
 */
void window_show(Window* window);
/**
 * @brief Hides a window
 * 
 * @param window The window to hide
 */
void window_hide(Window* window);
/**
 * @brief Focuses a window
 * 
 * @param window The window to focus
 */
void window_focus(Window* window);
/**
 * @brief Returns the main container of a window (needed for the ui)
 * 
 * @param window The window to get the main container from
 * @return UIContainer* The main container of the window
 */
UIContainer* window_get_main_container(Window* window);

/**
 * @brief Resets a window (resets the input data and sets close_requested to false)
 * 
 * @param window The window to reset
 */
void _window_reset(Window* window);
/**
 * @brief Handles an event for a window and calls the input and ui event handlers (should not be called manually)
 * 
 * @param window The window to handle the event for
 * @param event The event to handle
 */
void _window_handle_event(Window* window, SDL_Event* event);
/**
 * @brief Updates a window and updates the ui (should not be called manually)
 * 
 * @param window The window to update
 */
void _window_update(Window* window);
/**
 * @brief Renders a window and the ui (should not be called manually)
 * 
 * @param window The window to render
 */
void _window_render(Window* window);
/**
 * @brief Closes a window (closes the input and ui, destroys the renderer and the window, should not be called manually)
 * 
 * @param window The window to close
 */
void _window_close(Window* window);

#endif
