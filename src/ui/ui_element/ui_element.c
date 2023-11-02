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
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
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
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
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
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_button_update;
    element->recalculate = _ui_button_recalculate;
    element->render = _ui_button_render;
    element->destroy = _ui_button_destroy;

    strcpy(button->text, text);
    button->text_position = (Vector2){0, 0};
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
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
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
UITextbox* ui_create_textbox(UIContainer* parent, UIConstraints constraints, const char* text, Color color, Color text_color)
{
    UITextbox* textbox = (UITextbox*)malloc(sizeof(UITextbox));
    UIElement* element = (UIElement*)textbox;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
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

    if (parent->children->size > 0)
        textbox->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        textbox->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return textbox;
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
    checkbox->corner_radius = 2;

    if (parent->children->size > 0)
        checkbox->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        checkbox->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
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

    if (parent->children->size > 0)
        slider->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        slider->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return slider;
}
UIDropdownList* ui_create_dropdown(UIContainer* parent, UIConstraints constraints, char* items, Uint32 selected_item, Color color, Color text_color)
{
    UIDropdownList* dropdown = (UIDropdownList*)malloc(sizeof(UIDropdownList));
    UIElement* element = (UIElement*)dropdown;
    
    element->parent = (UIElement*)parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_dropdown_update;
    element->recalculate = _ui_dropdown_recalculate;
    element->render = _ui_dropdown_render;
    element->destroy = _ui_dropdown_destroy;

    dropdown->expanded = false;

    size_t item_count = 1;
    char temp[50]; int max_length = 0;
    for (size_t i = 0; items[i] != '\0'; i++)
    {
        if (items[i] == ';')
        {
            item_count++;
            max_length = renderer_query_text_size(temp).x;
            temp[0] = '\0';
        }
        else
            strncat(temp, &items[i], 1);
    }
    UIConstraint height_constraint = constraints.height;
    height_constraint.value *= (double)item_count + 1.0;
    UIConstraint width_constraint = new_pixel_constraint(max_length + 10);
    dropdown->items_container = ui_create_container(parent, (UIConstraints)
    {
        constraints.x,
        constraints.y,
        width_constraint,
        height_constraint
    });

    int idx = -1;
    items = strdup(items);
    char* token = strtok(items, ";");
    dropdown->items = vector_create(item_count + 1);
    _UIDropdownItem* item = _ui_dropdownitem_create(element, constraints, idx++, token, color, text_color, 2, _ui_dropdownitem_on_click);
    vector_push_back(dropdown->items, item);
    while (token != NULL)
    {
        UIConstraints item_constraints = (UIConstraints)
        {
            constraints.x,
            new_offset_constraint(0),
            width_constraint,
            height_constraint
        };
        item = _ui_dropdownitem_create(element, item_constraints, idx++, token, color, text_color, 2, _ui_dropdownitem_on_click);
        vector_push_back(dropdown->items, item);
        token = strtok(NULL, ";");
    }
    free(items);
    dropdown->selected_item = selected_item;

    if (parent->children->size > 0)
        dropdown->base.recalculate((UIElement*)vector_get(parent->children, parent->children->size - 1), element);
    else
        dropdown->base.recalculate(NULL, element);

    if (parent)
        vector_push_back(parent->children, element);
    
    return dropdown;
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

void _ui_panel_update(UIElement* self) { }
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

void _ui_label_update(UIElement* self) { }
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
    if (check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
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
    Vector2 text_size = renderer_query_text_size(button->text);
    button->text_position = (Vector2){self->position.x + (self->size.x - text_size.x) / 2,
                                      self->position.y + (self->size.y - text_size.y) / 2};
}
void _ui_button_render(UIElement* self)
{
    UIButton* button = (UIButton*)self;
    Color color = color_shift(button->color, button->mouse_state == MS_PRESS ? 15 : (button->mouse_state == MS_HOVER ? 10 : 0));
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
    if (check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
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
        if (app_get_active_window()->ui_data.backspace_pressed)
        {
            size_t textlen = strlen(textbox->text);
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
            if (strlen(textbox->text) + strlen(app_get_active_window()->ui_data.text_input) < UITEXT_MAX_LENGTH)
                strcat(textbox->text, app_get_active_window()->ui_data.text_input);
            app_get_active_window()->ui_data.text_input[0] = '\0';
        }
    }

    if (check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
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
    Vector2 text_size = renderer_query_text_size(textbox->text);
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, 2, textbox->mouse_state == MS_PRESS ? color_shift(textbox->color, 15) : (textbox->mouse_state == MS_HOVER ? color_shift(textbox->color, 10) : textbox->color));
    renderer_draw_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, 2, textbox->color);
    if (textbox->text[0] != '\0') renderer_draw_text(textbox->text, self->position.x + 6, self->position.y + (int)round((self->size.y - text_size.y) * 0.5), textbox->text_color);
    if (textbox->focused) renderer_draw_line(self->position.x + text_size.x + 6, self->position.y + 2, self->position.x + text_size.x + 6, self->position.y + self->size.y - 4, 2, textbox->text_color);
}
void _ui_textbox_destroy(UIElement* self)
{
    UITextbox* textbox = (UITextbox*)self;
    free(textbox);
}

