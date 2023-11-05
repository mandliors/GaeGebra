#include "ui_element.h"
#include "../../app/app.h"
#include "../../renderer/renderer.h"
#include "../../input/input.h"
#include "../../utils/math/math.h"

#include <math.h>
#include <string.h>

UIContainer* ui_create_container(UIContainer* parent, UIConstraints constraints)
{
    UIContainer* container = (UIContainer*)malloc(sizeof(UIContainer));
    UIElement* element = (UIElement*)container;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_container_update;
    element->recalculate = _ui_container_recalculate;
    element->render = _ui_container_render;
    element->destroy = _ui_container_destroy;

    container->children = vector_create(0);
    if (parent != NULL && parent->children->size > 0)
        container->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        container->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);

    return container;
}

UIPanel* ui_create_panel(UIContainer* parent, UIConstraints constraints, Color color, Color border_color, Uint32 border_width, Uint32 roundness)
{
    UIPanel* panel = (UIPanel*)malloc(sizeof(UIPanel));
    UIElement* element = (UIElement*)panel;

    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_panel_update;
    element->recalculate = _ui_panel_recalculate;
    element->render = _ui_panel_render;
    element->destroy = _ui_panel_destroy;

    panel->color = color;
    panel->border_color = border_color;
    panel->border_width = border_width;
    panel->corner_radius = roundness;

    if (parent->children->size > 0)
        panel->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        panel->base.recalculate(NULL, element);
    
    if (parent)
        vector_push_back(parent->children, element);

    return panel;
}
UILabel* ui_create_label(UIContainer* parent, UIConstraints constraints, const char* text, Color color)
{
    UILabel* label = (UILabel*)malloc(sizeof(UILabel));
    UIElement* element = (UIElement*)label;

    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_label_update;
    element->recalculate = _ui_label_recalculate;
    element->render = _ui_label_render;
    element->destroy = _ui_label_destroy;

    strcpy(label->text, text);
    label->color = color;

    if (parent->children->size > 0)
        label->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        label->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);  

    return label;
}
UIButton* ui_create_button(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_click)(UIButton* self))
{
    UIButton* button = (UIButton*)malloc(sizeof(UIButton));
    UIElement* element = (UIElement*)button;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_button_update;
    element->recalculate = _ui_button_recalculate;
    element->render = _ui_button_render;
    element->destroy = _ui_button_destroy;

    strcpy(button->text, text);
    button->text_position = (SDL_Point){0, 0};
    button->color = color;
    button->text_color = text_color;
    button->corner_radius = 2;
    button->mouse_state = MS_NONE;
    button->on_click = on_click;

    if (parent->children->size > 0)
        button->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        button->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return button;
}
UIImageButton* ui_create_imagebutton(UIContainer* parent, UIConstraints constraints, Texture* texture, void (*on_click)(UIImageButton* self))
{
    UIImageButton* button = (UIImageButton*)malloc(sizeof(UIImageButton));
    UIElement* element = (UIElement*)button;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_imagebutton_update;
    element->recalculate = _ui_imagebutton_recalculate;
    element->render = _ui_imagebutton_render;
    element->destroy = _ui_imagebutton_destroy;

    button->mouse_state = MS_NONE;
    button->texture = texture;
    button->on_click = on_click;

    if (parent->children->size > 0)
        button->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        button->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return button;
}
UITextbox* ui_create_textbox(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color, void (*on_text_changed)(UITextbox* self, const char* text))
{
    UITextbox* textbox = (UITextbox*)malloc(sizeof(UITextbox));
    UIElement* element = (UIElement*)textbox;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_textbox_update;
    element->recalculate = _ui_textbox_recalculate;
    element->render = _ui_textbox_render;
    element->destroy = _ui_textbox_destroy;

    textbox->color = color;
    textbox->text_color = text_color;
    textbox->corner_radius = 2;
    strcpy(textbox->text, text);
    textbox->focused = false;
    textbox->mouse_state = MS_NONE;
    textbox->on_text_changed = on_text_changed;

    if (parent->children->size > 0)
        textbox->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        textbox->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return textbox;
}
UICheckbox* ui_create_checkbox(UIContainer* parent, UIConstraints constraints, Color checked_color, Color unchecked_color, void (*on_checked_changed)(UICheckbox* self, bool checked))
{
    UICheckbox* checkbox = (UICheckbox*)malloc(sizeof(UICheckbox));
    UIElement* element = (UIElement*)checkbox;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_checkbox_update;
    element->recalculate = _ui_checkbox_recalculate;
    element->render = _ui_checkbox_render;
    element->destroy = _ui_checkbox_destroy;

    checkbox->checked = false;
    checkbox->checked_color = checked_color;
    checkbox->unchecked_color = unchecked_color;
    checkbox->corner_radius = 2;
    checkbox->mouse_state = MS_NONE;
    checkbox->on_checked_changed = on_checked_changed;

    if (parent->children->size > 0)
        checkbox->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        checkbox->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return checkbox;
}
UISlider* ui_create_slider(UIContainer* parent, UIConstraints constraints, double value, Color color, Color slider_color, void (*on_value_changed)(UISlider* self, double value))
{
    UISlider* slider = (UISlider*)malloc(sizeof(UISlider));
    UIElement* element = (UIElement*)slider;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
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
    slider->on_value_changed = on_value_changed;

    if (parent->children->size > 0)
        slider->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        slider->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return slider;
}
UIDropdownList* ui_create_dropdown(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_selection_changed)(UIDropdownList* self, Sint32 index))
{
    UIDropdownList* dropdown = (UIDropdownList*)malloc(sizeof(UIDropdownList));
    UIElement* element = (UIElement*)dropdown;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_dropdown_update;
    element->recalculate = _ui_dropdown_recalculate;
    element->render = _ui_dropdown_render;
    element->destroy = _ui_dropdown_destroy;

    size_t item_count = 1;
    for (size_t i = 0; items[i] != '\0'; i++)
        if (items[i] == ';')
            item_count++;
    dropdown->items = vector_create(item_count + 1);

    int idx = -1;
    items = strdup(items);
    char* token = strtok(items, ";");
    vector_push_back(dropdown->items, _ui_dropdownitem_create(dropdown, constraints, idx++, token));
    constraints.y = new_offset_constraint(0);
    while (token != NULL)
    {
        vector_push_back(dropdown->items, _ui_dropdownitem_create(dropdown, constraints, idx++, token));
        token = strtok(NULL, ";");
    }
    free(items);
    dropdown->selected_item = 0;
    strcpy(((_UIDropdownItem*)vector_get(dropdown->items, 0))->text, ((_UIDropdownItem*)vector_get(dropdown->items, dropdown->selected_item + 1))->text);

    dropdown->expanded = false;
    dropdown->color = color;
    dropdown->text_color = text_color;
    dropdown->corner_radius = 2;
    dropdown->on_selection_changed = on_selection_changed;

    if (parent->children->size > 0)
        dropdown->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        dropdown->base.recalculate(NULL, element);
    
    if (parent)
        vector_push_back(parent->children, element);
    
    return dropdown;
}
UISplitButton* ui_create_splitbutton(UIContainer* parent, UIConstraints constraints, char* items, Color color, Color text_color, void (*on_item_clicked)(UISplitButton* self, Sint32 index), bool auto_dropdown)
{
    UISplitButton* splitbutton = (UISplitButton*)malloc(sizeof(UISplitButton));
    UIElement* element = (UIElement*)splitbutton;
    
    element->parent = (UIElement*)parent;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_splitbutton_update;
    element->recalculate = _ui_splitbutton_recalculate;
    element->render = _ui_splitbutton_render;
    element->destroy = _ui_splitbutton_destroy;

    size_t item_count = 1;
    char temp[50]; int max_length = 0;
    int length;
    for (size_t i = 0; items[i] != '\0'; i++)
    {
        if (items[i] == ';')
        {
            item_count++;
            length = renderer_query_text_size(temp).x;
            if (length > max_length)
                max_length = length;
            temp[0] = '\0';
        }
        else
            strncat(temp, items + i, 1);
    }
    length = renderer_query_text_size(temp).x;
    if (length > max_length)
        max_length = length;
    splitbutton->items = vector_create(item_count);

    int idx = -1;
    items = strdup(items);
    char* token = strtok(items, ";");
    constraints.width = new_pixel_constraint(renderer_query_text_size(token).x + 12);
    vector_push_back(splitbutton->items, _ui_splitbuttonitem_create((UISplitButton*)splitbutton, constraints, idx++, token));
    element->constraints = constraints;
    constraints.y = new_offset_constraint(0);
    constraints.width = new_pixel_constraint(max_length + 12);
    token = strtok(NULL, ";");
    while (token != NULL)
    {
        vector_push_back(splitbutton->items, _ui_splitbuttonitem_create((UISplitButton*)splitbutton, constraints, idx++, token));
        token = strtok(NULL, ";");
    }
    free(items);

    splitbutton->expanded = false;
    splitbutton->color = color;
    splitbutton->text_color = text_color;
    splitbutton->corner_radius = 2;
    splitbutton->on_item_clicked = on_item_clicked;
    splitbutton->auto_dropdown = auto_dropdown;

    if (parent->children->size > 0)
        splitbutton->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        splitbutton->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);

    return splitbutton;
}

