#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "../ui_constraint/ui_constraint.h"
#include "../../utils/vector/vector.h"
#include "../../colors/colors.h"
#include "../../texture/texture.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#define UITEXT_MAX_LENGTH 50

typedef struct UIElement UIElement;
typedef struct UIElement
{
	UIElement* parent;
	UIConstraints constraints;
	Vector2 position;
	Vector2 size;

	void (*update)(UIElement* self);
	void (*recalculate)(UIElement* sibling, UIElement* self);
	void (*render)(UIElement* self);
	void (*destroy)(UIElement* self);
} UIElement;

typedef struct UIContainer
{
	UIElement base;

	Vector* children;
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
typedef struct UIButton
{
	UIElement base;

	char text[UITEXT_MAX_LENGTH + 1];
	Vector2 text_position;
	Color color;
	Color text_color;
	Uint32 corner_radius;
	MouseState mouse_state;
	void (*on_click)(UIButton* self);
} UIButton;

typedef struct UIImageButton UIImageButton;
typedef struct UIImageButton
{
	UIElement base;

	Texture* texture;
	MouseState mouse_state;
	void (*on_click)(UIImageButton* self);
} UIImageButton;

typedef struct UITextbox UITextbox;
typedef struct UITextbox
{
	UIElement base;

	char text[UITEXT_MAX_LENGTH + 1];
	Color color;
	Color text_color;
	Uint32 corner_radius;
	bool focused;
	MouseState mouse_state;
	void (*on_text_changed)(UITextbox* self, const char* text);
} UITextbox;

typedef struct UICheckbox UICheckbox;
typedef struct UICheckbox
{
	UIElement base;

	bool checked;
	Color checked_color;
	Color unchecked_color;
	Uint32 corner_radius;
	MouseState mouse_state;
	void (*on_checked_changed)(UICheckbox* self, bool checked);
} UICheckbox;

typedef struct UISlider UISlider; 
typedef struct UISlider
{
	UIElement base;

	double value;
	Color color;
	Color slider_color;
	Uint32 thickness;
	Uint32 corner_radius;
	MouseState mouse_state;
	void (*on_value_changed)(UISlider* self, double value);
} UISlider;

typedef struct UIDropdownList UIDropdownList;
typedef struct _UIDropdownItem _UIDropdownItem;
typedef struct _UIDropdownItem
{
	UIElement base;

	UIDropdownList* parent_dropdown;
	Sint32 dropdown_index;
	char text[UITEXT_MAX_LENGTH + 1];
	MouseState mouse_state;
	void (*on_click)(_UIDropdownItem* self);         
} _UIDropdownItem;

typedef struct UIDropdownList
{
	UIElement base;

	Vector* items;
	Uint32 selected_item;
	bool expanded;
	Color color;
	Color text_color;
	Uint32 corner_radius;
	void (*on_selection_changed)(UIDropdownList* self, Sint32 index);
} UIDropdownList;

//API functions
UIContainer* ui_create_container(UIContainer* parent, UIConstraints constraints);
UIPanel* ui_create_panel(UIContainer* parent, UIConstraints constraints, Color color, Color border_color, Uint32 border_width, Uint32 roundness);
UILabel* ui_create_label(UIContainer* parent, UIConstraints constraints, const char* text, Color color);
UIButton* ui_create_button(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_click)(UIButton* self));
UIImageButton* ui_create_imagebutton(UIContainer* parent, UIConstraints constraints, Texture* texture, void (*on_click)(UIImageButton* self));
UITextbox* ui_create_textbox(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_text_changed)(UITextbox* self, const char* text));
UICheckbox* ui_create_checkbox(UIContainer* parent, UIConstraints constraints, Color checked_color, Color unchecked_color, void (*on_checked_changed)(UICheckbox* self, bool checked));
UISlider* ui_create_slider(UIContainer* parent, UIConstraints constraints, double value, Color color, Color slider_color, void (*on_value_changed)(UISlider* self, double value));
UIDropdownList* ui_create_dropdown(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_selection_changed)(UIDropdownList* self, Sint32 index));

//internal functions
void _ui_container_update(UIElement* self);
void _ui_container_recalculate(UIElement* sibling, UIElement* self);
void _ui_container_render(UIElement* self);
void _ui_container_destroy(UIElement* self);

void _ui_panel_update(UIElement* self);
void _ui_panel_recalculate(UIElement* sibling, UIElement* self);
void _ui_panel_render(UIElement* self);
void _ui_panel_destroy(UIElement* self);

void _ui_label_update(UIElement* self);
void _ui_label_recalculate(UIElement* sibling, UIElement* self);
void _ui_label_render(UIElement* self);
void _ui_label_destroy(UIElement* self);

void _ui_button_update(UIElement* self);
void _ui_button_recalculate(UIElement* sibling, UIElement* self);
void _ui_button_render(UIElement* self);
void _ui_button_destroy(UIElement* self);

void _ui_imagebutton_update(UIElement* self);
void _ui_imagebutton_recalculate(UIElement* sibling, UIElement* self);
void _ui_imagebutton_render(UIElement* self);
void _ui_imagebutton_destroy(UIElement* self);

void _ui_textbox_update(UIElement* self);
void _ui_textbox_recalculate(UIElement* sibling, UIElement* self);
void _ui_textbox_render(UIElement* self);
void _ui_textbox_destroy(UIElement* self);

void _ui_checkbox_update(UIElement* self);
void _ui_checkbox_recalculate(UIElement* sibling, UIElement* self);
void _ui_checkbox_render(UIElement* self);
void _ui_checkbox_destroy(UIElement* self);

void _ui_slider_update(UIElement* self);
void _ui_slider_recalculate(UIElement* sibling, UIElement* self);
void _ui_slider_render(UIElement* self);
void _ui_slider_destroy(UIElement* self);

_UIDropdownItem* _ui_dropdownitem_create(UIDropdownList* parent, UIConstraints constraints, Sint32 index, const char* text, void (*on_click)(_UIDropdownItem* self));
void _ui_dropdownitem_update(UIElement* self);
void _ui_dropdownitem_recalculate(UIElement* sibling, UIElement* self);
void _ui_dropdownitem_render(UIElement* self);
void _ui_dropdownitem_destroy(UIElement* self);
void _ui_dropdownitem_on_click(_UIDropdownItem* self);

void _ui_dropdown_update(UIElement* self);
void _ui_dropdown_recalculate(UIElement* sibling, UIElement* self);
void _ui_dropdown_render(UIElement* self);
void _ui_dropdown_destroy(UIElement* self);

void __ui_element_recalculate(UIElement* sibling, UIElement* element);
int __ui_calculate_size(UIConstraint* constraint, int parent_size);
int __ui_calculate_position(UIConstraint* constraint, int sibling_position, int sibling_size, int parent_position, int parent_size, int size);

#endif