void _ui_checkbox_update(UIElement* self)
{
    UICheckbox* checkbox = (UICheckbox*)self;
    if (check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            checkbox->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && checkbox->mouse_state == MS_PRESS)
        {
            checkbox->checked = !checkbox->checked;
            checkbox->mouse_state = MS_HOVER;
        }
        else if (checkbox->mouse_state == MS_NONE)
            checkbox->mouse_state = MS_HOVER;
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
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, color_shift(checkbox->checked_color, 10));
        else if (checkbox->mouse_state == MS_PRESS)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, color_shift(checkbox->checked_color, 15));
        Vector2 p1 = (Vector2){self->position.x + (int)round(self->size.x * 0.25), (int)round(self->position.y + self->size.y * 0.5)};
        Vector2 p2 = (Vector2){self->position.x + (int)round(self->size.x * 0.45), (int)round(self->position.y + self->size.y * 0.75)};
        Vector2 p3 = (Vector2){self->position.x + (int)round(self->size.x * 0.8), (int)round(self->position.y + self->size.y * 0.25)};
        renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 2, checkbox->unchecked_color);
        renderer_draw_line(p2.x, p2.y, p3.x, p3.y, 2, checkbox->unchecked_color);
    }
    else
    {
        if (checkbox->mouse_state == MS_NONE)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, checkbox->checked_color);
        else if (checkbox->mouse_state == MS_HOVER)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, color_shift(checkbox->checked_color, 10));
        else if (checkbox->mouse_state == MS_PRESS)
            renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, checkbox->corner_radius, color_shift(checkbox->checked_color, 15));
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
    if (check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            slider->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && slider->mouse_state == MS_PRESS)
            slider->mouse_state = MS_HOVER;
        else if (slider->mouse_state == MS_NONE)
            slider->mouse_state = MS_HOVER;
    }
    else if (slider->mouse_state != MS_PRESS)
        slider->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        slider->mouse_state = MS_NONE;

    if (slider->mouse_state == MS_PRESS)
        slider->value = clamp((input_get_mouse_position().x - self->position.x) / (double)self->size.x, 0.0, 1.0);
}
void _ui_slider_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_slider_render(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    int handle_thickness = (int)round(1.4 * slider->thickness);
    Color color = color_shift(slider->color, slider->mouse_state == MS_PRESS ? 15 : (slider->mouse_state == MS_HOVER ? 10 : 0));
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y + (int)round((self->size.y - slider->thickness) * 0.5), self->size.x, slider->thickness, slider->corner_radius, color);
    renderer_draw_filled_rounded_rect(self->position.x + (int)round(slider->value * self->size.x) - (int)round(handle_thickness * 0.5), self->position.y, handle_thickness, self->size.y, slider->corner_radius, slider->slider_color);
}
void _ui_slider_destroy(UIElement* self)
{
    UISlider* slider = (UISlider*)self;
    free(slider);
}

