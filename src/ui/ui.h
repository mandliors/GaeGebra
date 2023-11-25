#pragma once

#include "ui_element/ui_element.h"

/**
 * @brief Holds the ui data of a window (containes the main container, the text input of the current frame, whether the backspace was pressed, whether the mouse is captured by a ui element and the expanded splitbutton)
 */
typedef struct UIData
{
	UIContainer* main_container;
	char text_input[SDL_TEXTINPUTEVENT_TEXT_SIZE];
	bool backspace_pressed;
	bool mouse_captured;
	UISplitButton* expanded_splitbutton;
} UIData;

/**
 * @brief Initializes the ui (and creates the main container)
 * 
 * @param ui_data The ui data to initialize
 * @param width The width of the window
 * @param height The height of the window
 */
void _ui_init(UIData* ui_data, int width, int height);
/**
 * @brief Handles an event that influences the ui
 * 
 * @param ui_data The ui data to change based on the event
 * @param event The event to handle
 */
void _ui_handle_event(UIData* ui_data, SDL_Event* event);
/**
 * @brief Updates the ui elements recursively
 * 
 * @param ui_data The ui data to update
 */
void _ui_update(UIData* ui_data);
/**
 * @brief Renders the ui elements recursively
 * 
 * @param ui_data The ui data to render
 */
void _ui_render(UIData* ui_data);
/**
 * @brief Closes the ui (destroys the main container)
 * 
 * @param ui_data The ui data to close
 */
void _ui_close(UIData* ui_data);
/**
 * @brief Sets the target ui data for the window
 * 
 * @param ui_data The ui data to set as target
 */
void _ui_set_target(UIData* ui_data);
/**
 * @brief Returns the target ui data for the window
 * 
 * @return UIData* The target ui data
 */
UIData* _ui_get_target();
