#include <assert.h>

#include "input.h"
#include "../window/window.h"
#include "../app/app.h"

bool input_is_mouse_button_down(int button)
{
    assert(app_get_active_window()); //there is no window
    return app_get_active_window()->input_data.current_mouse_button_state[button];
}
bool input_is_mouse_button_pressed(int button)
{
    assert(app_get_active_window()); //there is no window
    return app_get_active_window()->input_data.current_mouse_button_state[button] && !app_get_active_window()->input_data.old_mouse_button_state[button];
}
bool input_is_mouse_button_released(int button)
{
    assert(app_get_active_window()); //there is no window
    return !app_get_active_window()->input_data.current_mouse_button_state[button] && app_get_active_window()->input_data.old_mouse_button_state[button];
}

bool input_is_key_down(int key)
{
    assert(app_get_active_window()); //there is no window
    return app_get_active_window()->input_data.current_keyboard_state[key];
}
bool input_is_key_pressed(int key)
{
    assert(app_get_active_window()); //there is no window
    return app_get_active_window()->input_data.current_keyboard_state[key] && !app_get_active_window()->input_data.old_keyboard_state[key];
}
bool input_is_key_released(int key)
{
    assert(app_get_active_window()); //there is no window
    return !app_get_active_window()->input_data.current_keyboard_state[key] && app_get_active_window()->input_data.old_keyboard_state[key];
}

SDL_Point input_get_mouse_position()
{
    assert(app_get_active_window()); //there is no window
    return app_get_active_window()->input_data.mouse_position;
}
int input_get_mouse_wheel_delta()
{
    assert(app_get_active_window()); //there is no window
    return app_get_active_window()->input_data.mouse_wheel_delta;
}

void _input_init(InputData* input_data)
{
    for (int i = 0; i < 5; i++)
    {
        input_data->current_mouse_button_state[i] = false;
        input_data->old_mouse_button_state[i] = false;
    }
    SDL_GetMouseState(&input_data->mouse_position.x, &input_data->mouse_position.y);
    input_data->mouse_wheel_delta = 0;

    input_data->current_keyboard_state = (Uint8*)SDL_GetKeyboardState((int*)&input_data->key_count);
    input_data->old_keyboard_state = (Uint8*)malloc(input_data->key_count * sizeof(Uint8));
    if (input_data->old_keyboard_state == NULL)
    {
        SDL_Log("could not allocate memory");
        exit(1);
    }
    assert(input_data->old_keyboard_state); //couldn't allocate memory
}
void _input_handle_event(InputData* input_data, SDL_Event* event)
{
    assert(input_data); //there is no window
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
        input_data->mouse_wheel_delta = 0;
        memcpy(input_data->old_mouse_button_state, input_data->current_mouse_button_state, 5);
        memcpy(input_data->old_keyboard_state, input_data->current_keyboard_state, input_data->key_count);
}
void _input_close(InputData* input_data)
{
    free(input_data->old_keyboard_state);
}