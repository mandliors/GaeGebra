#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include <stdbool.h>

typedef struct InputData
{
    //mouse
    bool current_mouse_button_state[5];
    bool old_mouse_button_state[5];
    SDL_Point mouse_position;
    int mouse_wheel_delta;

    //keyboard
    Uint8* current_keyboard_state;
    Uint8* old_keyboard_state;
    int key_count;
} InputData;

//API functions
bool input_is_mouse_button_down(int button);
bool input_is_mouse_button_pressed(int button);
bool input_is_mouse_button_released(int button);

bool input_is_key_down(int key);
bool input_is_key_pressed(int key);
bool input_is_key_released(int key);

SDL_Point input_get_mouse_position();
int input_get_mouse_wheel_delta();

//internal functions
void _input_init(InputData* input_data);
void _input_handle_event(InputData* input_data, SDL_Event* event);
void _input_reset(InputData* input_data);
void _input_close(InputData* input_data);
void _input_set_target(InputData* input_data);
