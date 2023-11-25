/**
 * @file main.c
 * @author Örs Mándli (mandliors@gmail.com)
 * @brief This is the entry point of the application.
 * @version 0.1
 * @date 2023-11-25
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
void on_parallel_clicked(UIButton* self);
void on_perpendicular_clicked(UIButton* self);
void on_angle_bisector_clicked(UIButton* self);
void on_tangent_clicked(UIButton* self);

void on_open_button_clicked(UIButton* self);
void on_save_button_clicked(UIButton* self);
void on_cancel_button_clicked(UIButton* self);

void on_filemenu_clicked(UISplitButton* self, Sint32 index);
void on_editmenu_clicked(UISplitButton* self, Sint32 index);
void on_canvas_size_changed(UIContainer* self, SDL_Point size);

typedef enum State
{
    STATE_POINTER,
    STATE_CS_DRAGGED,

    STATE_POINT,
    
    STATE_LINE,
    STATE_LINE_POINT1_PLACED,

    STATE_CIRCLE,
    STATE_CIRCLE_CENTER_PLACED,

    STATE_PARALLEL,
    STATE_PARALLEL_LINE_SELECTED,

    STATE_PERPENDICULAR,
    STATE_PERPENDICULAR_LINE_SELECTED,

    STATE_ANGLE_BISECTOR,
    STATE_ANGLE_BISECTOR_LINE1_SELECTED,

    STATE_TANGENT,
    STATE_TANGENT_LINE_SELECTED,

    STATE_OPENING,
    STATE_SAVEING
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

    UIContainer* main_container = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 1r"), NULL);
    UIContainer* toolbar = ui_create_container(main_container, constraints_from_string("0p 30p 1r 50p"), NULL);
    ui_create_panel(toolbar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(40), WHITE, 0, 0);
    ui_create_button(toolbar, constraints_from_string("10p c 100p 0.8r"), "Pointer", color_from_grayscale(80), WHITE, on_pointer_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 80p 0.8r"), "Point", color_from_grayscale(80), WHITE, on_point_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 70p 0.8r"), "Line", color_from_grayscale(80), WHITE, on_line_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 90p 0.8r"), "Circle", color_from_grayscale(80), WHITE, on_circle_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Parallel", color_from_grayscale(80), WHITE, on_parallel_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 130p 0.8r"), "Perpendicular", color_from_grayscale(80), WHITE, on_perpendicular_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 140p 0.8r"), "Angle Bisector", color_from_grayscale(80), WHITE, on_angle_bisector_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 110p 0.8r"), "Tangent", color_from_grayscale(80), WHITE, on_tangent_clicked);
    
    UIContainer* menubar = ui_create_container(main_container, constraints_from_string("0p 0p 1r 30p"), NULL);
    ui_create_panel(menubar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), WHITE, 0, 0);
    UISplitButton* file_sb = ui_create_splitbutton(menubar, constraints_from_string("0p 0p 1r 1r"), "File;Open;Save", color_from_grayscale(180), BLACK, on_filemenu_clicked, true);
    ui_create_splitbutton(menubar, constraints_from_string("0o 0p 1r 1r"), "Edit;Clear;Close", color_from_grayscale(180), BLACK, on_editmenu_clicked, true);

    UIContainer* save_container = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 1r"), NULL);
    ui_create_panel(save_container, constraints_from_string("0p 0p 1r 1r"), color_fade(BLACK, 0.7), WHITE, 0, 0);
    UIContainer* save_menu = ui_create_container(save_container, constraints_from_string("c c 0.3r 200p"), NULL);
    ui_create_panel(save_menu, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), BLACK, 2, 0);
    ui_create_label(save_menu, constraints_from_string("0.05r 15p 0.9r 80p"), "Save As:", BLACK);
    ui_create_textbox(save_menu, constraints_from_string("c 15o 0.9r 60p"), "project", WHITE, BLACK, NULL);
    ui_create_button(save_menu, constraints_from_string("0.05r 15o 0.44r 50p"), "Cancel", color_from_grayscale(80), WHITE, on_cancel_button_clicked);
    ui_create_button(save_menu, constraints_from_string("0.51r -50o 0.44r 50p"), "Save", color_from_grayscale(80), WHITE, on_save_button_clicked);
    ui_hide_element((UIElement*)save_container);

    UIContainer* open_container = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 1r"), NULL);
    ui_create_panel(open_container, constraints_from_string("0p 0p 1r 1r"), color_fade(BLACK, 0.7), WHITE, 0, 0);
    UIContainer* open_menu = ui_create_container(open_container, constraints_from_string("c c 0.3r 200p"), NULL);
    ui_create_panel(open_menu, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), BLACK, 2, 0);
    ui_create_label(open_menu, constraints_from_string("0.05r 15p 0.9r 80p"), "Open:", BLACK);
    ui_create_textbox(open_menu, constraints_from_string("c 15o 0.9r 60p"), "project", WHITE, BLACK, NULL);
    ui_create_button(open_menu, constraints_from_string("0.05r 15o 0.44r 50p"), "Cancel", color_from_grayscale(80), WHITE, on_cancel_button_clicked);
    ui_create_button(open_menu, constraints_from_string("0.51r -50o 0.44r 50p"), "Open", color_from_grayscale(80), WHITE, on_open_button_clicked);
    ui_hide_element((UIElement*)open_container);

    UIContainer* canvas = ui_create_container(main_container, constraints_from_string("0p 80p 1r -80p"), on_canvas_size_changed);
    cs = coordinate_system_create(vector2_create(canvas->base.position.x, canvas->base.position.y),
                                  vector2_create(canvas->base.size.x, canvas->base.size.y),
                                  vector2_create(0.5, 0.5));

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
                if (hovered_shape != NULL)
                {
                    if (input_is_key_down(SDL_SCANCODE_LCTRL)  || input_is_key_down(SDL_SCANCODE_RCTRL) ||
                        input_is_key_down(SDL_SCANCODE_LSHIFT) || input_is_key_down(SDL_SCANCODE_RSHIFT))
                        coordinate_system_select_shape(cs, hovered_shape);
                    else
                    {
                        if (!hovered_shape->selected)
                            coordinate_system_deselect_shapes(cs);
                        coordinate_system_select_shape(cs, hovered_shape);
                        coordinate_system_drag_selected_shapes(cs, true);
                    }
                }
                else
                {
                    coordinate_system_deselect_shapes(cs);
                    if (coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
                        state = STATE_CS_DRAGGED;
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
                coordinate_system_drag_selected_shapes(cs, false);
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
                coordinate_system_drag_selected_shapes(cs, true);
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
            }
            break;
        
        case STATE_LINE:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, (Shape*)hovered_shape);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
                else
                {
                    Shape* point = (Shape*)point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Vector* shapes = coordinate_system_get_selected_shapes(cs);
                Point* first_point = (Point*)vector_get(shapes, 0);
                vector_destroy(shapes);
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                line_create(cs, first_point, (Point*)placing_point);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_drag_selected_shapes(cs, true);
                state = STATE_LINE_POINT1_PLACED;
            }
            break;
        case STATE_LINE_POINT1_PLACED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && !hovered_shape->dragged && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_drag_selected_shapes(cs, false);
                    coordinate_system_deselect_shapes(cs);
                    Line* line = (Line*)vector_get(cs->shapes, vector_size(cs->shapes) - 1);
                    line->p2 = (Point*)hovered_shape;
                    coordinate_system_destroy_shape(cs, vector_get(cs->shapes, vector_size(cs->shapes) - 2));
                }
                else
                {
                    coordinate_system_deselect_shapes(cs);
                    Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                    coordinate_system_select_shape(cs, (Shape*)placing_point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
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
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, (Shape*)hovered_shape);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
                else
                {
                    Shape* point = (Shape*)point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Vector* shapes = coordinate_system_get_selected_shapes(cs);
                Point* first_point = (Point*)vector_get(shapes, 0);
                vector_destroy(shapes);
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                circle_create(cs, first_point, (Point*)placing_point);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_drag_selected_shapes(cs, true);
                state = STATE_CIRCLE_CENTER_PLACED;
            }
            break;
        case STATE_CIRCLE_CENTER_PLACED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && !hovered_shape->dragged && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_drag_selected_shapes(cs, false);
                    coordinate_system_deselect_shapes(cs);
                    Circle* circle = (Circle*)vector_get(cs->shapes, vector_size(cs->shapes) - 1);
                    circle->perimeter_point = (Point*)hovered_shape;
                    coordinate_system_destroy_shape(cs, vector_get(cs->shapes, vector_size(cs->shapes) - 2));
                }
                else
                {                    
                    coordinate_system_deselect_shapes(cs);
                    Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                    coordinate_system_select_shape(cs, (Shape*)placing_point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
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
        
        case STATE_PARALLEL:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_LINE)
                {
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, (Shape*)hovered_shape);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Vector* shapes = coordinate_system_get_selected_shapes(cs);
                Line* line = (Line*)vector_get(shapes, 0);
                vector_destroy(shapes);
                if (line == NULL || line->base.type != ST_LINE)
                    break;
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                parallel_create(cs, line, (Point*)placing_point);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_drag_selected_shapes(cs, true);
                state = STATE_PARALLEL_LINE_SELECTED;
            }
            break;
        case STATE_PARALLEL_LINE_SELECTED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && !hovered_shape->dragged && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_drag_selected_shapes(cs, false);
                    coordinate_system_deselect_shapes(cs);
                    Parallel* parallel = (Parallel*)vector_get(cs->shapes, vector_size(cs->shapes) - 1);
                    parallel->point = (Point*)hovered_shape;
                    coordinate_system_destroy_shape(cs, vector_get(cs->shapes, vector_size(cs->shapes) - 2));
                }
                else
                {
                    coordinate_system_deselect_shapes(cs);
                    Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                    coordinate_system_select_shape(cs, (Shape*)placing_point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                state = STATE_PARALLEL;
            }
            else if (input_is_key_released(SDL_SCANCODE_ESCAPE) || (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && !coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position()))))
            {
                shape_destroy(cs, vector_pop_back(cs->shapes));
                shape_destroy(cs, vector_pop_back(cs->shapes));
                coordinate_system_deselect_shapes(cs);
                state = STATE_PARALLEL;
            }
            break;

        case STATE_PERPENDICULAR:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_LINE)
                {
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, (Shape*)hovered_shape);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Vector* shapes = coordinate_system_get_selected_shapes(cs);
                Line* line = (Line*)vector_get(shapes, 0);
                vector_destroy(shapes);
                if (line == NULL || line->base.type != ST_LINE)
                    break;
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                perpendicular_create(cs, line, (Point*)placing_point);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_drag_selected_shapes(cs, true);
                state = STATE_PERPENDICULAR_LINE_SELECTED;
            }
            break;
        case STATE_PERPENDICULAR_LINE_SELECTED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && !hovered_shape->dragged && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_drag_selected_shapes(cs, false);
                    coordinate_system_deselect_shapes(cs);
                    Perpendicular* perpendicular = (Perpendicular*)vector_get(cs->shapes, vector_size(cs->shapes) - 1);
                    perpendicular->point = (Point*)hovered_shape;
                    coordinate_system_destroy_shape(cs, vector_get(cs->shapes, vector_size(cs->shapes) - 2));
                }
                else
                {
                    coordinate_system_deselect_shapes(cs);
                    Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                    coordinate_system_select_shape(cs, (Shape*)placing_point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                state = STATE_PERPENDICULAR;
            }
            else if (input_is_key_released(SDL_SCANCODE_ESCAPE) || (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && !coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position()))))
            {
                shape_destroy(cs, vector_pop_back(cs->shapes));
                shape_destroy(cs, vector_pop_back(cs->shapes));
                coordinate_system_deselect_shapes(cs);
                state = STATE_PERPENDICULAR;
            }
            break;

        case STATE_ANGLE_BISECTOR:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_LINE)
                {
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, (Shape*)hovered_shape);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Vector* shapes = coordinate_system_get_selected_shapes(cs);
                Line* line = (Line*)vector_get(shapes, 0);
                vector_destroy(shapes);
                if (line == NULL || line->base.type != ST_LINE)
                    break;
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                angle_bisector_create(cs, line, NULL);
                state = STATE_ANGLE_BISECTOR_LINE1_SELECTED;
            }
            break;
        case STATE_ANGLE_BISECTOR_LINE1_SELECTED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_LINE)
                {
                    coordinate_system_drag_selected_shapes(cs, false);
                    coordinate_system_deselect_shapes(cs);
                    AngleBisector* angle_bisector = (AngleBisector*)vector_get(cs->shapes, vector_size(cs->shapes) - 1);
                    angle_bisector->line2 = (Line*)hovered_shape;
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                state = STATE_ANGLE_BISECTOR;
            }
            else if (input_is_key_released(SDL_SCANCODE_ESCAPE) || (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && !coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position()))))
            {
                shape_destroy(cs, vector_pop_back(cs->shapes));
                coordinate_system_deselect_shapes(cs);
                state = STATE_ANGLE_BISECTOR;
            }
            break;
        
        case STATE_TANGENT:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && hovered_shape->type == ST_CIRCLE)
                {
                    coordinate_system_deselect_shapes(cs);
                    coordinate_system_select_shape(cs, (Shape*)hovered_shape);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Vector* shapes = coordinate_system_get_selected_shapes(cs);
                Circle* circle = (Circle*)vector_get(shapes, 0);
                vector_destroy(shapes);
                if (circle == NULL || circle->base.type != ST_CIRCLE)
                    break;
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                Point* placing_point = point_create(cs, screen_to_coordinates(cs, vector2_from_point(input_get_mouse_position())));
                tangent_create(cs, circle, (Point*)placing_point);
                coordinate_system_select_shape(cs, (Shape*)placing_point);
                coordinate_system_drag_selected_shapes(cs, true);
                state = STATE_TANGENT_LINE_SELECTED;
            }
            break;
        case STATE_TANGENT_LINE_SELECTED:
            if (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position())))
            {
                Shape* hovered_shape = coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position()));
                if (hovered_shape != NULL && !hovered_shape->dragged && hovered_shape->type == ST_POINT)
                {
                    coordinate_system_drag_selected_shapes(cs, false);
                    coordinate_system_deselect_shapes(cs);
                    Tangent* tangent = (Tangent*)vector_get(cs->shapes, vector_size(cs->shapes) - 1);
                    tangent->point = (Point*)hovered_shape;
                    coordinate_system_destroy_shape(cs, vector_get(cs->shapes, vector_size(cs->shapes) - 2));
                }
                else
                {
                    coordinate_system_deselect_shapes(cs);
                    Point* placing_point = vector_get(cs->shapes, vector_size(cs->shapes) - 2);
                    coordinate_system_select_shape(cs, (Shape*)placing_point);
                    coordinate_system_drag_selected_shapes(cs, true);
                }
            }
            else if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                coordinate_system_drag_selected_shapes(cs, false);
                coordinate_system_deselect_shapes(cs);
                state = STATE_TANGENT;
            }
            else if (input_is_key_released(SDL_SCANCODE_ESCAPE) || (input_is_mouse_button_pressed(SDL_BUTTON_LEFT) && !coordinate_system_is_hovered(cs, vector2_from_point(input_get_mouse_position()))))
            {
                shape_destroy(cs, vector_pop_back(cs->shapes));
                shape_destroy(cs, vector_pop_back(cs->shapes));
                coordinate_system_deselect_shapes(cs);
                state = STATE_TANGENT;
            }
            break;

        case STATE_SAVEING:
            if (input_is_key_released(SDL_SCANCODE_ESCAPE))
                state = STATE_POINTER;
            else
                ui_show_element((UIElement*)save_container);
            break;
        
        case STATE_OPENING:
            if (input_is_key_released(SDL_SCANCODE_ESCAPE))
                state = STATE_POINTER;
            else
                ui_show_element((UIElement*)open_container);
            break;
        }
        
        if (input_is_key_down(SDL_SCANCODE_LCTRL) || input_is_key_down(SDL_SCANCODE_RCTRL))
        {
            if (input_is_key_pressed(SDL_SCANCODE_A))
                coordinate_system_select_all_shapes(cs);
            else if (input_is_key_pressed(SDL_SCANCODE_O))
                on_filemenu_clicked(file_sb, 0);
            else if (input_is_key_pressed(SDL_SCANCODE_S))
                on_filemenu_clicked(file_sb, 1);
        }
        else if (input_is_key_pressed(SDL_SCANCODE_ESCAPE))
            coordinate_system_deselect_shapes(cs);
        else if (input_is_key_released(SDL_SCANCODE_DELETE))
                coordinate_system_delete_selected_shapes(cs);

        SDL_SetCursor(cursor_default);
        if (state != STATE_OPENING && state != STATE_SAVEING)
        {
            if (coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position())) ||
                state == STATE_CS_DRAGGED)
                SDL_SetCursor(cursor_hand);
        }

        coordinate_system_zoom(cs, 1.0 + input_get_mouse_wheel_delta() / 100.0 * MOUSE_WHEEL_SENSITIVITY);
        coordinate_system_update(cs);       
        
        //draw
        app_set_target(window);
        renderer_clear(WHITE);
        coordinate_system_draw(cs);
        
        //fps (temporary)
        //static char buffer[10];
        //sprintf(buffer, "Fps: %.0lf", 1.0 / app_get_delta_time());
        //renderer_draw_text(buffer, 10, 90, BLACK);

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
void on_parallel_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_PARALLEL;
}
void on_perpendicular_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_PERPENDICULAR;
}
void on_angle_bisector_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_ANGLE_BISECTOR;
}
void on_tangent_clicked(UIButton* self __attribute__((unused)))
{
    state = STATE_TANGENT;
}

void on_open_button_clicked(UIButton* self)
{
    char* path = ((UITextbox*)vector_get(((UIContainer*)self->base.parent)->children, 2))->text;
    if (path[0] == '\0')
        return;
    char* extension = ".gae";
    char* new_path = malloc(strlen(path) + strlen(extension) + 1);
    strcpy(new_path, path);
    strcat(new_path, extension);

    CoordinateSystem* new_cs = coordinate_system_load(new_path);
    if (new_cs == NULL)
    {
        free(new_path);
        return;
    }
    new_cs->position = cs->position;
    new_cs->size = cs->size;
    new_cs->origin = vector2_create(0.5, 0.5);
    coordinate_system_destroy(cs);
    cs = new_cs;
    ui_hide_element((UIElement*)self->base.parent->parent);
    state = STATE_POINTER;

    free(new_path);
}
void on_save_button_clicked(UIButton* self)
{
    char* path = ((UITextbox*)vector_get(((UIContainer*)self->base.parent)->children, 2))->text;
    if (path[0] == '\0')
        return;
    char* extension = ".gae";
    char* new_path = malloc(strlen(path) + strlen(extension) + 1);
    strcpy(new_path, path);
    strcat(new_path, extension);

    coordinate_system_save(cs, new_path);
    ui_hide_element((UIElement*)self->base.parent->parent);
    state = STATE_POINTER;

    free(new_path);
}
void on_cancel_button_clicked(UIButton* self)
{
    ui_hide_element((UIElement*)self->base.parent->parent);
    state = STATE_POINTER;
}

void on_filemenu_clicked(UISplitButton* self __attribute__((unused)), Sint32 index __attribute__((unused)))
{
    if (index == 0)
        state = STATE_OPENING;
    else if (index == 1)
        state = STATE_SAVEING;
}
void on_editmenu_clicked(UISplitButton* self __attribute__((unused)), Sint32 index __attribute__((unused)))
{
    if (index == 0)
        coordinate_system_clear(cs);
    else if (index == 1)
        app_request_close();
}
void on_canvas_size_changed(UIContainer* self, SDL_Point size)
{
    coordinate_system_update_dimensions(cs, vector2_create(self->base.position.x, self->base.position.y),
                                            vector2_create(size.x, size.y));
}