void _ui_container_update(UIElement* self)
{
    UIContainer* container = (UIContainer*)self;
    for (int32_t i = container->children->size - 1; i >= 0; i--)
    {
        UIElement* child = (UIElement*)vector_get(container->children, i);
        child->update(child);
    }
}
void _ui_container_recalculate(UIElement* sibling, UIElement* self)
{
    if (self->parent != NULL)
    {
        //recalculate size
        self->size.x = __ui_calculate_size(&self->constraints.width, self->parent->size.x);
        self->size.y = __ui_calculate_size(&self->constraints.height, self->parent->size.y);
        if (self->constraints.width.constraint_type == CT_ASPECT)
            self->size.x = (int)round(self->constraints.width.value * self->size.y);
        else if (self->constraints.height.constraint_type == CT_ASPECT)
            self->size.y = (int)round(self->constraints.height.value * self->size.x);
        if (self->size.x < 0 || self->size.y < 0)
            SDL_Log("invalid constraint type for container size");

        //recalculate position
        self->position.x = __ui_calculate_position (&self->constraints.x,
                                                    sibling == NULL ? -1 : sibling->position.x,
                                                    sibling == NULL ? -1 : sibling->size.x,
                                                    self->parent->position.x,
                                                    self->parent->size.x,
                                                    self->size.x);
        self->position.y = __ui_calculate_position (&self->constraints.y,
                                                    sibling == NULL ? -1 : sibling->position.y,
                                                    sibling == NULL ? -1 : sibling->size.y,
                                                    self->parent->position.y,
                                                    self->parent->size.y,
                                                    self->size.y);
        if (self->position.x < 0 || self->position.y < 0)
            SDL_Log("invalid constraint type for container position");
    }
    UIContainer* container = (UIContainer*)self;
    if (container->children->size > 0)
    {
        UIElement* child = (UIElement*)vector_get(container->children, 0);
        child->recalculate(NULL, child);
        for (size_t i = 1; i < container->children->size; i++)
        {
            UIElement* child = (UIElement*)vector_get(container->children, i);
            UIElement* sibling = (UIElement*)vector_get(container->children, i - 1);
            child->recalculate(sibling, child);
        }
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
    UIContainer* container = (UIContainer*)self;
    for (size_t i = 0; i < container->children->size; i++)
    {
        UIElement* child = (UIElement*)vector_get(container->children, i);
        child->destroy(child);
    }
    vector_free(container->children);
    free(container);
}

void _ui_panel_update(UIElement* self)
{
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
        app_get_active_window()->ui_data.mouse_captured = true;
}
void _ui_panel_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_panel_render(UIElement* self)
{
    UIPanel* panel = (UIPanel*)self;
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, panel->corner_radius, panel->color);
    for (size_t i = 0; i < panel->border_width; i++)
        renderer_draw_rounded_rect(self->position.x + i, self->position.y + i, self->size.x - 2 * i, self->size.y - 2 * i, panel->corner_radius, panel->border_color);
}
void _ui_panel_destroy(UIElement* self)
{
    UIPanel* panel = (UIPanel*)self;
    free(panel);
}

