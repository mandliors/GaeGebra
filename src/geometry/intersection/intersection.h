#pragma once

#include "../shape/shape.h"
#include "../../utils/vector/vector.h"

typedef struct Intersection
{
    Vector* points;
    Shape* shape1;
    Shape* shape2;
} Intersection;

Intersection intersection_create(Shape* shape1, Shape* shape2);
Intersection intersection_get(Shape* shape1, Shape* shape2);
void intersection_destroy(Intersection* intersection);