#pragma once

#include "../shape/shape.h"
#include "../../utils/vector/vector.h"

/**
 * @brief Returns the intersection(s) of two shapes
 * 
 * @param shape1 The first shape
 * @param shape2 The second shape
 * @return Vector* The intersection vector, containing the intersection point(s) (should be freed!)
 */
Vector* intersection_get(Shape* shape1, Shape* shape2);