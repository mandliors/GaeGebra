#pragma once

#include <stdbool.h>

#include "../vector2/vector2.h"

#define OVERLAP_DISTANCE 5

typedef struct Shape Shape;
typedef struct Shape
{
    void (*draw)(Shape* self);
    bool (*overlap_point)(Shape* self, Vector2 point);
    void (*recalculate)(Shape* self);
    void (*free)(Shape* self);
} Shape;

typedef struct Point
{
    Shape base;
    Vector2 position;
} Point;

typedef struct Line
{
    Shape base;
    Point *p1, *p2;
    Vector2 normal;
} Line;

typedef struct Circle
{
    Shape base;
    Point* center;
    double radius;
} Circle;

Point* point_create(Vector2 position);
Line* line_create(Point* p1, Point* p2);
Circle* circle_create(Point* center, double radius);

bool _point_overlap(Shape* self, Vector2 point);
bool _line_overlap(Shape* self, Vector2 point);
bool _circle_overlap(Shape* self, Vector2 point);

void _point_free(Shape* self);
void _line_free(Shape* self);
void _circle_free(Shape* self);

void _point_draw(Shape* self);
void _line_draw(Shape* self);
void _circle_draw(Shape* self);
