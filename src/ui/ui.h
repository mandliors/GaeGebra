#pragma once

#include "ui_element/ui_element.h"

typedef struct UIData
{
	UIContainer* main_container;
	char text_input[SDL_TEXTINPUTEVENT_TEXT_SIZE];
	bool backspace_pressed;
	bool mouse_captured;
	UISplitButton* expanded_splitbutton;
} UIData;

//internal functions
void _ui_init(UIData* ui_data, int width, int height);
void _ui_handle_event(UIData* ui_data, SDL_Event* event);
void _ui_update(UIData* ui_data);
void _ui_render(UIData* ui_data);
void _ui_close(UIData* ui_data);
void _ui_set_target(UIData* ui_data);
UIData* _ui_get_target();
