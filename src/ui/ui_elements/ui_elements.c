#include "ui_elements.h"
#include "../../renderer/renderer.h"
#include "../../input/input.h"
#include "../../utils/math/math.h"

#include "../../debugmalloc.h"

#include <math.h>

UIContainer* ui_create_container(UIContainer* parent, UIConstraints constraints)
{
    UIContainer* container = (UIContainer*)malloc(sizeof(UIContainer));
    UIElement* element = (UIElement*)container;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_container_update;
    element->recalculate = _ui_container_recalculate;
    element->render = _ui_container_render;
    element->destroy = _ui_container_destroy;

    container->children = vector_create(0);
    if (parent)
        vector_push_back(parent->children, element);
    container->base.recalculate(element);

    return container;
}

UIPanel* ui_create_panel(UIContainer* parent, UIConstraints constraints, Color color, Color border_color, int border_width, int roundness)
{
    UIPanel* panel = (UIPanel*)malloc(sizeof(UIPanel));
    UIElement* element = (UIElement*)panel;

    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_panel_update;
    element->recalculate = _ui_panel_recalculate;
    element->render = _ui_panel_render;
    element->destroy = _ui_panel_destroy;

    panel->color = color;
    panel->border_color = border_color;
    panel->border_width = border_width;
    panel->corner_radius = roundness;

    if (parent)
        vector_push_back(parent->children, element);
    panel->base.recalculate(element);

    return panel;
}
UILabel* ui_create_label(UIContainer* parent, UIConstraints constraints, const char* text, Color color)
{
    UILabel* label = (UILabel*)malloc(sizeof(UILabel));
    UIElement* element = (UIElement*)label;

    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_label_update;
    element->recalculate = _ui_label_recalculate;
    element->render = _ui_label_render;
    element->destroy = _ui_label_destroy;

    label->text = malloc(strlen(text) + 1);
    strcpy(label->text, text);
    label->color = color;

    if (parent)
        vector_push_back(parent->children, element);
    label->base.recalculate(element);

    return label;
}
UIButton* ui_create_button(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_click)(UIButton* self))
{
    UIButton* button = (UIButton*)malloc(sizeof(UIButton));
    UIElement* element = (UIElement*)button;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_button_update;
    element->recalculate = _ui_button_recalculate;
    element->render = _ui_button_render;
    element->destroy = _ui_button_destroy;

    button->color = color;
    button->text_color = text_color;
    button->corner_radius = 2;
    button->text = malloc(strlen(text) + 1);
    strcpy(button->text, text);
    button->mouse_state = MS_NONE;
    button->on_click = on_click;

    if (parent)
        vector_push_back(parent->children, element);
    button->base.recalculate(element);
    
    return button;
}
UICheckbox* ui_create_checkbox(UIContainer* parent, UIConstraints constraints, Color checked_color, Color unchecked_color)
{
    UICheckbox* checkbox = (UICheckbox*)malloc(sizeof(UICheckbox));
    UIElement* element = (UIElement*)checkbox;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_checkbox_update;
    element->recalculate = _ui_checkbox_recalculate;
    element->render = _ui_checkbox_render;
    element->destroy = _ui_checkbox_destroy;

    checkbox->checked = false;
    checkbox->checked_color = checked_color;
    checkbox->unchecked_color = unchecked_color;
    checkbox->border_color = (Color){(checkbox->unchecked_color.r + 128) % 256, (checkbox->unchecked_color.g + 128) % 256, (checkbox->unchecked_color.b + 128) % 256, 255};
    checkbox->corner_radius = 2;

    if (parent)
        vector_push_back(parent->children, element);
    checkbox->base.recalculate(element);
    
    return checkbox;
}
UISlider* ui_create_slider(UIContainer* parent, UIConstraints constraints, double value, Color color, Color slider_color)
{
    UISlider* slider = (UISlider*)malloc(sizeof(UISlider));
    UIElement* element = (UIElement*)slider;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_slider_update;
    element->recalculate = _ui_slider_recalculate;
    element->render = _ui_slider_render;
    element->destroy = _ui_slider_destroy;

    slider->value = value;
    slider->color = color;
    slider->slider_color = slider_color;
    slider->thickness = 7;
    slider->corner_radius = 1;
    slider->mouse_state = MS_NONE;

    if (parent)
        vector_push_back(parent->children, element);
    slider->base.recalculate(element);
    
    return slider;
}

