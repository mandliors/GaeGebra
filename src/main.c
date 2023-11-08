/**
 * @file main.c
 * @author Örs Mándli (mandliors@gmail.com)
 * @brief This is the entry point of the application.
 * @version 0.1
 * @date 2023-11-05
 * 
 * @copyright Copyright (c) 2023
 */

#include "includes.h"
#include "utils/vector/vector.h"

#define FPS 60

void on_pointer_clicked(UIButton* self);
void on_point_clicked(UIButton* self);
void on_line_clicked(UIButton* self);
void on_circle_clicked(UIButton* self);

void on_filemenu_clicked(UISplitButton* self, Sint32 index);
void on_editmenu_clicked(UISplitButton* self, Sint32 index);
void on_canvas_size_changed(UIContainer* self, SDL_Point size);

typedef enum State
{
    STATE_POINTER,
    STATE_POINT,
    STATE_LINE,
    STATE_CIRCLE
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

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    UIContainer* toolbar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 30p 1r 50p"), NULL);
    ui_create_panel(toolbar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(40), WHITE, 0, 0);
    ui_create_button(toolbar, constraints_from_string("10p c 100p 0.8r"), "Pointer", color_from_grayscale(80), WHITE, on_pointer_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Point", color_from_grayscale(80), WHITE, on_point_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Line", color_from_grayscale(80), WHITE, on_line_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Circle", color_from_grayscale(80), WHITE, on_circle_clicked);
    ui_create_textbox(toolbar, constraints_from_string("10o c 200p 0.8r"), "Hello World!", WHITE, BLACK, NULL);
    
    UIContainer* menubar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 30p"), NULL);
    ui_create_panel(menubar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), WHITE, 0, 0);
    ui_create_splitbutton(menubar, constraints_from_string("0p 0p 1r 1r"), "File;Close", color_from_grayscale(180), BLACK, on_filemenu_clicked, true);
    ui_create_splitbutton(menubar, constraints_from_string("0o 0p 1r 1r"), "Edit;Close", color_from_grayscale(180), BLACK, on_editmenu_clicked, true);

    UIContainer* canvas = ui_create_container(window_get_main_container(window), constraints_from_string("0p 90p 1r -100p"), on_canvas_size_changed);
    cs = coordinate_system_create(vector2_create(canvas->base.position.x, canvas->base.position.y),
                                  vector2_create(canvas->base.size.x, canvas->base.size.y),
                                  vector2_create(0.5, 0.5));
    coordinate_system_add_shape(cs, (Shape*)point_create(vector2_create(300, 200)));
    coordinate_system_add_shape(cs, (Shape*)line_create(point_create(vector2_create(100, 100)), point_create(vector2_create(200, 200))));
    coordinate_system_add_shape(cs, (Shape*)circle_create(point_create(vector2_create(450, 450)), 50));

    while (!window->close_requested)
    {
        //update
        app_update();

        Vector2 p1 = vector2_from_point(input_get_mouse_position());
        Vector2 p2 = screen_to_coordinates(cs, p1);
        Vector2 p3 = coordinates_to_screen(cs, p2);
        switch (state)
        {
        case STATE_POINTER:
            if (coordinate_system_get_hovered_shape(cs, vector2_from_point(input_get_mouse_position())) != NULL)
                SDL_SetCursor(cursor);
            else
            {
                if (input_is_mouse_button_down(SDL_BUTTON_LEFT))
                    coordinate_system_translate(cs, vector2_from_point(input_get_mouse_motion()));
                SDL_SetCursor(SDL_GetDefaultCursor());
            }
            break;

        case STATE_POINT:
            if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                Point* point = point_create(vector2_from_point(input_get_mouse_position()));
                coordinate_system_add_shape(cs, (Shape*)point);
            }
            break;
        
        case STATE_LINE:
            break;

        case STATE_CIRCLE:
            if (input_is_key_released(SDL_SCANCODE_U))
                coordinate_system_zoom(cs, 1.1);
            if (input_is_key_released(SDL_SCANCODE_J))
                coordinate_system_zoom(cs, 1.0 / 1.1);
            printf("(%d, %d) -> (%d, %d) -> (%d, %d)\n", (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y);
            break;
        }

        //draw
        renderer_clear(WHITE);
        app_set_target(window);
        coordinate_system_draw(cs);
        
        app_render();
    }
    coordinate_system_free(cs);    
    SDL_FreeCursor(cursor);

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