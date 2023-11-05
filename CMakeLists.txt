cmake_minimum_required(VERSION 3.11)
project(coordinate_geometry)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)

add_executable(GaeGebra)
target_sources(GaeGebra PRIVATE
    src/main.c
    src/app/app.c
    src/color/color.c
    src/font/font.c
    src/input/input.c
    src/renderer/renderer.c
    src/texture/texture.c
    src/ui/ui_constraint/ui_constraint.c
    src/ui/ui_element/ui_element.c
    src/ui/ui.c
    src/utils/math/math.c
    src/utils/vector/vector.c
    src/window/window.c
)
target_include_directories(GaeGebra PRIVATE src)
target_compile_options(GaeGebra PRIVATE -Wall -Wextra -Wpedantic) #-Wconversion
target_link_libraries(GaeGebra PRIVATE
    SDL2::Main
    SDL2::Image
    SDL2::TTF
    SDL2::GFX
    m
)
target_compile_options(GaeGebra PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(GaeGebra PRIVATE -fsanitize=address)

############################### OLD CMAKEFILE #####################################
#cmake_minimum_required(VERSION 3.11)
#
#project(GaeGebra)
#
#include_directories(${CMAKE_SOURCE_DIR})
#file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
#
#add_executable(GaeGebra ${SOURCE_FILES})
#
#target_link_libraries(GaeGebra SDL2 SDL2_gfx SDL2_image SDL2_ttf m)
#target_compile_options(GaeGebra PRIVATE -Wall -Werror)
###################################################################################