void _ui_label_update(UIElement* self)
{
    SDL_Point size = renderer_query_text_size(((UILabel*)self)->text);
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, size.x, size.y))
        app_get_active_window()->ui_data.mouse_captured = true;
}
void _ui_label_recalculate(UIElement* sibling, UIElement* self)
{
    //recalculate size
    UILabel* label = (UILabel*)self;
    self->size = renderer_query_text_size(label->text);

    //recalculate position
    self->position.x = __ui_calculate_position (&self->constraints.x,
                                                sibling == NULL ? -1 : sibling->position.x,
                                                sibling == NULL ? -1 : sibling->size.x,
                                                self->parent->position.x,
                                                self->parent->size.x,
                                                self->size.x);
    self->position.y = __ui_calculate_position (&self->constraints.y,
                                                sibling->position.y,
                                                sibling->size.y,
                                                self->parent->position.y,
                                                self->parent->size.y,
                                                self->size.y);
    if (self->position.x < 0 || self->position.y < 0)
        SDL_Log("invalid constraint type for ui element");
}
void _ui_label_render(UIElement* self)
{
    UILabel* label = (UILabel*)self;
    renderer_draw_text(label->text, self->position.x, self->position.y, label->color);
}
void _ui_label_destroy(UIElement* self)
{
    UILabel* label = (UILabel*)self;
    free(label);
}

void _ui_button_update(UIElement* self)
{
    UIButton* button = (UIButton*)self;
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            button->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && button->mouse_state == MS_PRESS)
        {
            if (button->on_click)
                button->on_click(button);
            button->mouse_state = MS_HOVER;
        }
        else if (button->mouse_state == MS_NONE)
            button->mouse_state = MS_HOVER;
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (button->mouse_state != MS_PRESS)
        button->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        button->mouse_state = MS_NONE;
}
void _ui_button_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
    
    UIButton* button = (UIButton*)self;
    SDL_Point text_size = renderer_query_text_size(button->text);
    button->text_position = (SDL_Point){self->position.x + (self->size.x - text_size.x) / 2,
                                      self->position.y + (self->size.y - text_size.y) / 2};
}
void _ui_button_render(UIElement* self)
{
    UIButton* button = (UIButton*)self;
    Color color = __clever_color_shift(button->color, button->mouse_state == MS_PRESS ? 15 : (button->mouse_state == MS_HOVER ? 10 : 0));
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, button->corner_radius, color);
    if (button->text[0] != '\0')
        renderer_draw_text(button->text, button->text_position.x, button->text_position.y, button->text_color);
}
void _ui_button_destroy(UIElement* self)
{
    UIButton* button = (UIButton*)self;
    free(button);
}

