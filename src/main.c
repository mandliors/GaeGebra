/**
 * @file main.c
 * @author Örs Mándli (mandliors@gmail.com)
 * @brief This is the entry point of the application.
 * @version 0.1
 * @date 2023-11-05
 * 
 * @copyright Copyright (c) 2023
 */

#include "app/app.h"
#include "color/color.h"
#include "font/font.h"
#include "geometry/coordinate_system/coordinate_system.h"
#include "geometry/shape/shape.h"
#include "geometry/vector2/vector2.h"
#include "input/input.h"
#include "renderer/renderer.h"
#include "texture/texture.h"
#include "ui/ui.h"
#include "ui/ui_constraint/ui_constraint.h"
#include "ui/ui_element/ui_element.h"
#include "utils/math/math.h"
#include "utils/vector/vector.h"
#include "window/window.h"

#define FPS 60
#define MOUSE_WHEEL_SENSITIVITY 5

void on_pointer_clicked(UIButton* self);
void on_point_clicked(UIButton* self);
void on_line_clicked(UIButton* self);
void on_circle_clicked(UIButton* self);

void on_filemenu_clicked(UISplitButton* self, Sint32 index);
void on_editmenu_clicked(UISplitButton* self, Sint32 index);
void on_canvas_size_changed(UIContainer* self, SDL_Point size);

/* TODO: add shape type, put points back to the shapes vector
         add selected bool to shapes
         check intersection between shapes, add multiple types of points (intersections)
         shapes should have indices to the defining points?
*/

typedef enum State
{
    STATE_POINTER,
    STATE_CS_DRAGGED,

    STATE_POINT,
    
    STATE_LINE,
    STATE_LINE_POINT1_PLACED,

    STATE_CIRCLE,
    STATE_CIRCLE_CENTER_PLACED

} State;

CoordinateSystem* cs;
State state = STATE_POINTER;

