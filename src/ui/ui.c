#include "ui.h"
#include "../app/app.h"

void _ui_init(UIData* ui_data, int width, int height)
{
    UIConstraints constraints;
    ui_data->main_container = ui_create_container(NULL, constraints);
    ui_data->main_container->base.size = (Vector2){width, height};
    ui_data->text_input[0] = '\0';
    ui_data->backspace_pressed = false;
    SDL_StopTextInput(); //has to be called, because text input seems to be on by default (no idea why)
}
void _ui_handle_event(UIData* ui_data, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        ui_data->main_container->base.size.x = event->window.data1;
        ui_data->main_container->base.size.y = event->window.data2;
        ui_data->main_container->base.recalculate(NULL, (UIElement*)ui_data->main_container);
    }
    else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE)
        ui_data->backspace_pressed = true;
    else if (event->type == SDL_TEXTINPUT && strlen(ui_data->text_input) + strlen(event->text.text) < SDL_TEXTINPUTEVENT_TEXT_SIZE)
        strcat(ui_data->text_input, event->text.text);
}
void _ui_update(UIData* ui_data)
{
    _ui_container_update((UIElement*)ui_data->main_container);
}
void _ui_render(UIData* ui_data)
{
    _ui_container_render((UIElement*)ui_data->main_container);
}
void _ui_close(UIData* ui_data)
{
    _ui_container_destroy((UIElement*)ui_data->main_container);
}