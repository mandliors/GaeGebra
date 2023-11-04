#cmake_minimum_required(VERSION 3.11)
#
#project(coordinate_geometry)
#
#include_directories(${CMAKE_SOURCE_DIR})
#file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
#
#add_executable(coordinate_geometry ${SOURCE_FILES})
#
#target_link_libraries(coordinate_geometry SDL2 SDL2_gfx SDL2_image SDL2_ttf m)
#target_compile_options(coordinate_geometry PRIVATE -Wall -Werror)

cmake_minimum_required(VERSION 3.11)
project(coordinate_geometry)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)

add_executable(coordinate_geometry)
target_sources(coordinate_geometry PRIVATE
    src/main.c
    src/app/app.c
    src/colors/colors.c
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
target_include_directories(coordinate_geometry PRIVATE src)
target_compile_options(coordinate_geometry PRIVATE -Wall -Wextra -Wpedantic) #-Wconversion
target_link_libraries(coordinate_geometry PRIVATE
    SDL2::Main
    SDL2::Image
    SDL2::TTF
    SDL2::GFX
    m
)
target_compile_options(coordinate_geometry PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(coordinate_geometry PRIVATE -fsanitize=address)
