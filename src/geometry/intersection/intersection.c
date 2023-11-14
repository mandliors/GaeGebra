#include "intersection.h"

static Intersection _line_line_intersection(Line* line1, Line* line2);
static Intersection _line_circle_intersection(Line* line, Circle* circle);
static Intersection _circle_circle_intersection(Circle* circle1, Circle* circle2);

Intersection intersection_new(Shape* shape1, Shape* shape2)
{
    Intersection intersection;
    intersection.points = vector_create(0);
    intersection.shape1 = shape1;
    intersection.shape2 = shape2;
    return intersection;
}
Intersection intersection_get(Shape* shape1, Shape* shape2)
{
    switch (shape1->type)
    {
    case ST_POINT:
        return (Intersection){ NULL, NULL, NULL };
    case ST_LINE:
        switch (shape2->type)
        {
        case ST_POINT:
            return (Intersection){ NULL, NULL, NULL };
        case ST_LINE:
            return _line_line_intersection((Line*)shape1, (Line*)shape2);
        case ST_CIRCLE:
            return _line_circle_intersection((Line*)shape1, (Circle*)shape2);
        default:
            return (Intersection){ NULL, NULL, NULL };
        }
    case ST_CIRCLE:
        switch (shape2->type)
        {
        case ST_POINT:
            return (Intersection){ NULL, NULL, NULL };
        case ST_LINE:
            return _line_circle_intersection((Line*)shape2, (Circle*)shape1);
        case ST_CIRCLE:
            return _circle_circle_intersection((Circle*)shape1, (Circle*)shape2);
        default:
            return (Intersection){ NULL, NULL, NULL };
        }
    default:
        return (Intersection){ NULL, NULL, NULL };
    }
    return (Intersection){ NULL, NULL, NULL };
}
void intersection_destroy(Intersection* intersection)
{
    free(intersection->points);
    free(intersection);
}

static Intersection _line_line_intersection(Line* line1, Line* line2)
{
    Vector2 p1 = line1->p1->coordinates;
    Vector2 p2 = line1->p2->coordinates;
    Vector2 p3 = line2->p1->coordinates;
    Vector2 p4 = line2->p2->coordinates;

    float denominator = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
    if (denominator == 0)
        return (Intersection){ NULL, NULL, NULL };

    float ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
    float ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1)
        return (Intersection){ NULL, NULL, NULL };

    Vector2* intersection_point = malloc(sizeof(Vector2));
    *intersection_point = vector2_add(p1, vector2_multiply(vector2_subtract(p2, p1), vector2_create(ua, ua)));
    Intersection intersection = intersection_new((Shape*)line1, (Shape*)line2);
    vector_push_back(intersection.points, intersection_point);
    return intersection;
}
static Intersection _line_circle_intersection(Line* line, Circle* circle)
{
    Vector2 p1 = line->p1->coordinates;
    Vector2 p2 = line->p2->coordinates;
    Vector2 center = circle->center->coordinates;
    float radius = vector2_distance(center, circle->perimeter_point->coordinates);

    Vector2 d = vector2_subtract(p2, p1);
    Vector2 f = vector2_subtract(p1, center);

    float a = vector2_dot(d, d);
    float b = 2 * vector2_dot(f, d);
    float c = vector2_dot(f, f) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (Intersection){ NULL, NULL, NULL };

    discriminant = sqrt(discriminant);
    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);

    if (t1 >= 0 && t1 <= 1)
    {
        Vector2* intersection_point = malloc(sizeof(Vector2));
        *intersection_point = vector2_add(p1, vector2_multiply(d, vector2_create(t1, t1)));
        Intersection intersection = intersection_new((Shape*)line, (Shape*)circle);
        vector_push_back(intersection.points, intersection_point);
        return intersection;
    }
    if (t2 >= 0 && t2 <= 1)
    {
        Vector2* intersection_point = malloc(sizeof(Vector2));
        *intersection_point = vector2_add(p1, vector2_multiply(d, vector2_create(t2, t2)));
        Intersection intersection = intersection_new((Shape*)line, (Shape*)circle);
        vector_push_back(intersection.points, intersection_point);
        return (Intersection){ NULL, NULL, NULL };
    }
    return (Intersection){ NULL, NULL, NULL };
}
static Intersection _circle_circle_intersection(Circle* circle1, Circle* circle2)
{
    Vector2 center1 = circle1->center->coordinates;
    Vector2 center2 = circle2->center->coordinates;
    float radius1 = vector2_distance(center1, circle1->perimeter_point->coordinates);
    float radius2 = vector2_distance(center2, circle2->perimeter_point->coordinates);

    float d = vector2_distance(center1, center2);
    if (d > radius1 + radius2)
        return (Intersection){ NULL, NULL, NULL };
    if (d < fabs(radius1 - radius2))
        return (Intersection){ NULL, NULL, NULL };
    if (d == 0 && radius1 == radius2)
        return (Intersection){ NULL, NULL, NULL };

    float a = (radius1 * radius1 - radius2 * radius2 + d * d) / (2 * d);
    float h = sqrt(radius1 * radius1 - a * a);
    Vector2 p2 = vector2_add(center1, vector2_multiply(vector2_subtract(center2, center1), vector2_create(a / d, a / d)));
    Vector2 p3 = vector2_add(p2, vector2_multiply(vector2_rotate90(vector2_subtract(center2, center1)), vector2_create(h / d, h / d)));

    Vector2* intersection_point = malloc(sizeof(Vector2));
    *intersection_point = p3;
    Intersection intersection = intersection_new((Shape*)circle1, (Shape*)circle2);
    vector_push_back(intersection.points, intersection_point);
    return intersection;
}