#pragma once

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "../ui_constraint/ui_constraint.h"
#include "../../utils/vector/vector.h"
#include "../../color/color.h"
#include "../../texture/texture.h"

#include <stdbool.h>

#define UITEXT_MAX_LENGTH 50

typedef struct UIElement UIElement;
typedef void (*UIElementUpdate)(UIElement* self);
typedef void (*UIElementRecalculate)(UIElement* sibling, UIElement* self);
typedef void (*UIElementRender)(UIElement* self);
typedef void (*UIElementDestroy)(UIElement* self);

/**
 * @brief The base UI element structure (needed for polymorphism)
 */
typedef struct UIElement
{
	UIElement* parent;
	UIConstraints constraints;
	SDL_Point position;
	SDL_Point size;
	bool shown;

	UIElementUpdate update;
	UIElementRecalculate recalculate;
	UIElementRender render;
	UIElementDestroy destroy;
} UIElement;

typedef struct UIContainer UIContainer;
typedef void (*UIContainerSizeChanged)(UIContainer* self, SDL_Point size);

/**
 * @brief The UI container structure (provides a container for other UI elements)
 */
typedef struct UIContainer
{
	UIElement base;

	Vector* children;
	UIContainerSizeChanged on_size_changed;
} UIContainer;

/**
 * @brief The UI panel structure (colored panel with border)
 */
typedef struct UIPanel
{
	UIElement base;

	Color color;
	Color border_color;
	Uint32 border_width;
	Uint32 corner_radius;
} UIPanel;

/**
 * @brief The UI label structure
 */
typedef struct UILabel
{
	UIElement base;

	char text[UITEXT_MAX_LENGTH + 1];
	Color color;
} UILabel;

/**
 * @brief Mouse state enum, needed for UI Elements
 */
typedef enum MouseState { MS_NONE = 0, MS_HOVER, MS_PRESS } MouseState;
typedef struct UIButton UIButton;
typedef void (*UIButtonClick)(UIButton* self);

/**
 * @brief The UI button structure
 */
typedef struct UIButton
{
	UIElement base;

	char text[UITEXT_MAX_LENGTH + 1];
	SDL_Point text_position;
	Color color;
	Color text_color;
	Uint32 corner_radius;
	MouseState mouse_state;
	UIButtonClick on_click;
} UIButton;

typedef struct UIImageButton UIImageButton;
typedef void (*UIImageButtonClick)(UIImageButton* self);

/**
 * @brief The UI image button structure
 */
typedef struct UIImageButton
{
	UIElement base;

	Texture* texture;
	MouseState mouse_state;
	UIImageButtonClick on_click;
} UIImageButton;

typedef struct UITextbox UITextbox;
typedef void (*UITextboxTextChanged)(UITextbox* self, const char* text);

/**
 * @brief The UI textbox structure (has a fixed length)
 */
typedef struct UITextbox
{
	UIElement base;

	char text[UITEXT_MAX_LENGTH + 1];
	Color color;
	Color text_color;
	Uint32 corner_radius;
	bool focused;
	MouseState mouse_state;
	UITextboxTextChanged on_text_changed;
} UITextbox;

typedef struct UICheckbox UICheckbox;
typedef void (*UICheckboxCheckedChanged)(UICheckbox* self, bool checked);

/**
 * @brief The UI checkbox structure
 */
typedef struct UICheckbox
{
	UIElement base;

	bool checked;
	Color checked_color;
	Color unchecked_color;
	Uint32 corner_radius;
	MouseState mouse_state;
	UICheckboxCheckedChanged on_checked_changed;
} UICheckbox;

typedef struct UISlider UISlider; 
typedef void (*UISliderValueChanged)(UISlider* self, double value);

/**
 * @brief The UI slider structure
 */
typedef struct UISlider
{
	UIElement base;

	double value;
	Color color;
	Color slider_color;
	Uint32 thickness;
	Uint32 corner_radius;
	MouseState mouse_state;
	UISliderValueChanged on_value_changed;
} UISlider;

typedef struct UIDropdownList UIDropdownList;
typedef void (*UIDropdownListSelectionChanged)(UIDropdownList* self, Sint32 index);

/**
 * @brief The UI dropdown list structure
 */
typedef struct UIDropdownList
{
	UIElement base;

	Vector* items;
	Uint32 selected_item;
	bool expanded;
	Color color;
	Color text_color;
	Uint32 corner_radius;
	UIDropdownListSelectionChanged on_selection_changed;
} UIDropdownList;

typedef struct UISplitButton UISplitButton;
typedef void (*UISplitButtonClicked)(UISplitButton* self, Sint32 index);

/**
 * @brief The UI split button structure
 */
