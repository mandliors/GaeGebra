#include "shape.h"

#include "../../renderer/renderer.h"

Point* point_create(Vector2 position)
{
    Point* point = malloc(sizeof(Point));
    point->base.draw = _point_draw;
    point->base.overlap_point = _point_overlap;
    point->base.recalculate = NULL;
    point->base.free = _point_free;
    point->position = position;
    return point;
}
Line* line_create(Point* p1, Point* p2)
{
    Line* line = malloc(sizeof(Line));
    line->base.draw = _line_draw;
    line->base.overlap_point = _line_overlap;
    line->base.recalculate = NULL;
    line->base.free = _line_free;
    line->p1 = p1;
    line->p2 = p2;
    line->normal = vector2_normalize(vector2_rotate90(vector2_subtract(p2->position, p1->position)));
    return line;
}
Circle* circle_create(Point* center, double radius)
{
    Circle* circle = malloc(sizeof(Circle));
    circle->base.draw = _circle_draw;
    circle->base.overlap_point = _circle_overlap;
    circle->base.recalculate = NULL;
    circle->base.free = _circle_free;
    circle->center = center;
    circle->radius = radius;
    return circle;
}

bool _point_overlap(Shape* self, Vector2 point)
{
    return vector2_distance(((Point*)self)->position, point) <= OVERLAP_DISTANCE;
}
bool _line_overlap(Shape* self, Vector2 point)
{
    Line* line = (Line*)self;
    return fabs(vector2_dot(vector2_subtract(point, line->p1->position), line->normal)) <= OVERLAP_DISTANCE;

    //My method (also works)
    //return fabs(line->normal.x * point.x + line->normal.y * point.y -
    //           (line->normal.x * line->p1->position.x + line->normal.y * line->p1->position.y)) <=
    //           sqrt(line->normal.x * line->normal.x + line->normal.y * line->normal.y) * OVERLAP_DISTANCE;
}
bool _circle_overlap(Shape* self, Vector2 point)
{
    Circle* circle = (Circle*)self;
    return fabs(vector2_distance(circle->center->position, point) - circle->radius) <= OVERLAP_DISTANCE;
}

void _point_free(Shape* self)
{
    Point* point = (Point*)self;
    free(point);
}
void _line_free(Shape* self)
{
    Line* line = (Line*)self;
    free(line);
}
void _circle_free(Shape* self)
{
    Circle* circle = (Circle*)self;
    free(circle);
}

void _point_draw(Shape* self)
{
    Point* point = (Point*)self;
    renderer_draw_filled_circle(point->position.x, point->position.y, 5, BLACK);
    renderer_draw_circle(point->position.x, point->position.y, 5, GRAY);
}
void _line_draw(Shape* self)
{
    Line* line = (Line*)self;
    renderer_draw_line(line->p1->position.x, line->p1->position.y, line->p2->position.x, line->p2->position.y, 2, BLACK);
}
void _circle_draw(Shape* self)
{
    Circle* circle = (Circle*)self;
    renderer_draw_circle(circle->center->position.x, circle->center->position.y, circle->radius, GRAY);
    for (size_t r = circle->radius - 1; r < circle->radius + 2; r++)
        renderer_draw_circle(circle->center->position.x, circle->center->position.y, r, BLACK);
}