void _ui_imagebutton_update(UIElement* self)
{
    UIImageButton* button = (UIImageButton*)self;
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            button->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && button->mouse_state == MS_PRESS)
        {
            if (button->on_click)
                button->on_click(button);
            button->mouse_state = MS_HOVER;
        }
        else if (button->mouse_state == MS_NONE)
            button->mouse_state = MS_HOVER;
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (button->mouse_state != MS_PRESS)
        button->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        button->mouse_state = MS_NONE;
}
void _ui_imagebutton_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_imagebutton_render(UIElement* self)
{
    UIImageButton* button = (UIImageButton*)self;
    renderer_draw_texture(button->texture, self->position.x, self->position.y, self->size.x, self->size.y);
    if (button->mouse_state != MS_NONE)
    {
        SDL_SetTextureBlendMode(button->texture->texture, SDL_BLENDMODE_ADD);
        SDL_SetTextureAlphaMod(button->texture->texture, button->mouse_state == MS_HOVER ? 10 : 15);
        renderer_draw_texture(button->texture, self->position.x, self->position.y, self->size.x, self->size.y);
        SDL_SetTextureAlphaMod(button->texture->texture, 255);
        SDL_SetTextureBlendMode(button->texture->texture, SDL_BLENDMODE_BLEND);
    }
}
void _ui_imagebutton_destroy(UIElement* self)
{
    UIImageButton* button = (UIImageButton*)self;
    free(button);
}

void _ui_textbox_update(UIElement* self)
{
    UITextbox* textbox = (UITextbox*)self;
    if (textbox->focused) 
    {
        size_t textlen = strlen(textbox->text);
        if (app_get_active_window()->ui_data.backspace_pressed)
        {
            if (input_is_key_down(SDL_SCANCODE_LCTRL) && textlen > 0)
            {
                bool found = false;
                for (size_t i = textlen - 1; i >= 1; i--)
                {
                    if (textbox->text[i] == ' ')
                    {
                        textbox->text[i] = '\0';
                        found = true;
                        break;
                    }
                }
                if (!found)
                    textbox->text[0] = '\0';
            }
            else
            {
                do
                {
                    if (textlen == 0)
                        break;
                    if ((textbox->text[textlen - 1] & 0x80) == 0x00)
                    {
                        textbox->text[textlen - 1] = 0x00;
                        break;
                    }
                    if ((textbox->text[textlen - 1] & 0xC0) == 0x80) 
                    {
                        textbox->text[textlen - 1] = 0x00;
                        textlen--;
                    }
                    if ((textbox->text[textlen - 1] & 0xC0) == 0xC0)
                    {
                        textbox->text[textlen - 1] = 0x00;
                        break;
                    }
                } while(true);
            }
            app_get_active_window()->ui_data.backspace_pressed = false;
        }
        else if (app_get_active_window()->ui_data.text_input[0] != '\0')
        {
            if (textlen + strlen(app_get_active_window()->ui_data.text_input) < UITEXT_MAX_LENGTH)
                strcat(textbox->text, app_get_active_window()->ui_data.text_input);
            app_get_active_window()->ui_data.text_input[0] = '\0';
        }
        if (strlen(textbox->text) != textlen)
            if (textbox->on_text_changed)
                textbox->on_text_changed(textbox, textbox->text);
    }

    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            textbox->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        {
            if (textbox->mouse_state == MS_PRESS)
            {
                textbox->mouse_state = MS_HOVER;
                textbox->focused = true;
                SDL_StartTextInput();
            }
            else if (textbox->mouse_state == MS_HOVER)
                textbox->focused = false;
        }
        else if (textbox->mouse_state == MS_NONE)
            textbox->mouse_state = MS_HOVER;
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
    {
        textbox->focused = false;
        SDL_StopTextInput();
    }
    else if (textbox->mouse_state != MS_PRESS || input_is_mouse_button_released(SDL_BUTTON_LEFT))
        textbox->mouse_state = MS_NONE;
}
void _ui_textbox_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_textbox_render(UIElement* self)
{
    UITextbox* textbox = (UITextbox*)self;
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, 2, textbox->mouse_state == MS_PRESS ? __clever_color_shift(textbox->color, 15) : (textbox->mouse_state == MS_HOVER ? __clever_color_shift(textbox->color, 10) : textbox->color));
    renderer_draw_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, 2, textbox->text_color);
    
    SDL_Point text_size = renderer_query_text_size(textbox->text);
    int cursor_offset = (int)round(self->size.y - text_size.y) * 0.5;
    if (text_size.x > self->size.x - 12)
    {
        renderer_set_clip_rect(self->position.x + 6, self->position.y, self->size.x - 12, self->size.y);
        renderer_draw_text(textbox->text, self->position.x + 6 - (text_size.x - self->size.x + 12), self->position.y + (int)round((self->size.y - text_size.y) * 0.5), textbox->text_color);
        renderer_reset_clip_rect();
        if (textbox->focused)
            renderer_draw_line(self->position.x + self->size.x - 6, self->position.y + cursor_offset, self->position.x + self->size.x - 6, self->position.y + self->size.y - cursor_offset, 2, textbox->text_color);
    }
    else
    {
        if (textbox->text[0] != '\0')
            renderer_draw_text(textbox->text, self->position.x + 6, self->position.y + (int)round((self->size.y - text_size.y) * 0.5), textbox->text_color);
        if (textbox->focused)
            renderer_draw_line(self->position.x + text_size.x + 6, self->position.y + cursor_offset, self->position.x + text_size.x + 6, self->position.y + self->size.y - cursor_offset, 2, textbox->text_color);
    }
}
void _ui_textbox_destroy(UIElement* self)
{
    UITextbox* textbox = (UITextbox*)self;
    free(textbox);
}

