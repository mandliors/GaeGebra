#pragma once

#include <stdbool.h>

#include "../vector2/vector2.h"

#define OVERLAP_DISTANCE 5

typedef struct CoordinateSystem CoordinateSystem;
typedef struct Shape Shape;

typedef void (*ShapeDraw)(struct CoordinateSystem* cs, struct Shape* self);
typedef void (*ShapeTranslate)(struct CoordinateSystem* cs, struct Shape* self, Vector2 translation);
typedef void (*ShapeDestroy)(struct CoordinateSystem* cs, struct Shape* self);
typedef bool (*ShapeOverlapPoint)(struct CoordinateSystem* cs, struct Shape* self, Vector2 point);
typedef bool (*ShapeIsDefinedBy)(struct Shape* self, struct Shape* shape);

/**
 * @brief The types of shapes that can be created
 */
typedef enum ShapeType
{
    ST_POINT,
    ST_LINE,
    ST_CIRCLE,
    
    ST_PARALLEL,
    ST_PERPENDICULAR,
    ST_ANGLE_BISECTOR,
    ST_TANGENT,

    ST_COUNT
} ShapeType;

/**
 * @brief The base shape struct (needed for polymorphism)
 */
typedef struct Shape
{
    ShapeType type;
    bool selected;
    bool dragged;
} Shape;

/**
 * @brief The point struct
 */
typedef struct Point
{
    Shape base;
    Vector2 coordinates;
} Point;

/**
 * @brief The line struct
 */
typedef struct Line
{
    Shape base;
    Point *p1, *p2;
} Line;

/**
 * @brief The circle struct
 */
typedef struct Circle
{
    Shape base;
    Point* center;
    Point* perimeter_point;
} Circle;

/**
 * @brief The parallel line struct
 */
typedef struct Parallel
{
    Shape base;
    Line* line;
    Point* point;
} Parallel;

/**
 * @brief The perpendicular line struct
 */
typedef struct Perpendicular
{
    Shape base;
    Line* line;
    Point* point;
} Perpendicular;

/**
 * @brief The angle bisector lines struct
 */
typedef struct AngleBisector
{
    Shape base;
    Line* line1;
    Line* line2;
} AngleBisector;

/**
 * @brief The tangent lines struct (circle tangents)
 */
typedef struct Tangent
{
    Shape base;
    Circle* circle;
    Point* point;
} Tangent;

/**
 * @brief Creates a point in the coordinate system
 * 
 * @param cs The coordinate system to create the point in
 * @param coordinates The coordinates of the point
 * @return Point* The created point
 */
Point* point_create(CoordinateSystem* cs, Vector2 coordinates);
/**
 * @brief Creates a line in the coordinate system
 * 
 * @param cs The coordinate system to create the line in
 * @param p1 A point of the line
 * @param p2 Another point of the line (has to be different from p1)
 * @return Line* The created line
 */
Line* line_create(CoordinateSystem* cs, Point* p1, Point* p2);
/**
 * @brief Creates a circle in the coordinate system
 * 
 * @param cs The coordinate system to create the circle in
 * @param center The center of the circle
 * @param perimeter_point A point on the perimeter of the circle (has to be different from center)
 * @return Circle* The created circle
 */
Circle* circle_create(CoordinateSystem* cs, Point* center, Point* perimeter_point);
/**
 * @brief Creates a parallel line in the coordinate system
 * 
 * @param cs The coordinate system to create the parallel line in
 * @param line The line to create the parallel line to
 * @param point The point the parallel line goes through
 * @return Parallel* The created parallel line
 */
Parallel* parallel_create(CoordinateSystem* cs, Line* line, Point* point);
/**
 * @brief Creates a perpendicular line in the coordinate system
 * 
 * @param cs The coordinate system to create the perpendicular line in
 * @param line The line to create the perpendicular line to
 * @param point The point the perpendicular line goes through
 * @return Perpendicular* The created perpendicular line
 */
Perpendicular* perpendicular_create(CoordinateSystem* cs, Line* line, Point* point);
/**
 * @brief Creates an angle bisector in the coordinate system
 * 
 * @param cs The coordinate system to create the angle bisector in
 * @param line1 The first line to create the angle bisector to
 * @param line2 The second line to create the angle bisector to
 * @return AngleBisector* The created angle bisector
 */
AngleBisector* angle_bisector_create(CoordinateSystem* cs, Line* line1, Line* line2);
/**
 * @brief Creates a tangent to a circle in the coordinate system
 * 
 * @param cs The coordinate system to create the tangent in
 * @param circle The circle to create the tangent to
 * @param point The point the tangent goes through
 * @return Tangent* The created tangent
 */
Tangent* tangent_create(CoordinateSystem* cs, Circle* circle, Point* point);

/**
 * @brief Draws a shape (can be called on any shape)
 * 
 * @param cs The coordinate system to draw the point in
 * @param self The shape to draw
 */
void shape_draw(CoordinateSystem* cs, Shape* self);
/**
 * @brief Updates a shape (can be called on any shape)
 * 
 * @param cs The coordinate system to update the shape in
 * @param self The shape to update
 */
void shape_update(CoordinateSystem* cs, Shape* self);
/**
 * @brief Translates a shape if it is allowed (can be called on any shape)
 * 
 * @param cs The coordinate system to translate the shape in
 * @param self The shape to translate
 * @param translation The translation vector
 */
void shape_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
/**
 * @brief Destroys a shape, but does not remove it from the coordinate systems shapes! (can be called on any shape)
 * 
 * @param cs The coordinate system to destroy the shape in
 * @param self The shape to destroy
 */
void shape_destroy(CoordinateSystem* cs, Shape* self);
/**
 * @brief Checks if a point overlaps the shape (can be called on any shape)
 * 
 * @param cs The coordinate system to check the overlap in
 * @param self The shape to check the overlap with
 * @param point The point to check the overlap with
 * @return true If the point overlaps with the shape
 * @return false If the point does not overlap with the shape
 */
bool shape_overlap_point(CoordinateSystem* cs, Shape* self, Vector2 point);
/**
 * @brief Checks if a shape is defined by another shape (can be called on any shape)
 * 
 * @param self The shape to check if it is defined by the other shape
 * @param shape The shape to check if it defines the other shape
 * @return true If the shape is defined by the other shape
 * @return false If the shape is not defined by the other shape
 */
bool shape_is_defined_by(Shape* self, Shape* shape);