#ifndef INPUT_H
#define INPUT_H

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
bool is_mouse_button_down(int button);
bool is_mouse_button_pressed(int button);
bool is_mouse_button_released(int button);

bool is_key_down(int key);
bool is_key_pressed(int key);
bool is_key_released(int key);

SDL_Point get_mouse_position();
int get_mouse_wheel_delta();

//internal functions
void _input_init(InputData* input_data);
void _input_handle_event(InputData* input_data, SDL_Event* event);
void _input_reset(InputData* input_data);
void _input_close(InputData* input_data);

#endif