int main(void)
{
    app_init();
    app_set_target_fps(FPS);

    Window* window = window_create("GaeGebra", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_MaximizeWindow(window->window);

    Font* font = font_load("../assets/LiberationSerif.ttf", 20);
    renderer_set_default_font(font);

    SDL_Cursor* cursor_hand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Cursor* cursor_default = SDL_GetDefaultCursor();

    UIContainer* toolbar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 30p 1r 50p"), NULL);
    ui_create_panel(toolbar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(40), WHITE, 0, 0);
    ui_create_button(toolbar, constraints_from_string("10p c 100p 0.8r"), "Pointer", color_from_grayscale(80), WHITE, on_pointer_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Point", color_from_grayscale(80), WHITE, on_point_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Line", color_from_grayscale(80), WHITE, on_line_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Circle", color_from_grayscale(80), WHITE, on_circle_clicked);
    ui_create_textbox(toolbar, constraints_from_string("10o c 200p 0.8r"), "bing chilling", WHITE, BLACK, NULL);
    
    UIContainer* menubar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 30p"), NULL);
    ui_create_panel(menubar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), WHITE, 0, 0);
    ui_create_splitbutton(menubar, constraints_from_string("0p 0p 1r 1r"), "File;Close", color_from_grayscale(180), BLACK, on_filemenu_clicked, true);
    ui_create_splitbutton(menubar, constraints_from_string("0o 0p 1r 1r"), "Edit;Close", color_from_grayscale(180), BLACK, on_editmenu_clicked, true);

    UIContainer* canvas = ui_create_container(window_get_main_container(window), constraints_from_string("0p 90p 1r -100p"), on_canvas_size_changed);
    cs = coordinate_system_create(vector2_create(canvas->base.position.x, canvas->base.position.y),
                                  vector2_create(canvas->base.size.x, canvas->base.size.y),
                                  vector2_create(0.5, 0.5));
    point_create(cs, vector2_create(-12, 15));
    line_create(cs, point_create(cs, vector2_create(-10, 10)), point_create(cs, vector2_create(-2, 2)));
    circle_create(cs, point_create(cs, vector2_create(2, 1)), point_create(cs, vector2_create(5, 1)));

    while (!window->close_requested)
    {
        //update
        app_update();

        switch (state)
        {
        case STATE_POINTER:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                coordinate_system_deselect_shapes(cs);
                if (hovered_shape != NULL)
                {
                    coordinate_system_select_shape(cs, hovered_shape);
                    coordinate_system_set_dragged_shape(cs, hovered_shape);
                }
                else if (coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
                    state = STATE_CS_DRAGGED;
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
                coordinate_system_set_dragged_shape(cs, NULL);
            else if (input_is_key_released(SDL_SCANCODE_DELETE))
                coordinate_system_delete_selected_shapes(cs);
            break;
        case STATE_CS_DRAGGED:
            if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
                state = STATE_POINTER;
            else
                coordinate_system_translate(cs, vector2_from_point(input_get_mouse_motion()));
            break;

        case STATE_POINT:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* point = (Shape*)point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                coordinate_system_deselect_shapes(cs);
                coordinate_system_select_shape(cs, point);
                coordinate_system_set_dragged_shape(cs, point);
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                coordinate_system_deselect_shapes(cs);
                coordinate_system_set_dragged_shape(cs, NULL);
            }
            break;
        
        case STATE_LINE:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* point = (Shape*)point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                coordinate_system_deselect_shapes(cs);
                coordinate_system_select_shape(cs, point);
                coordinate_system_set_dragged_shape(cs, point);
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                coordinate_system_deselect_shapes(cs);
                coordinate_system_set_dragged_shape(cs, NULL);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                coordinate_system_set_dragged_shape(cs, (Shape*)placing_point);
                line_create(cs, (Point*)vector_get(cs->shapes, vector_size(cs->shapes) - 2), (Point*)placing_point);
                state = STATE_LINE_POINT1_PLACED;
            }
            break;
        case STATE_LINE_POINT1_PLACED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_set_dragged_shape(cs, (Shape*)placing_point);
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_deselect_shapes(cs);
                coordinate_system_set_dragged_shape(cs, NULL);
                state = STATE_LINE;
            }
            else if (input_is_key_released(SDL_SCANCODE_ESCAPE) || (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && !coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position()))))
            {
                shape_destroy(cs, vector_pop_back(cs->shapes));
                shape_destroy(cs, vector_pop_back(cs->shapes));
                coordinate_system_deselect_shapes(cs);
                state = STATE_LINE;
            }
            break;

        case STATE_CIRCLE:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* point = (Shape*)point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                coordinate_system_deselect_shapes(cs);
                coordinate_system_select_shape(cs, point);
                coordinate_system_set_dragged_shape(cs, point);
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                coordinate_system_deselect_shapes(cs);
                coordinate_system_set_dragged_shape(cs, NULL);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                coordinate_system_set_dragged_shape(cs, (Shape*)placing_point);
                circle_create(cs, (Point*)vector_get(cs->shapes, vector_size(cs->shapes) - 2), (Point*)placing_point);
                state = STATE_CIRCLE_CENTER_PLACED;
            }
            break;
        case STATE_CIRCLE_CENTER_PLACED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_set_dragged_shape(cs, (Shape*)placing_point);
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_deselect_shapes(cs);
                coordinate_system_set_dragged_shape(cs, NULL);
                state = STATE_CIRCLE;
            }
            else if (input_is_key_released(SDL_SCANCODE_ESCAPE) || (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && !coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position()))))
            {
                shape_destroy(cs, vector_pop_back(cs->shapes));
                shape_destroy(cs, vector_pop_back(cs->shapes));
                coordinate_system_deselect_shapes(cs);
                state = STATE_CIRCLE;
            }
            break;
        }
        SDL_SetCursor(cursor_default);
        if (coordinate_system_get_dragged_shape(cs) != NULL || state == STATE_CS_DRAGGED)
            SDL_SetCursor(cursor_hand);
        else if (coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position())) != NULL)
            SDL_SetCursor(cursor_hand);

        coordinate_system_zoom(cs, 1.0 + input_get_mouse_wheel_delta() / 100.0 * MOUSE_WHEEL_SENSITIVITY);
        coordinate_system_update(cs);       
        //draw
        app_set_target(window);
        renderer_clear(WHITE);
        coordinate_system_draw(cs);
        
        //fps (temporary)
        static char buffer[10];
        sprintf(buffer, "Fps: %.0lf", 1.0 / app_get_delta_time());
        renderer_draw_text(buffer, 10, 90, BLACK);

        app_render();
    }
    coordinate_system_destroy(cs);
    SDL_FreeCursor(cursor_hand);

    app_close();
    return 0;
}

void on_pointer_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_POINTER;
}
void on_point_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_POINT;
}
void on_line_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_LINE;
}
void on_circle_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_CIRCLE;
}

void on_filemenu_clicked(UISplitButton* self __attribute__((unused)), Sint32 index __attribute__((unused)))
{
    if (index == 0)
        app_request_close();
}
void on_editmenu_clicked(UISplitButton* self __attribute__((unused)), Sint32 index __attribute__((unused)))
{
    if (index == 0)
        app_request_close();
}
void on_canvas_size_changed(UIContainer* self, SDL_Point size)
{
    coordinate_system_update_dimensions(cs, vector2_create(self->base.position.x, self->base.position.y),
                                            vector2_create(size.x, size.y));
}