void _ui_checkbox_update(UIElement* self)
{
    UICheckbox* checkbox = (UICheckbox*)self;
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            checkbox->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && checkbox->mouse_state == MS_PRESS)
        {
            checkbox->checked = !checkbox->checked;
            checkbox->mouse_state = MS_HOVER;
            if (checkbox->on_checked_changed)
                checkbox->on_checked_changed(checkbox, checkbox->checked);
        }
        else if (checkbox->mouse_state == MS_NONE)
            checkbox->mouse_state = MS_HOVER;
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (checkbox->mouse_state != MS_PRESS)
        checkbox->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        checkbox->mouse_state = MS_NONE;
}
void _ui_checkbox_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_checkbox_render(UIElement* self)
{
    UICheckbox* checkbox = (UICheckbox*)self;
    if (checkbox->checked)
    {
        if (checkbox->mouse_state == MS_NONE)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, checkbox->checked_color);
        else if (checkbox->mouse_state == MS_HOVER)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, __clever_color_shift(checkbox->checked_color, 10));
        else if (checkbox->mouse_state == MS_PRESS)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, __clever_color_shift(checkbox->checked_color, 15));
        SDL_Point p1 = (SDL_Point){self->position.x + (int)round(self->size.x * 0.25), (int)round(self->position.y + self->size.y * 0.5)};
        SDL_Point p2 = (SDL_Point){self->position.x + (int)round(self->size.x * 0.45), (int)round(self->position.y + self->size.y * 0.75)};
        SDL_Point p3 = (SDL_Point){self->position.x + (int)round(self->size.x * 0.8), (int)round(self->position.y + self->size.y * 0.25)};
        renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 2, checkbox->unchecked_color);
        renderer_draw_line(p2.x, p2.y, p3.x, p3.y, 2, checkbox->unchecked_color);
    }
    else
    {
        if (checkbox->mouse_state == MS_NONE)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, checkbox->checked_color);
        else if (checkbox->mouse_state == MS_HOVER)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, __clever_color_shift(checkbox->checked_color, 10));
        else if (checkbox->mouse_state == MS_PRESS)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, __clever_color_shift(checkbox->checked_color, 15));
        renderer_draw_filled_rounded_rect(self->position.x + 2, self->position.y + 2, self->size.x - 4, self->size.y - 4, checkbox->corner_radius, checkbox->unchecked_color);
    }
}
void _ui_checkbox_destroy(UIElement* self)
{
    UICheckbox* checkbox = (UICheckbox*)self;
    free(checkbox);
}

void _ui_slider_update(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            slider->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && slider->mouse_state == MS_PRESS)
            slider->mouse_state = MS_HOVER;
        else if (slider->mouse_state == MS_NONE)
            slider->mouse_state = MS_HOVER;
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (slider->mouse_state != MS_PRESS)
        slider->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        slider->mouse_state = MS_NONE;

    if (slider->mouse_state == MS_PRESS)
    {
        slider->value = clamp((input_get_mouse_position().x - self->position.x) / (double)self->size.x, 0.0, 1.0);
        if (slider->on_value_changed)
            slider->on_value_changed(slider, slider->value);
    }
}
void _ui_slider_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_slider_render(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    int handle_thickness = (int)round(1.4 * slider->thickness);
    Color color = __clever_color_shift(slider->color, slider->mouse_state == MS_PRESS ? 15 : (slider->mouse_state == MS_HOVER ? 10 : 0));
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y + (int)round((self->size.y - slider->thickness) * 0.5), self->size.x, slider->thickness, slider->corner_radius, color);
    renderer_draw_filled_rounded_rect(self->position.x + (int)round(slider->value * self->size.x) - (int)round(handle_thickness * 0.5), self->position.y, handle_thickness, self->size.y, slider->corner_radius, slider->slider_color);
}
void _ui_slider_destroy(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    free(slider);
}

