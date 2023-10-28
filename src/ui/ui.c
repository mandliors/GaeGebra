#include "ui.h"
#include "../app/app.h"

#include "../debugmalloc.h"

void _ui_init(UIData* ui_data, int width, int height)
{
    UIConstraints constraints = {NULL, NULL, NULL, NULL};
    ui_data->main_container = ui_create_container(NULL, constraints);
    ui_data->main_container->base.size = (Vector2){width, height};
}
void _ui_handle_event(UIData* ui_data, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        ui_data->main_container->base.size.x = event->window.data1;
        ui_data->main_container->base.size.y = event->window.data2;
        ui_data->main_container->base.recalculate((UIElement*)ui_data->main_container);
    }
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