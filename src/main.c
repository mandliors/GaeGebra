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

typedef enum State
{
    STATE_POINTER,
    STATE_POINT,
    STATE_LINE,
    STATE_CIRCLE
} State;

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

    UIContainer* toolbar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 30p 1r 60p"));
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Pointer", color_from_grayscale(200), BLACK, on_pointer_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Point", color_from_grayscale(200), BLACK, on_point_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Line", color_from_grayscale(200), BLACK, on_line_clicked);
    ui_create_button(toolbar, constraints_from_string("10o c 100p 0.8r"), "Circle", color_from_grayscale(200), BLACK, on_circle_clicked);
    ui_create_textbox(toolbar, constraints_from_string("10o c 200p 0.8r"), "Hello World!", WHITE, BLACK, NULL);
    
    UIContainer* menubar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 30p"));
    ui_create_panel(menubar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), WHITE, 0, 0);
    ui_create_splitbutton(menubar, constraints_from_string("0p 0p 1r 1r"), "File;Close", color_from_grayscale(180), BLACK, on_filemenu_clicked, true);
    ui_create_splitbutton(menubar, constraints_from_string("0o 0p 1r 1r"), "Edit;Close", color_from_grayscale(180), BLACK, on_editmenu_clicked, true);

    Vector* shapes = vector_create(0);

    vector_push_back(shapes, point_create(vector2_create(300, 200)));
    vector_push_back(shapes, line_create(point_create(vector2_create(100, 100)), point_create(vector2_create(200, 200))));
    vector_push_back(shapes, circle_create(point_create(vector2_create(450, 450)), 50));

    while (!window->close_requested)
    {
        //update
        app_update();

        switch (state)
        {
        case STATE_POINTER:
            for (size_t i = 0; i < vector_size(shapes); i++)
            {
                Shape* shape = vector_get(shapes, i);
                if (shape->overlap_point(shape, vector2_from_point(input_get_mouse_position())))
                {
                    SDL_SetCursor(cursor);
                    break;
                }
                else
                    SDL_SetCursor(SDL_GetDefaultCursor());
            }
            break;

        case STATE_POINT:
            if (input_is_mouse_button_released(SDL_BUTTON_LEFT))
            {
                Point* point = point_create(vector2_from_point(input_get_mouse_position()));
                vector_push_back(shapes, point);
            }
            break;
        
        case STATE_LINE:
            break;

        case STATE_CIRCLE:
            break;
        }

        //draw
        app_set_target(window);
        renderer_clear(WHITE);
        for (size_t i = 0; i < vector_size(shapes); i++)
        {
            Shape* shape = vector_get(shapes, i);
            shape->draw(shape);
        }
        
        app_render();
    }

    for (size_t i = 0; i < vector_size(shapes); i++)
    {
        Shape* shape = vector_get(shapes, i);
        shape->free(shape);
    }
    vector_free(shapes);

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