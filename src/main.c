#include <stdio.h>

#include "includes.h"
#include "debugmalloc.h"

#define FPS 60

void click(UIButton* self)
{
    app_request_close();
}

int main(int argc, char** argv)
{
    debugmalloc_log_file("log.txt");

    app_init();
    Window* main_window = window_create("Test", 800, 600, SDL_WINDOW_RESIZABLE);
    Window* window2 = window_create("Test2", 400, 300, 0); window_hide(window2);
    app_set_target_fps(FPS);

    Font* font = font_load("../assets/lucida.ttf", 20);
    renderer_set_default_font(font);

    renderer_set_target(window2->renderer);
    renderer_clear(CYAN);
    renderer_set_target(main_window->renderer);

    UIContainer* left_container = ui_create_container(window_get_main_container(main_window), (UIConstraints)
    {
        new_pixel_constraint(10),
        new_pixel_constraint(10),
        new_pixel_constraint(60),
        new_pixel_constraint(-20)
    });
    ui_create_panel(left_container, (UIConstraints)
    {
        new_relative_constraint(0),
        new_relative_constraint(0),
        new_relative_constraint(1),
        new_relative_constraint(1)
    }, GRAY, DARK_GRAY, 2, 2);
    ui_create_button(left_container, (UIConstraints)
    {
        new_center_constraint(),
        new_pixel_constraint(8),
        new_relative_constraint(0.7),
        new_aspect_constraint(1.0)
    }, "X", (Color){40, 40, 40, 255}, GRAY, click);
    ui_create_checkbox(left_container, (UIConstraints)
    {
        new_center_constraint(),
        new_center_constraint(),
        new_relative_constraint(0.7),
        new_aspect_constraint(1.0)
    }, (Color){50, 50, 50, 255}, GRAY);

    UIContainer* top_container = ui_create_container(window_get_main_container(main_window), (UIConstraints)
    {
        new_pixel_constraint(80),
        new_pixel_constraint(10),
        new_pixel_constraint(-90),
        new_pixel_constraint(60)
    });
    ui_create_panel(top_container, (UIConstraints)
    {
        new_pixel_constraint(0),
        new_pixel_constraint(0),
        new_relative_constraint(1),
        new_relative_constraint(1)
    }, GRAY, DARK_GRAY, 2, 2);
    ui_create_label(top_container, (UIConstraints)
    {
        new_center_constraint(),
        new_center_constraint(),
        new_pixel_constraint(0),
        new_pixel_constraint(0)
    }, "Coordinate Geometry", DARK_GRAY);
    ui_create_slider(top_container, (UIConstraints)
    {
        new_pixel_constraint(10),
        new_pixel_constraint(10),
        new_relative_constraint(0.3),
        new_pixel_constraint(30)
    }, 0.5, DARK_GRAY, color_shift(DARK_GRAY, 40));
    
    while (!main_window->close_requested)
    {
        app_update();

        renderer_clear(GRAY);

        app_render();
    }
    font_unload(font);

    app_close();
    SDL_Quit();

    debugmalloc_dump();

    return 0;
}