void _ui_container_update(UIElement* self)
{
    UIContainer* container = (UIContainer*)self;
    for (size_t i = 0; i < container->children->size; i++)
    {
        UIElement* child = (UIElement*)vector_get(container->children, i);
        child->update(child);
    }
}
void _ui_container_recalculate(UIElement* self)
{
    if (self->parent != NULL)
    {
        //recalculate size
        self->size.x = __ui_calculate_size(self->constraints.width, self->parent->size.x);
        self->size.y = __ui_calculate_size(self->constraints.height, self->parent->size.y);
        if (self->constraints.width->constraint_type == CT_ASPECT)
            self->size.x = self->constraints.width->value * self->size.y;
        else if (self->constraints.height->constraint_type == CT_ASPECT)
            self->size.y = self->constraints.height->value * self->size.x;
        if (self->size.x < 0 || self->size.y < 0)
            SDL_Log("invalid constraint type for container size");

        //recalculate position
        self->position.x = __ui_calculate_position (self->constraints.x,
                                                    self->parent->position.x,
                                                    self->parent->size.x,
                                                    self->size.x);
        self->position.y = __ui_calculate_position (self->constraints.y,
                                                    self->parent->position.y,
                                                    self->parent->size.y,
                                                    self->size.y);
        if (self->position.x < 0 || self->position.y < 0)
            SDL_Log("invalid constraint type for container position");
    }
    UIContainer* container = (UIContainer*)self;
    for (size_t i = 0; i < container->children->size; i++)
    {
        UIElement* child = (UIElement*)vector_get(container->children, i);
        child->recalculate(child);
    }
}
void _ui_container_render(UIElement* self)
{
    UIContainer* container = (UIContainer*)self;
    for (size_t i = 0; i < container->children->size; i++)
    {
        UIElement* child = (UIElement*)vector_get(container->children, i);
        child->render(child);
    }
}
void _ui_container_destroy(UIElement* self)
{
    _ui_constraints_free(&self->constraints);
    UIContainer* container = (UIContainer*)self;
    for (size_t i = 0; i < container->children->size; i++)
    {
        UIElement* child = (UIElement*)vector_get(container->children, i);
        child->destroy(child);
    }
    vector_free(container->children);
    free(container);
}

void _ui_panel_update(UIElement* self) { }
void _ui_panel_recalculate(UIElement* self)
{
    __ui_element_recalculate(self);
}
void _ui_panel_render(UIElement* self)
{
    UIPanel* panel = (UIPanel*)self;
    draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, panel->corner_radius, panel->color);
    for (int i = 0; i < panel->border_width; i++)
        draw_rounded_rect(self->position.x + i, self->position.y + i, self->size.x - 2 * i, self->size.y - 2 * i, panel->corner_radius, panel->border_color);
}
void _ui_panel_destroy(UIElement* self)
{
    _ui_constraints_free(&self->constraints);
    UIPanel* panel = (UIPanel*)self;
    free(panel);
}

void _ui_label_update(UIElement* self) { }
void _ui_label_recalculate(UIElement* self)
{
    //recalculate size
    UILabel* label = (UILabel*)self;
    self->size = query_text_size(label->text);

    //recalculate position
    self->position.x = __ui_calculate_position (self->constraints.x,
                                                self->parent->position.x,
                                                self->parent->size.x,
                                                self->size.x);
    self->position.y = __ui_calculate_position (self->constraints.y,
                                                self->parent->position.y,
                                                self->parent->size.y,
                                                self->size.y);
    if (self->position.x < 0 || self->position.y < 0)
        SDL_Log("invalid constraint type for ui element");
}
void _ui_label_render(UIElement* self)
{
    UILabel* label = (UILabel*)self;
    draw_text(label->text, self->position.x, self->position.y, label->color);
}
void _ui_label_destroy(UIElement* self)
{
    _ui_constraints_free(&self->constraints);
    UILabel* label = (UILabel*)self;
    free(label->text);
    free(label);
}

