#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include <stdbool.h>

/**
 * @brief Holds the input data of a window (needed for press, hold, release events and mouse motion)
 */
typedef struct InputData
{
    //mouse
    bool current_mouse_button_state[5];
    bool old_mouse_button_state[5];
    SDL_Point current_mouse_position;
    SDL_Point old_mouse_position;
    int mouse_wheel_delta;

    //keyboard
    Uint8* current_keyboard_state;
    Uint8* old_keyboard_state;
    int key_count;
} InputData;

/**
 * @brief Returns if the mouse button is down (being held)
 * 
 * @param button The mouse button to check
 * @return true Returns true if the mouse button is held
 * @return false Returns false if the mouse button is held
 */
bool input_is_mouse_button_down(int button);
/**
 * @brief Returns if the mouse button has just been pressed
 * 
 * @param button The mouse button to check
 * @return true Returns true if the mouse button has just been pressed
 * @return false Returns false if the mouse button has not been pressed
 */
bool input_is_mouse_button_pressed(int button);
/**
 * @brief Returns if the mouse button has just been released
 * 
 * @param button The mouse button to check
 * @return true Returns true if the mouse button has just been released
 * @return false Returns false if the mouse button has not been released
 */
bool input_is_mouse_button_released(int button);

/**
 * @brief Returns if the key is down (being held)
 * 
 * @param key The key to check
 * @return true Returns true if the key is held
 * @return false Returns false if the key is not held
 */
bool input_is_key_down(int key);
/**
 * @brief Returns if the key has just been pressed
 * 
 * @param key The key to check
 * @return true Returns true if the key has just been pressed
 * @return false Returns false if the key has not been pressed
 */
bool input_is_key_pressed(int key);
/**
 * @brief Returns if the key has just been released
 * 
 * @param key The key to check
 * @return true Returns true if the key has just been released
 * @return false Returns false if the key has not been released
 */
bool input_is_key_released(int key);

/**
 * @brief Returns the mouse position
 * 
 * @return SDL_Point The mouse position
 */
SDL_Point input_get_mouse_position();
/**
 * @brief Returns the mouse motion since the last frame
 * 
 * @return SDL_Point The mouse motion
 */
SDL_Point input_get_mouse_motion();
/**
 * @brief Returns the mouse wheel delta since the last frame
 * 
 * @return int The mouse wheel delta
 */
int input_get_mouse_wheel_delta();

/**
 * @brief Initializes the input data (should not be called manually)
 * 
 * @param input_data The input data to initialize
 */
void _input_init(InputData* input_data);
/**
 * @brief Handles an input event (should not be called manually)
 * 
 * @param input_data The input data to change based on the event
 * @param event The event to handle
 */
void _input_handle_event(InputData* input_data, SDL_Event* event);
/**
 * @brief Resets the input data (should not be called manually)
 * 
 * @param input_data The input data to reset
 */
void _input_reset(InputData* input_data);
/**
 * @brief Frees input data resources (should not be called manually)
 * 
 * @param input_data The input data to close
 */
void _input_close(InputData* input_data);
/**
 * @brief Sets the target input data for the application (should not be called manually)
 * 
 * @param input_data The input data to set as target
 */
void _input_set_target(InputData* input_data);
