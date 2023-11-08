#include "input.h"
#include "../app/app.h"

InputData* target_input_data;

bool input_is_mouse_button_down(int button)
{
    return target_input_data->current_mouse_button_state[button];
}
bool input_is_mouse_button_pressed(int button)
{
    return target_input_data->current_mouse_button_state[button] && !target_input_data->old_mouse_button_state[button];
}
bool input_is_mouse_button_released(int button)
{
    return !target_input_data->current_mouse_button_state[button] && target_input_data->old_mouse_button_state[button];
}

bool input_is_key_down(int key)
{
    return target_input_data->current_keyboard_state[key];
}
bool input_is_key_pressed(int key)
{
    return target_input_data->current_keyboard_state[key] && !target_input_data->old_keyboard_state[key];
}
bool input_is_key_released(int key)
{
    return !target_input_data->current_keyboard_state[key] && target_input_data->old_keyboard_state[key];
}

SDL_Point input_get_mouse_position()
{
    return target_input_data->mouse_position;
}
SDL_Point input_get_mouse_motion()
{
    return (SDL_Point) { target_input_data->mouse_position.x - target_input_data->old_mouse_position.x,
                         target_input_data->mouse_position.y - target_input_data->old_mouse_position.y };
}
int input_get_mouse_wheel_delta()
{
    return target_input_data->mouse_wheel_delta;
}

void _input_init(InputData* input_data)
{
    for (int i = 0; i < 5; i++)
    {
        input_data->current_mouse_button_state[i] = false;
        input_data->old_mouse_button_state[i] = false;
    }
    SDL_GetMouseState(&input_data->mouse_position.x, &input_data->mouse_position.y);
    input_data->old_mouse_position = input_data->mouse_position;
    input_data->mouse_wheel_delta = 0;

    input_data->current_keyboard_state = (Uint8*)SDL_GetKeyboardState((int*)&input_data->key_count);
    input_data->old_keyboard_state = (Uint8*)malloc(input_data->key_count * sizeof(Uint8));
    if (input_data->old_keyboard_state == NULL)
    {
        SDL_Log("couldn't allocate memory");
        exit(1);
    }
}
void _input_handle_event(InputData* input_data, SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        input_data->current_mouse_button_state[event->button.button] = true;
        break;
    case SDL_MOUSEBUTTONUP:
        input_data->current_mouse_button_state[event->button.button] = false;
        break;
    case SDL_MOUSEWHEEL:
        input_data->mouse_wheel_delta = event->wheel.y;
        break;
    case SDL_MOUSEMOTION:
        SDL_GetMouseState(&input_data->mouse_position.x, &input_data->mouse_position.y);
        break;
    }
}
void _input_reset(InputData* input_data)
{
    memcpy(input_data->old_mouse_button_state, input_data->current_mouse_button_state, 5);
    memcpy(input_data->old_keyboard_state, input_data->current_keyboard_state, input_data->key_count);
    input_data->old_mouse_position = input_data->mouse_position;
    input_data->mouse_wheel_delta = 0;
}
void _input_close(InputData* input_data)
{
    free(input_data->old_keyboard_state);
}

void _input_set_target(InputData* input_data)
{
    target_input_data = input_data;
}