typedef struct UISplitButton
{
	UIElement base;

	Vector* items;
	bool expanded;
	Color color;
	Color text_color;
	Uint32 corner_radius;
	UISplitButtonClicked on_item_clicked;
	bool auto_dropdown;
} UISplitButton;

/**
 * @brief Creates a UIContainer
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param on_size_changed The on size changed callback (can be NULL)
 * @return UIContainer* The created container
 */
UIContainer* ui_create_container(UIContainer* parent, UIConstraints constraints, void (*on_size_changed)(UIContainer* self, SDL_Point size));
/**
 * @brief Creates a UIPanel
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param color The panel color
 * @param border_color The border color
 * @param border_width The border width
 * @param roundness The roundness
 * @return UIPanel* The created panel
 */
UIPanel* ui_create_panel(UIContainer* parent, UIConstraints constraints, Color color, Color border_color, Uint32 border_width, Uint32 roundness);
/**
 * @brief Creates a UILabel
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param text The text
 * @param color The text color
 * @return UILabel* The created label
 */
UILabel* ui_create_label(UIContainer* parent, UIConstraints constraints, const char* text, Color color);
/**
 * @brief Creates a UIButton
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param text The text
 * @param color The button color
 * @param text_color The text color
 * @param on_click The on click callback (can be NULL)
 * @return UIButton* The created button
 */
UIButton* ui_create_button(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_click)(UIButton* self));
/**
 * @brief Creates a UIImageButton
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param texture The texture
 * @param on_click The on click callback (can be NULL)
 * @return UIImageButton* The created image button
 */
UIImageButton* ui_create_imagebutton(UIContainer* parent, UIConstraints constraints, Texture* texture, void (*on_click)(UIImageButton* self));
/**
 * @brief Creates a UITextbox
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param text The text
 * @param color The textbox color
 * @param text_color The text color
 * @param on_text_changed The on text changed callback (can be NULL)
 * @return UITextbox* The created textbox
 */
UITextbox* ui_create_textbox(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_text_changed)(UITextbox* self, const char* text));
/**
 * @brief Creates a UICheckbox
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param checked_color The checked color
 * @param unchecked_color The unchecked color
 * @param on_checked_changed The on checked changed callback (can be NULL)
 * @return UICheckbox* The created checkbox
 */
UICheckbox* ui_create_checkbox(UIContainer* parent, UIConstraints constraints, Color checked_color, Color unchecked_color, void (*on_checked_changed)(UICheckbox* self, bool checked));
/**
 * @brief Creates a UISlider
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param value The value
 * @param color The color of the horizontal bar
 * @param slider_color The slider color (the vertical bar)
 * @param on_value_changed The on value changed callback (can be NULL)
 * @return UISlider* The created slider
 */
UISlider* ui_create_slider(UIContainer* parent, UIConstraints constraints, double value, Color color, Color slider_color, void (*on_value_changed)(UISlider* self, double value));
/**
 * @brief Creates a UIDropdownList
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param items The items (semicolon separated)
 * @param color The color
 * @param text_color The text color
 * @param on_selection_changed The on selection changed callback (can be NULL)
 * @return UIDropdownList* The created dropdown list
 */
UIDropdownList* ui_create_dropdown(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_selection_changed)(UIDropdownList* self, Sint32 index));
/**
 * @brief Creates a UISplitButton
 * 
 * @param parent The parent container
 * @param constraints The constraints
 * @param items The items (semicolon separated)
 * @param color The color
 * @param text_color The text color
 * @param on_item_clicked The on item clicked callback (can be NULL)
 * @param auto_dropdown Whether to automatically dropdown the list when the button is clicked
 * @return UISplitButton* The created split button
 */
UISplitButton* ui_create_splitbutton(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_item_clicked)(UISplitButton* self, Sint32 index), bool auto_dropdown);

/**
 * @brief Shows a UI element (sets the shown flag to true)
 * 
 * @param self The UI element to show
 */
void ui_show_element(UIElement* self);
/**
 * @brief Hides a UI element (sets the shown flag to false)
 * 
 * @param self The UI element to hide
 */
void ui_hide_element(UIElement* self);

/**
 * @brief Updates a UIContainer and its children recursively
 * 
 * @param self The UIContainer to update
 */
void _ui_container_update(UIElement* self);
/**
 * @brief Recalculates the position and size of a UIContainer and its children
 * 
 * @param sibling The sibling of the UIContainer (used for calculating the position in case of offset constraints)
 * @param self The UIContainer to recalculate
 */
void _ui_container_recalculate(UIElement* sibling, UIElement* self);
/**
 * @brief Renders a UIContainer and its children recursively
 * 
 * @param self The UIContainer to render
 */
void _ui_container_render(UIElement* self);
/**
 * @brief Destroys a UIContainer and its children recursively
 * 
 * @param self The UIContainer to destroy
 */
void _ui_container_destroy(UIElement* self);