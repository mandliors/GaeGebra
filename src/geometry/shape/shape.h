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

typedef enum ShapeType
{
    ST_POINT,
    ST_LINE,
    ST_CIRCLE
} ShapeType;

typedef struct Shape
{
    bool selected;
    ShapeType type;

    ShapeDraw draw;
    ShapeTranslate translate;
    ShapeDestroy destroy;
    ShapeOverlapPoint overlap_point;
    ShapeIsDefinedBy is_defined_by;
} Shape;

typedef struct Point
{
    Shape base;
    Vector2 coordinates;
} Point;

typedef struct Line
{
    Shape base;
    Point *p1, *p2;
} Line;

typedef struct Circle
{
    Shape base;
    Point* center;
    Point* perimeter_point;
} Circle;

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