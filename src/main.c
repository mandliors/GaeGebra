#include <stdio.h>

#include "includes.h"

#define FPS 60

void clicked(UIButton* self)
{
    app_request_close();
}

int main(void)
{
    app_init();
    app_set_target_fps(FPS);

    Window* main_window = window_create("Test", 800, 600, SDL_WINDOW_RESIZABLE);

    Font* font = font_load("../assets/LiberationSerif.ttf", 20);
    renderer_set_default_font(font);

    UIContainer* left_container = ui_create_container(window_get_main_container(main_window), constraints_from_string("10p 10p 60p -20p"));
    ui_create_panel(left_container, constraints_from_string("0r 0r 1r 1r"), GRAY, DARK_GRAY, 2, 2);
    ui_create_button(left_container, constraints_from_string("c 8p 0.7r 1.0a"), "X", (Color){40, 40, 40, 255}, GRAY, clicked);
    ui_create_button(left_container, constraints_from_string("c 10o 0.7r 1.0a"), "_", (Color){40, 40, 40, 255}, GRAY, clicked);
    ui_create_button(left_container, constraints_from_string("c 10o 0.7r 1.0a"), "O", (Color){40, 40, 40, 255}, GRAY, clicked);
    ui_create_checkbox(left_container, constraints_from_string("c c 0.7r 1.0a"), (Color){40, 40, 40, 255}, GRAY, NULL);

    UIContainer* top_container = ui_create_container(window_get_main_container(main_window), constraints_from_string("80p 10p -90p 60p"));
    ui_create_panel(top_container, constraints_from_string("0r 0r 1r 1r"), GRAY, DARK_GRAY, 2, 2);
    ui_create_label(top_container, constraints_from_string("c c 0p 0p"), "Coordinate Geometry", DARK_GRAY);
    ui_create_slider(top_container, constraints_from_string("0.68r 10p 0.3r 30p"), 0.5, DARK_GRAY, color_shift(DARK_GRAY, 40), NULL);
    ui_create_textbox(top_container, constraints_from_string("10p 10p 0.3r 30p"), "bing chilling", DARK_GRAY, GRAY, NULL);

    UIContainer* main_container = ui_create_container(window_get_main_container(main_window), constraints_from_string("80p 80p -90p -90p"));
    ui_create_panel(main_container, constraints_from_string("0r 0r 1r 1r"), GRAY, DARK_GRAY, 2, 2);
    ui_create_dropdown(main_container, constraints_from_string("c c 0.3r 30p"), "Item 1;Item 2;Item 3;Item 4", DARK_GRAY, GRAY, NULL);

    while (!main_window->close_requested)
    {
        app_update();
        renderer_clear(GRAY);
        app_render();
    }

    app_close();
    return 0;
}