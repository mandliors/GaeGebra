#pragma once

#include <stdbool.h>

#include "../vector2/vector2.h"

#define OVERLAP_DISTANCE 5

typedef struct CoordinateSystem CoordinateSystem;
typedef struct IShape IShape;
typedef struct IShape
{
    void (*draw)(CoordinateSystem* cs, IShape* self);
    void (*translate)(CoordinateSystem* cs, IShape* self, Vector2 translation);
    void (*destroy)(CoordinateSystem* cs, IShape* self);
    bool (*overlap_point)(CoordinateSystem* cs, IShape* self, Vector2 point);
    bool (*is_defined_by)(IShape* self, IShape* shape);
} IShape;

typedef struct Point
{
    IShape base;
    Vector2 coordinates;
} Point;

typedef struct Line
{
    IShape base;
    Point *p1, *p2;
} Line;

typedef struct Circle
{
    IShape base;
    Point* center;
    double radius;
} Circle;

Point* point_create(CoordinateSystem* cs, Vector2 coordinates);
Line* line_create(CoordinateSystem* cs, Point* p1, Point* p2);
Circle* circle_create(CoordinateSystem* cs, Point* center, double radius);