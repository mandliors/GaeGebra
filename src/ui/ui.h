#ifndef UI_H
#define UI_H

#include "ui_elements/ui_elements.h"

typedef struct UIData
{
	UIContainer* main_container;
} UIData;

//internal functions
void _ui_init(UIData* ui_data, int width, int height);
void _ui_handle_event(UIData* ui_data, SDL_Event* event);
void _ui_update(UIData* ui_data);
void _ui_render(UIData* ui_data);
void _ui_close(UIData* ui_data);

#endif