#pragma once

#include "../shape/shape.h"
#include "../vector2/vector2.h"
#include "../../texture/texture.h"
#include "../../utils/vector/vector.h"

#define INITIAL_ZOOM 20

typedef struct CoordinateSystem
{
    Vector2 position;
    Vector2 size;
    Vector2 origin;
    double zoom;

    Vector* shapes;
    Vector* intersection_points;
    Shape* dragged_shape;
} CoordinateSystem;

/**
 * @brief Creates a coordinate system
 * 
 * @param position The position of the coordinate system in the screen
 * @param size The size of the coordinate system (in pixels)
 * @param origin The origin of the coordinate system (relative to the coordinate system (normalized))
 * @return CoordinateSystem* The created coordinate system
 */
CoordinateSystem* coordinate_system_create(Vector2 position, Vector2 size, Vector2 origin);
/**
 * @brief Destroys a coordinate system
 * 
 * @param cs The coordinate system to destroy
 */
void coordinate_system_destroy(CoordinateSystem* cs);

/**
 * @brief Translates a point from the screen to the coordinate system
 * 
 * @param cs The coordinate system to translate the point to
 * @param point The point to translate
 * @return Vector2 The translated point
 */
Vector2 screen_to_coordinates(CoordinateSystem* cs, Vector2 point);
/**
 * @brief Translates a point from the coordinate system to the screen
 * 
 * @param cs The coordinate system to translate the point from
 * @param point The point to translate
 * @return Vector2 The translated point
 */
Vector2 coordinates_to_screen(CoordinateSystem* cs, Vector2 point);

/**
 * @brief Returns whether the coordinate system is hovered by the point
 * 
 * @param cs The coordinate system to check
 * @param point The point to check
 */
bool coordinate_system_is_hovered(CoordinateSystem* cs, Vector2 point);/**
 * @brief Selects a shape
 * 
 * @param cs The coordinate system to select the shape in
 * @param shape The shape to select
 */
void coordinate_system_select_shape(CoordinateSystem* cs, Shape* shape);
/**
 * @brief Sets the dragged shape
 * 
 * @param cs The coordinate system to set the dragged shape in
 * @param shape The shape to set as dragged
 */
void coordinate_system_set_dragged_shape(CoordinateSystem* cs, Shape* shape);
/**
 * @brief Returns the shape hovered by the point
 * 
 * @param cs The coordinate system to check
 * @param point The point to check
 * @return Shape* The hovered shape (NULL if none)
 */
Shape* coordinate_system_get_hovered_shape(CoordinateSystem* cs, Vector2 point);
/**
 * @brief Returns the selected shapes
 * 
 * @param cs The coordinate system to retrieve to selected shapes from
 * @return Vector* A vector of the selected shapes
 */
Vector* coordinate_system_get_selected_shapes(CoordinateSystem* cs);
/**
 * @brief Returns the dragged shape
 * 
 * @param cs The coordinate system to retrieve to dragged shape from
 * @return Shape* The dragged shape
 */
Shape* coordinate_system_get_dragged_shape(CoordinateSystem* cs);
/**
 * @brief Deselects all the selected shapes
 * 
 * @param cs The coordinate system to deselect the shapes in
 */
void coordinate_system_deselect_shapes(CoordinateSystem* cs);
/**
 * @brief Deletes all the selected shapes
 * 
 * @param cs The coordinate system to delete the shapes in
*/
void coordinate_system_delete_selected_shapes(CoordinateSystem* cs);
/**
 * @brief Translates the coordinate system
 * 
 * @param cs The coordinate system to translate
 * @param translation The translation vector (in pixels)
 */
void coordinate_system_translate(CoordinateSystem* cs, Vector2 translation);
/**
 * @brief Zooms into the coordinate system
 * 
 * @param cs The coordinate system to zoom into
 * @param zoom The zoom factor
 */
void coordinate_system_zoom(CoordinateSystem* cs, double zoom);
/**
 * @brief Updates the coordinate system and calculates the intersections
 * 
 * @param cs The coordinate system to update
 */
void coordinate_system_update(CoordinateSystem* cs);
/**
 * @brief Draws the coordinate system
 * 
 * @param cs The coordinate system to draw
 */
void coordinate_system_draw(CoordinateSystem* cs);
/**
 * @brief Updates the dimensions of the coordinate system
 * 
 * @param cs The coordinate system to update
 * @param position The new position
 * @param size The new size
 */
void coordinate_system_update_dimensions(CoordinateSystem* cs, Vector2 position, Vector2 size);
/**
 * @brief Destroys a shape and removes it from the coordinate system (as well as the shapes it defined)
 * 
 * @param cs The coordinate system to remove the shape from
 * @param shape The shape to remove
 */
void coordinate_system_destroy_shape(CoordinateSystem* cs, Shape* shape);