void _ui_button_update(UIElement* self)
{
    UIButton* button = (UIButton*)self;
    if (check_collision_point_rect(get_mouse_position().x, get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (is_mouse_button_pressed(SDL_BUTTON_LEFT))
            button->mouse_state = MS_PRESS;
        else if (is_mouse_button_released(SDL_BUTTON_LEFT) && button->mouse_state == MS_PRESS)
        {
            if (button->on_click)
                button->on_click(button);
            button->mouse_state = MS_HOVER;
        }
        else if (button->mouse_state == MS_NONE)
            button->mouse_state = MS_HOVER;
    }
    else if (button->mouse_state != MS_PRESS)
        button->mouse_state = MS_NONE;
    else if (is_mouse_button_released(SDL_BUTTON_LEFT))
        button->mouse_state = MS_NONE;
}
void _ui_button_recalculate(UIElement* self)
{
    __ui_element_recalculate(self);
    
    UIButton* button = (UIButton*)self;
    Vector2 text_size = query_text_size(button->text);
    button->text_position = (Vector2){self->position.x + (self->size.x - text_size.x) / 2,
                                      self->position.y + (self->size.y - text_size.y) / 2};
}
void _ui_button_render(UIElement* self)
{
    UIButton* button = (UIButton*)self;
    Color color = color_shift(button->color, button->mouse_state == MS_PRESS ? 15 : (button->mouse_state == MS_HOVER ? 10 : 0));
    draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, button->corner_radius, color);
    if (strcmp(button->text, "") != 0)
        draw_text(button->text, button->text_position.x, button->text_position.y, button->text_color);
}
void _ui_button_destroy(UIElement* self)
{
    _ui_constraints_free(&self->constraints);
    UIButton* button = (UIButton*)self;
    free(button->text);
    free(button);
}

void _ui_checkbox_update(UIElement* self)
{
    UICheckbox* checkbox = (UICheckbox*)self;
    if (check_collision_point_rect(get_mouse_position().x, get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (is_mouse_button_pressed(SDL_BUTTON_LEFT))
            checkbox->mouse_state = MS_PRESS;
        else if (is_mouse_button_released(SDL_BUTTON_LEFT) && checkbox->mouse_state == MS_PRESS)
        {
            checkbox->checked = !checkbox->checked;
            checkbox->mouse_state = MS_HOVER;
        }
        else if (checkbox->mouse_state == MS_NONE)
            checkbox->mouse_state = MS_HOVER;
    }
    else if (checkbox->mouse_state != MS_PRESS)
        checkbox->mouse_state = MS_NONE;
    else if (is_mouse_button_released(SDL_BUTTON_LEFT))
        checkbox->mouse_state = MS_NONE;
}
void _ui_checkbox_recalculate(UIElement* self)
{
    __ui_element_recalculate(self);
}
void _ui_checkbox_render(UIElement* self)
{
    UICheckbox* checkbox = (UICheckbox*)self;
    if (checkbox->checked)
    {
        draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, checkbox->checked_color);
        Vector2 p1 = (Vector2){self->position.x + self->size.x * 0.25, self->position.y + self->size.y * 0.5};
        Vector2 p2 = (Vector2){self->position.x + self->size.x * 0.45, self->position.y + self->size.y * 0.75};
        Vector2 p3 = (Vector2){self->position.x + self->size.x * 0.8, self->position.y + self->size.y * 0.25};
        draw_line(p1.x, p1.y, p2.x, p2.y, 2, checkbox->unchecked_color);
        draw_line(p2.x, p2.y, p3.x, p3.y, 2, checkbox->unchecked_color);
    }
    else
    {
        if (checkbox->mouse_state == MS_NONE)
            draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, checkbox->border_color);
        else if (checkbox->mouse_state == MS_HOVER)
            draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, checkbox->checked_color);
        else if (checkbox->mouse_state == MS_PRESS)
            draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, color_shift(checkbox->checked_color, 4));
        draw_filled_rounded_rect(self->position.x + 2, self->position.y + 2, self->size.x - 4, self->size.y - 4, checkbox->corner_radius, checkbox->unchecked_color);
    }
}
void _ui_checkbox_destroy(UIElement* self)
{
    _ui_constraints_free(&self->constraints);
    UICheckbox* checkbox = (UICheckbox*)self;
    free(checkbox);
}

