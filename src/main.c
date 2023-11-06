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

#define FPS 60

void on_filemenu_clicked(UISplitButton* self, Sint32 index);
void on_editmenu_clicked(UISplitButton* self, Sint32 index);

int main(void)
{
    app_init();
    app_set_target_fps(FPS);

    Window* window = window_create("GaeGebra", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_MaximizeWindow(window->window);

    Font* font = font_load("../assets/LiberationSerif.ttf", 20);
    renderer_set_default_font(font);

    UIContainer* toolbar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 30p 1r 60p"));
    ui_create_textbox(toolbar, constraints_from_string("10p c 200p 0.8r"), "Hello World!", WHITE, BLACK, NULL);
    
    UIContainer* menubar = ui_create_container(window_get_main_container(window), constraints_from_string("0p 0p 1r 30p"));
    ui_create_panel(menubar, constraints_from_string("0p 0p 1r 1r"), color_from_grayscale(200), WHITE, 0, 0);
    ui_create_splitbutton(menubar, constraints_from_string("0p 0p 1r 1r"), "File;Close", color_from_grayscale(180), BLACK, on_filemenu_clicked, true);
    ui_create_splitbutton(menubar, constraints_from_string("0o 0p 1r 1r"), "Edit;Close", color_from_grayscale(180), BLACK, on_editmenu_clicked, true);

    while (!window->close_requested)
    {
        app_update();
        app_set_target(window);
        renderer_clear(WHITE);
        app_render();
    }

    app_close();
    return 0;
}

void on_filemenu_clicked(UISplitButton* self, Sint32 index)
{
    printf("File menu clicked: %d%p\n", index, (void*)self);
    if (index == 0)
        app_request_close();
}
void on_editmenu_clicked(UISplitButton* self, Sint32 index)
{
    printf("File menu clicked: %d%p\n", index, (void*)self);
    if (index == 0)
        app_request_close();
}