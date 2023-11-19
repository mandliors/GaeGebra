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

typedef struct UIContainer
{
	UIElement base;

	Vector* children;
	UIContainerSizeChanged on_size_changed;
} UIContainer;

typedef struct UIPanel
{
	UIElement base;

	Color color;
	Color border_color;
	Uint32 border_width;
	Uint32 corner_radius;
} UIPanel;

typedef struct UILabel
{
	UIElement base;

	char text[UITEXT_MAX_LENGTH + 1];
	Color color;
} UILabel;

typedef enum MouseState { MS_NONE = 0, MS_HOVER, MS_PRESS } MouseState;
typedef struct UIButton UIButton;
typedef void (*UIButtonClick)(UIButton* self);

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

typedef struct UIImageButton
{
	UIElement base;

	Texture* texture;
	MouseState mouse_state;
	UIImageButtonClick on_click;
} UIImageButton;

typedef struct UITextbox UITextbox;
typedef void (*UITextboxTextChanged)(UITextbox* self, const char* text);

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

//API functions
UIContainer* ui_create_container(UIContainer* parent, UIConstraints constraints, void (*on_size_changed)(UIContainer* self, SDL_Point size));
UIPanel* ui_create_panel(UIContainer* parent, UIConstraints constraints, Color color, Color border_color, Uint32 border_width, Uint32 roundness);
UILabel* ui_create_label(UIContainer* parent, UIConstraints constraints, const char* text, Color color);
UIButton* ui_create_button(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_click)(UIButton* self));
UIImageButton* ui_create_imagebutton(UIContainer* parent, UIConstraints constraints, Texture* texture, void (*on_click)(UIImageButton* self));
UITextbox* ui_create_textbox(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_text_changed)(UITextbox* self, const char* text));
UICheckbox* ui_create_checkbox(UIContainer* parent, UIConstraints constraints, Color checked_color, Color unchecked_color, void (*on_checked_changed)(UICheckbox* self, bool checked));
UISlider* ui_create_slider(UIContainer* parent, UIConstraints constraints, double value, Color color, Color slider_color, void (*on_value_changed)(UISlider* self, double value));
UIDropdownList* ui_create_dropdown(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_selection_changed)(UIDropdownList* self, Sint32 index));
UISplitButton* ui_create_splitbutton(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_item_clicked)(UISplitButton* self, Sint32 index), bool auto_dropdown);

void ui_show_element(UIElement* self);
void ui_hide_element(UIElement* self);

//internal functions
void _ui_container_update(UIElement* self);
void _ui_container_recalculate(UIElement* sibling, UIElement* self);
void _ui_container_render(UIElement* self);
void _ui_container_destroy(UIElement* self);