void _ui_slider_update(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    if (check_collision_point_rect(get_mouse_position().x, get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (is_mouse_button_pressed(SDL_BUTTON_LEFT))
            slider->mouse_state = MS_PRESS;
        else if (is_mouse_button_released(SDL_BUTTON_LEFT) && slider->mouse_state == MS_PRESS)
            slider->mouse_state = MS_HOVER;
        else if (slider->mouse_state == MS_NONE)
            slider->mouse_state = MS_HOVER;
    }
    else if (slider->mouse_state != MS_PRESS)
        slider->mouse_state = MS_NONE;
    else if (is_mouse_button_released(SDL_BUTTON_LEFT))
        slider->mouse_state = MS_NONE;

    if (slider->mouse_state == MS_PRESS)
        slider->value = clamp((get_mouse_position().x - self->position.x) / (double)self->size.x, 0.0, 1.0);
}
void _ui_slider_recalculate(UIElement* self)
{
    __ui_element_recalculate(self);
}
void _ui_slider_render(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    int handle_thickness = 1.4 * slider->thickness;
    Color color = color_shift(slider->color, slider->mouse_state == MS_PRESS ? 15 : (slider->mouse_state == MS_HOVER ? 10 : 0));
    draw_filled_rounded_rect(self->position.x, self->position.y + (self->size.y - slider->thickness) * 0.5, self->size.x, slider->thickness, slider->corner_radius, color);
    draw_filled_rounded_rect(self->position.x + slider->value * self->size.x - handle_thickness * 0.5, self->position.y, handle_thickness, self->size.y, slider->corner_radius, slider->slider_color);
}
void _ui_slider_destroy(UIElement* self)
{
    _ui_constraints_free(&self->constraints);
    UISlider* slider = (UISlider*)self;
    free(slider);
}

void __ui_element_recalculate(UIElement* element)
{
    //recalculate size
    element->size.x = __ui_calculate_size(element->constraints.width, element->parent->size.x);
    element->size.y = __ui_calculate_size(element->constraints.height, element->parent->size.y);
    if (element->constraints.width->constraint_type == CT_ASPECT)
        element->size.x = element->constraints.width->value * element->size.y;
    else if (element->constraints.height->constraint_type == CT_ASPECT)
        element->size.y = element->constraints.height->value * element->size.x;
    if (element->size.x < 0 || element->size.y < 0)
        SDL_Log("invalid constraint type for ui element size");

    //recalculate position
    element->position.x = __ui_calculate_position(element->constraints.x,
                                                  element->parent->position.x,
                                                  element->parent->size.x,
                                                  element->size.x);
    element->position.y = __ui_calculate_position(element->constraints.y,
                                                  element->parent->position.y,
                                                  element->parent->size.y,
                                                  element->size.y);
    if (element->position.x < 0 || element->position.y < 0)
        SDL_Log("invalid constraint type for ui element position");
}
int __ui_calculate_size(UIConstraint* constraint, int parent_size)
{
    if (constraint->constraint_type == CT_PIXEL)
    {
        if (constraint->value < 0)
            return round(parent_size + constraint->value);
        else
            return round(constraint->value);
    }
    else if (constraint->constraint_type == CT_RELATIVE)
        return round(constraint->value * parent_size);
    else
        return -1;
}
int __ui_calculate_position(UIConstraint* constraint, int parent_position, int parent_size, int size)
{
    switch (constraint->constraint_type)
    {
        case CT_PIXEL:
        {
            if (constraint->value < 0)
                return round(parent_position + parent_size + constraint->value);
            else
                return round(parent_position + constraint->value);
        }
        case CT_CENTER:
            return round(parent_position + (parent_size - size) / 2);
        case CT_RELATIVE:
            return round(parent_position + constraint->value * parent_size);
        default: //CT_ASPECT
            return -1;
    }
}