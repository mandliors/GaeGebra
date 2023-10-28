#ifndef UI_WIDGETS_H
#define UI_WIDGETS_H

#include "../ui_constraints/ui_constraints.h"
#include "../../utils/vector/vector.h"
#include "../../colors/colors.h"

#include <stdbool.h>

typedef struct UIElement UIElement;
typedef struct UIElement
{
	UIElement* parent;
	UIConstraints constraints;
	Vector2 position;
	Vector2 size;

	void (*update)(UIElement* self);
	void (*recalculate)(UIElement* self);
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
	int border_width;
	int corner_radius;
} UIPanel;

typedef struct UILabel
{
	UIElement base;

	char* text;
	Color color;
} UILabel;

typedef enum MouseState { MS_NONE = 0, MS_HOVER, MS_PRESS } MouseState;
typedef struct UIButton UIButton;
typedef struct UIButton
{
	UIElement base;

	char* text;
	Vector2 text_position;
	MouseState mouse_state;
	Color color;
	Color text_color;
	int corner_radius;
	void (*on_click)(UIButton* self);
} UIButton;

typedef struct UICheckbox
{
	UIElement base;

	bool checked;
	Color checked_color;
	Color unchecked_color;
	Color border_color;
	int corner_radius;
	MouseState mouse_state;
} UICheckbox;

typedef struct UISlider
{
	UIElement base;

	double value;
	Color color;
	Color slider_color;
	int thickness;
	int corner_radius;
	MouseState mouse_state;
} UISlider;

//API functions
UIContainer* ui_create_container(UIContainer* parent, UIConstraints constraints);
UIPanel* ui_create_panel(UIContainer* parent, UIConstraints constraints, Color color, Color border_color, int border_width, int roundness);
UILabel* ui_create_label(UIContainer* parent, UIConstraints constraints, const char* text, Color color);
UIButton* ui_create_button(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_click)(UIButton* self));
UICheckbox* ui_create_checkbox(UIContainer* parent, UIConstraints constraints, Color checked_color, Color unchecked_color);
UISlider* ui_create_slider(UIContainer* parent, UIConstraints constraints, double value, Color color, Color slider_color);

//internal functions
void _ui_container_update(UIElement* self);
void _ui_container_recalculate(UIElement* self);
void _ui_container_render(UIElement* self);
void _ui_container_destroy(UIElement* self);

void _ui_panel_update(UIElement* self);
void _ui_panel_recalculate(UIElement* self);
void _ui_panel_render(UIElement* self);
void _ui_panel_destroy(UIElement* self);

void _ui_label_update(UIElement* self);
void _ui_label_recalculate(UIElement* self);
void _ui_label_render(UIElement* self);
void _ui_label_destroy(UIElement* self);

void _ui_button_update(UIElement* self);
void _ui_button_recalculate(UIElement* self);
void _ui_button_render(UIElement* self);
void _ui_button_destroy(UIElement* button);

void _ui_checkbox_update(UIElement* self);
void _ui_checkbox_recalculate(UIElement* self);
void _ui_checkbox_render(UIElement* self);
void _ui_checkbox_destroy(UIElement* self);

void _ui_slider_update(UIElement* self);
void _ui_slider_recalculate(UIElement* self);
void _ui_slider_render(UIElement* self);
void _ui_slider_destroy(UIElement* self);

void __ui_element_recalculate(UIElement* element);
int __ui_calculate_size(UIConstraint* constraint, int parent_size);
int __ui_calculate_position(UIConstraint* constraint, int parent_position, int parent_size, int size);

#endif