void _ui_dropdown_update(UIElement* self)
{
    UIDropdownList* dropdown = (UIDropdownList*)self;
    UIElement* top_item = (UIElement*)vector_get(dropdown->items, 0);
    top_item->update(top_item);
    if (dropdown->expanded)
    {
        for (size_t i = 1; i < dropdown->items->size; i++)
        {
            UIElement* item = (UIElement*)vector_get(dropdown->items, i);
            item->update(item);
        }
    }
    if (input_is_mouse_button_released(SDL_BUTTON_LEFT) &&
        !check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
        self->position.x, self->position.y, self->size.x, self->size.y))
        dropdown->expanded = false;
}
void _ui_dropdown_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
    UIDropdownList* dropdown = (UIDropdownList*)self;
    UIElement* top_item = (UIElement*)vector_get(dropdown->items, 0);
    top_item->recalculate(sibling, top_item);
    
    for (size_t i = 1; i < dropdown->items->size; i++)
    {
        UIElement* item = (UIElement*)vector_get(dropdown->items, i);
        item->recalculate((UIElement*)vector_get(dropdown->items, i - 1), item);
    }
}
void _ui_dropdown_render(UIElement* self)
{
    UIDropdownList* dropdown = (UIDropdownList*)self;
    UIElement* top_item = (UIElement*)vector_get(dropdown->items, 0);
    top_item->render(top_item);
    if (dropdown->expanded)
    {
        for (size_t i = 0; i < dropdown->items->size; i++)
        {
            UIElement* item = (UIElement*)vector_get(dropdown->items, i);
            item->render(item);
        }
    }
    double arrow_size = 0.35;
    SDL_Point arrow_pos = (SDL_Point){self->position.x + self->size.x - (int)round(self->size.y * arrow_size) - 10,
                                  self->position.y + (int)round(self->size.y * (1 - arrow_size) * 0.5)};
    renderer_draw_filled_triangle(arrow_pos.x, arrow_pos.y, arrow_pos.x + (int)round(self->size.y * arrow_size),
                                  arrow_pos.y, arrow_pos.x + (int)round(self->size.y * arrow_size * 0.5),
                                  arrow_pos.y + (int)round(self->size.y * arrow_size), dropdown->text_color);
}
void _ui_dropdown_destroy(UIElement* self)
{
    UIDropdownList* dropdown = (UIDropdownList*)self;
    for (size_t i = 0; i < dropdown->items->size; i++)
    {
        UIElement* item = (UIElement*)vector_get(dropdown->items, i);
        item->destroy(item);
    }
    vector_free(dropdown->items);
    free(dropdown);
}

_UIDropdownItem* _ui_dropdownitem_create(UIDropdownList* parent, UIConstraints constraints, Sint32 index, const char* text)
{
    _UIDropdownItem* item = (_UIDropdownItem*)malloc(sizeof(_UIDropdownItem));
    UIElement* element = (UIElement*)item;
    
    element->parent = parent->base.parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_dropdownitem_update;
    element->recalculate = _ui_dropdownitem_recalculate;
    element->render = _ui_dropdownitem_render;
    element->destroy = _ui_dropdownitem_destroy;

    item->parent_dropdown = parent;
    item->dropdown_index = index;
    strcpy(item->text, text);
    item->mouse_state = MS_NONE;

    return item;
}
void _ui_dropdownitem_update(UIElement* self)
{
    _UIDropdownItem* item = (_UIDropdownItem*)self;
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            item->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && item->mouse_state == MS_PRESS)
        {
            item->mouse_state = MS_NONE;
            _ui_dropdownitem_on_click(item);
        }
        else if (item->mouse_state == MS_NONE)
            item->mouse_state = MS_HOVER;
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (item->mouse_state != MS_PRESS)
        item->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        item->mouse_state = MS_NONE;
}
void _ui_dropdownitem_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
    self->position.x = ((_UIDropdownItem*)self)->parent_dropdown->base.position.x;
}
void _ui_dropdownitem_render(UIElement* self)
{
    _UIDropdownItem* item = (_UIDropdownItem*)self;
    int shift = item->dropdown_index == -1 ? 0 : 8;
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, item->parent_dropdown->corner_radius,
                                      item->mouse_state == MS_PRESS ? __clever_color_shift(item->parent_dropdown->color, 15 + shift)
                                      : (item->mouse_state == MS_HOVER ? __clever_color_shift(item->parent_dropdown->color, 10 + shift)
                                      : __clever_color_shift(item->parent_dropdown->color, shift)));
    renderer_draw_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y,
                               item->parent_dropdown->corner_radius, __clever_color_shift(item->parent_dropdown->color, shift));
    if (item->text[0] != '\0') renderer_draw_text(item->text, self->position.x + 6, self->position.y + (int)round((self->size.y - renderer_query_text_size(item->text).y) * 0.5), __clever_color_shift(item->parent_dropdown->text_color, shift));
}
void _ui_dropdownitem_destroy(UIElement* self)
{
    _UIDropdownItem* item = (_UIDropdownItem*)self;
    free(item);
}
void _ui_dropdownitem_on_click(_UIDropdownItem* self)
{
    UIDropdownList* dropdown = (UIDropdownList*)self->parent_dropdown;
    if (self->dropdown_index == -1)
        dropdown->expanded = !dropdown->expanded;
    else
    {
        _UIDropdownItem* top_item = (_UIDropdownItem*)vector_get(dropdown->items, 0);
        strcpy(top_item->text, self->text);
        if ((Sint32)dropdown->selected_item != self->dropdown_index && dropdown->on_selection_changed)
            dropdown->on_selection_changed(dropdown, self->dropdown_index);
        dropdown->expanded = false;
        dropdown->selected_item = self->dropdown_index;
    }
}