_UIDropdownItem* _ui_dropdownitem_create(UIElement* parent, UIConstraints constraints, Sint32 index, const char* text, Color color, Color text_color, Uint32 corner_radius, void (*on_click)(_UIDropdownItem* self))
{
    _UIDropdownItem* item = (_UIDropdownItem*)malloc(sizeof(_UIDropdownItem));
    UIElement* element = (UIElement*)item;
    
    element->parent = parent->parent;
    element->constraints = constraints;
    element->position = (Vector2){0, 0};
    element->size = (Vector2){0, 0};
    element->update = _ui_dropdownitem_update;
    element->recalculate = _ui_dropdownitem_recalculate;
    element->render = _ui_dropdownitem_render;
    element->destroy = _ui_dropdownitem_destroy;

    item->parent_dropdown = parent;
    item->dropdown_index = index;
    strcpy(item->text, text);
    item->color = color;
    item->text_color = text_color;
    item->corner_radius = corner_radius;
    item->mouse_state = MS_NONE;
    item->on_click = on_click;

    return item;
}
void _ui_dropdownitem_update(UIElement* self)
{
    _UIDropdownItem* item = (_UIDropdownItem*)self;
    if (check_collision_point_rect(input_get_mouse_position().x, input_get_mouse_position().y,
                                   self->position.x, self->position.y, self->size.x, self->size.y))
    {
        if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            item->mouse_state = MS_PRESS;
        else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && item->mouse_state == MS_PRESS)
        {
            item->mouse_state = MS_NONE;
            item->on_click(item);
        }
        else if (item->mouse_state == MS_NONE)
            item->mouse_state = MS_HOVER;
    }
    else if (item->mouse_state != MS_PRESS)
        item->mouse_state = MS_NONE;
    else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
        item->mouse_state = MS_NONE;
}
void _ui_dropdownitem_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
}
void _ui_dropdownitem_render(UIElement* self)
{
    _UIDropdownItem* item = (_UIDropdownItem*)self;
    renderer_draw_filled_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, item->corner_radius, item->mouse_state == MS_PRESS ? color_shift(item->color, 15) : (item->mouse_state == MS_HOVER ? color_shift(item->color, 10) : item->color));
    renderer_draw_rounded_rect(self->position.x, self->position.y, self->size.x, self->size.y, item->corner_radius, item->color);
    if (item->text[0] != '\0') renderer_draw_text(item->text, self->position.x + 6, self->position.y + (int)round((self->size.y - renderer_query_text_size(item->text).y) * 0.5), item->text_color);
}
void _ui_dropdownitem_destroy(UIElement* self)
{
    _UIDropdownItem* item = (_UIDropdownItem*)self;
    free(item);
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
}
void _ui_dropdown_recalculate(UIElement* sibling, UIElement* self)
{
    __ui_element_recalculate(sibling, self);
    UIDropdownList* dropdown = (UIDropdownList*)self;
    UIElement* top_item = (UIElement*)vector_get(dropdown->items, 0);
    top_item->recalculate(sibling, top_item);
    if (dropdown->expanded)
    {
        UIDropdownList* dropdown = (UIDropdownList*)self;
        for (size_t i = 1; i < dropdown->items->size; i++)
        {
            UIElement* item = (UIElement*)vector_get(dropdown->items, i);
            item->recalculate((UIElement*)vector_get(dropdown->items, i - 1), item);
        }
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
void _ui_dropdownitem_on_click(_UIDropdownItem* self)
{
    UIDropdownList* dropdown = (UIDropdownList*)self->parent_dropdown;
    if (self->dropdown_index == -1)
        dropdown->expanded = true;
    else
    {
        _UIDropdownItem* top_item = (_UIDropdownItem*)vector_get(dropdown->items, 0);
        strcpy(top_item->text, self->text);
        dropdown->expanded = false;
        dropdown->selected_item = self->dropdown_index;
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
                return (int)round(parent_position + constraint->value);
        default: //CT_ASPECT
            return -1;
    }
}