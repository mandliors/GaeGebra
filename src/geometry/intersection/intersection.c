#include "intersection.h"

#define EPSILON 0.0001

static Vector* _line_line_intersection(Line* line1, Line* line2);
static Vector* _line_circle_intersection(Line* line, Circle* circle);
static Vector* _circle_circle_intersection(Circle* circle1, Circle* circle2);

static bool _equals(double a, double b);

Vector* intersection_get(Shape* shape1, Shape* shape2)
{
    switch (shape1->type)
    {
    case ST_POINT:
        return NULL;
    case ST_LINE:
        switch (shape2->type)
        {
        case ST_POINT:
            return NULL;
        case ST_LINE:
            return _line_line_intersection((Line*)shape1, (Line*)shape2);
        case ST_CIRCLE:
            return _line_circle_intersection((Line*)shape1, (Circle*)shape2);
        default:
            return NULL;
        }
    case ST_CIRCLE:
        switch (shape2->type)
        {
        case ST_POINT:
            return NULL;
        case ST_LINE:
            return _line_circle_intersection((Line*)shape2, (Circle*)shape1);
        case ST_CIRCLE:
            return _circle_circle_intersection((Circle*)shape1, (Circle*)shape2);
        default:
            return NULL;
        }
    default:
        return NULL;
    }
    return NULL;
}

static Vector* _line_line_intersection(Line* line1, Line* line2)
{
    Vector2 normal1 = vector2_rotate90(vector2_subtract(line1->p2->coordinates, line1->p1->coordinates));
    Vector2 normal2 = vector2_rotate90(vector2_subtract(line2->p2->coordinates, line2->p1->coordinates));
    double c1 = vector2_dot(normal1, line1->p1->coordinates);
    double c2 = vector2_dot(normal2, line2->p1->coordinates);
    double x, y;

    if (_equals(normal1.y, 0.0))
    {
        if (_equals(normal2.y, 0.0))
            return NULL;
        x = c1 / normal1.x;
        y = (c2 - normal2.x * x) / normal2.y;
    }
    else if (_equals(normal2.y, 0.0))
    {
        x = c2 / normal2.x;
        y = (c1 - normal1.x * x) / normal1.y;
    }
    else
    {
        x = (normal2.y * c1 / normal1.y - c2) / (normal1.x * normal2.y / normal1.y - normal2.x);
        if (!_equals(normal1.y, 0.0))
            y = (c1 - normal1.x * x) / normal1.y;
        else
            y = (c2 - normal2.x * x) / normal2.y;
    }
    Vector2* intersection_point = malloc(sizeof(Vector2));
    *intersection_point = vector2_create(x, y);
    Vector* intersection = vector_create(1);
    vector_push_back(intersection, intersection_point);
    return intersection;

    //---------------------------- LINE SEGEMENT INTERSECTION -------------------------------------
    /*Vector2 p1 = line1->p1->coordinates;
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
    Intersection intersection = intersection_create((Shape*)line1, (Shape*)line2);
    vector_push_back(intersection.points, intersection_point);
    return intersection;*/
}
static Vector* _line_circle_intersection(Line* line, Circle* circle)
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
        return NULL;

    discriminant = sqrt(discriminant);
    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);

    Vector* intersections = vector_create(2);
    Vector2* intersection_point1 = malloc(sizeof(Vector2));
    *intersection_point1 = vector2_add(p1, vector2_multiply(d, vector2_create(t1, t1)));
    Vector2* intersection_point2 = malloc(sizeof(Vector2));
    *intersection_point2 = vector2_add(p1, vector2_multiply(d, vector2_create(t2, t2)));
    vector_push_back(intersections, intersection_point1);
    vector_push_back(intersections, intersection_point2);
    return intersections;
}
static Vector* _circle_circle_intersection(Circle* circle1, Circle* circle2)
{
    double r0 = vector2_distance(circle1->center->coordinates, circle1->perimeter_point->coordinates);
    double r1 = vector2_distance(circle2->center->coordinates, circle2->perimeter_point->coordinates);
    double d = vector2_distance(circle1->center->coordinates, circle2->center->coordinates);

    if (d > r0 + r1 || d < fabs(r0 - r1))
        return NULL;
    
    double a = (r0 * r0 - r1 * r1 + d * d) / (2 * d);
    double h = sqrt(r0 * r0 - a * a);
    Vector2 p2 = vector2_add(circle1->center->coordinates, vector2_multiply(vector2_subtract(circle2->center->coordinates, circle1->center->coordinates), vector2_create(a / d, a / d)));
    Vector2 po = vector2_multiply(vector2_rotate90(vector2_subtract(circle2->center->coordinates, circle1->center->coordinates)), vector2_create(h / d, h / d));

    Vector2* intersection_point1 = malloc(sizeof(Vector2));
    *intersection_point1 = vector2_add(p2, po);
    Vector2* intersection_point2 = malloc(sizeof(Vector2));
    *intersection_point2 = vector2_subtract(p2, po);
    Vector* intersections = vector_create(2);
    vector_push_back(intersections, intersection_point1);
    vector_push_back(intersections, intersection_point2);
    return intersections;
}

static bool _equals(double a, double b)
{
    return fabs(a - b) < EPSILON;
}