void _ui_splitbutton_update(UIElement* self)
{
    UISplitButton* splitbutton = (UISplitButton*)self;        
    UIElement* top_item = (UIElement*)vector_get(splitbutton->items, 0);
    top_item->update(top_item);
    if (splitbutton->expanded)
    {
        for (size_t i = 1; i < splitbutton->items->size; i++)
        {
            UIElement* item = (UIElement*)vector_get(splitbutton->items, i);
            item->update(item);
        }
    }
    if (input_is_mouse_button_released(SDL_BUTTON_LEFT) &&
        !check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
        self->position.x, self->position.y, self->size.x, self->size.y))
    {
        splitbutton->expanded = false;
        if (app_get_active_window()->ui_data.expanded_splitbutton == splitbutton)
            app_get_active_window()->ui_data.expanded_splitbutton = NULL;
    }
}
void _ui_splitbutton_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
    UISplitButton* splitbutton = (UISplitButton*)self;
    UIElement* top_item = (UIElement*)vector_get(splitbutton->items, 0);
    top_item->recalculate(sibling, top_item);
    
    for (size_t i = 1; i < splitbutton->items->size; i++)
    {
        UIElement* item = (UIElement*)vector_get(splitbutton->items, i);
        item->recalculate((UIElement*)vector_get(splitbutton->items, i - 1), item);
    }
}
void _ui_splitbutton_render(UIElement* self)
{
    UISplitButton* splitbutton = (UISplitButton*)self;
    UIElement* top_item = (UIElement*)vector_get(splitbutton->items, 0);
    top_item->render(top_item);
    if (splitbutton->expanded)
    {
        for (size_t i = 0; i < splitbutton->items->size; i++)
        {
            UIElement* item = (UIElement*)vector_get(splitbutton->items, i);
            item->render(item);
        }
    }
}
void _ui_splitbutton_destroy(UIElement* self)
{
    UISplitButton* splitbutton = (UISplitButton*)self;
    for (size_t i = 0; i < splitbutton->items->size; i++)
    {
        UIElement* item = (UIElement*)vector_get(splitbutton->items, i);
        item->destroy(item);
    }
    vector_free(splitbutton->items);
    free(splitbutton);
}

_UISplitButtonItem* _ui_splitbuttonitem_create(UISplitButton* parent, UIConstraints constraints, Sint32 index, const char* text)
{
    _UISplitButtonItem* item = (_UISplitButtonItem*)malloc(sizeof(_UISplitButtonItem));
    UIElement* element = (UIElement*)item;
    
    element->parent = parent->base.parent;
    element->constraints = constraints;
    element->position = (SDL_Point){0, 0};
    element->size = (SDL_Point){0, 0};
    element->update = _ui_splitbuttonitem_update;
    element->recalculate = _ui_splitbuttonitem_recalculate;
    element->render = _ui_splitbuttonitem_render;
    element->destroy = _ui_splitbuttonitem_destroy;

    item->parent_splitbutton = parent;
    item->splitbutton_index = index;
    strcpy(item->text, text);
    item->mouse_state = MS_NONE;

    return item;
}
void _ui_splitbuttonitem_update(UIElement* self)
{
    _UISplitButtonItem* item = (_UISplitButtonItem*)self;
    if (!app_get_active_window()->ui_data.mouse_captured && check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (app_get_active_window()->ui_data.expanded_splitbutton != NULL &&
            app_get_active_window()->ui_data.expanded_splitbutton != item->parent_splitbutton)
        {
            app_get_active_window()->ui_data.expanded_splitbutton->expanded = false;
            app_get_active_window()->ui_data.expanded_splitbutton = item->parent_splitbutton;
            item->parent_splitbutton->expanded = true;
        }
        else
        {
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
                item->mouse_state = MS_PRESS;
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && item->mouse_state == MS_PRESS)
            {
                item->mouse_state = MS_NONE;
                _ui_splitbuttonitem_on_click(item);
            }
            else if (item->mouse_state == MS_NONE)
                item->mouse_state = MS_HOVER;
        }
        app_get_active_window()->ui_data.mouse_captured = true;
    }
    else if (item->mouse_state != MS_PRESS)
        item->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        item->mouse_state = MS_NONE;
}
void _ui_splitbuttonitem_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
    self->position.x = ((_UISplitButtonItem*)self)->parent_splitbutton->base.position.x;
}
void _ui_splitbuttonitem_render(UIElement* self)
{
    _UISplitButtonItem* item = (_UISplitButtonItem*)self;
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, item->parent_splitbutton->corner_radius,
                                      item->mouse_state == MS_PRESS ? __clever_color_shift(item->parent_splitbutton->color, 15)
                                      : (item->mouse_state == MS_HOVER ? __clever_color_shift(item->parent_splitbutton->color, 10)
                                      : item->parent_splitbutton->color));
    renderer_draw_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y,
                               item->parent_splitbutton->corner_radius, item->parent_splitbutton->color);
    if (item->text[0] != '\0') renderer_draw_text(item->text, self->position.x + 6, self->position.y + (int)round((self->size.y - renderer_query_text_size(item->text).y) * 0.5), item->parent_splitbutton->text_color);
}
void _ui_splitbuttonitem_destroy(UIElement* self)
{
    _UISplitButtonItem* item = (_UISplitButtonItem*)self;
    free(item);
}
void _ui_splitbuttonitem_on_click(_UISplitButtonItem* self)
{
    UISplitButton* splitbutton = (UISplitButton*)self->parent_splitbutton;
    if (self->splitbutton_index == -1)
    {
        if (splitbutton->expanded)
            app_get_active_window()->ui_data.expanded_splitbutton = NULL;
        else
            app_get_active_window()->ui_data.expanded_splitbutton = splitbutton;
        splitbutton->expanded = !splitbutton->expanded;
    }
    else
    {
        if (splitbutton->on_item_clicked)
            splitbutton->on_item_clicked(splitbutton, self->splitbutton_index);
        splitbutton->expanded = false;
        app_get_active_window()->ui_data.expanded_splitbutton = NULL;
    }
}

void __ui_element_recalculate(UIElement* sibling, UIElement* element)
{
    //recalculate size
    element->size.x = __ui_calculate_size(&element->constraints.width, element->parent->size.x);
    element->size.y = __ui_calculate_size(&element->constraints.height, element->parent->size.y);
    if (element->constraints.width.constraint_type == CT_ASPECT)
        element->size.x = (int)round(element->constraints.width.value * element->size.y);
    else if (element->constraints.height.constraint_type == CT_ASPECT)
        element->size.y = (int)round(element->constraints.height.value * element->size.x);
    if (element->size.x < 0 || element->size.y < 0)
        SDL_Log("invalid constraint type for ui element size");

    //recalculate position
    element->position.x = __ui_calculate_position(&element->constraints.x,
                                                  sibling == NULL ? -1 : sibling->position.x,
                                                  sibling == NULL ? -1 : sibling->size.x,
                                                  element->parent->position.x,
                                                  element->parent->size.x,
                                                  element->size.x);
    element->position.y = __ui_calculate_position(&element->constraints.y,
                                                  sibling == NULL ? -1 : sibling->position.y,
                                                  sibling == NULL ? -1 : sibling->size.y,
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
            return (int)round(parent_size + constraint->value);
        else
            return (int)round(constraint->value);
    }
    else if (constraint->constraint_type == CT_RELATIVE)
        return (int)round(constraint->value * parent_size);
    else
        return -1;
}
int __ui_calculate_position(UIConstraint* constraint, int sibling_position, int sibling_size, int parent_position, int parent_size, int size)
{
    switch (constraint->constraint_type)
    {
        case CT_PIXEL:
        {
            if (constraint->value < 0)
                return (int)round(parent_position + parent_size + constraint->value);
            else
                return (int)round(parent_position + constraint->value);
        }
        case CT_CENTER:
            return (int)round(parent_position + (parent_size - size) / 2);
        case CT_RELATIVE:
            return (int)round(parent_position + constraint->value * parent_size);
        case CT_OFFSET:
            if (sibling_position > 0)
                return (int)round(sibling_position + sibling_size + constraint->value);
            else
                return (int)round(parent_position + sibling_size + constraint->value);
        default: //CT_ASPECT
            return -1;
    }
}
Color __clever_color_shift(Color color, int shift)
{
    if (color.r + color.g + color.b < 255 * 3)
        return color_shift(color, shift);
    else
        return color_shift(color, -shift);
}