#include "shape.h"

#include "../coordinate_system/coordinate_system.h"
#include "../../renderer/renderer.h"
#include "../../input/input.h"

#include <math.h>

#define EPSILON 0.0001

static void _point_draw(CoordinateSystem* cs, Shape* self);
static void _line_draw(CoordinateSystem* cs, Shape* self);
static void _circle_draw(CoordinateSystem* cs, Shape* self);
static void _parallel_draw(CoordinateSystem* cs, Shape* self);
static void _perpendicular_draw(CoordinateSystem* cs, Shape* self);
static void _angle_bisector_draw(CoordinateSystem* cs, Shape* self);
static void _tangent_draw(CoordinateSystem* cs, Shape* self);

static void _point_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _line_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _circle_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _parallel_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _perpendicular_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _angle_bisector_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _tangent_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);

static void _point_destroy(CoordinateSystem* cs, Shape* self);
static void _line_destroy(CoordinateSystem* cs, Shape* self);
static void _circle_destroy(CoordinateSystem* cs, Shape* self);
static void _parallel_destroy(CoordinateSystem* cs, Shape* self);
static void _perpendicular_destroy(CoordinateSystem* cs, Shape* self);
static void _angle_bisector_destroy(CoordinateSystem* cs, Shape* self);
static void _tangent_destroy(CoordinateSystem* cs, Shape* self);

static bool _point_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _line_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _circle_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _parallel_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _perpendicular_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _angle_bisector_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _tangent_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);

static bool _point_is_defined_by(Shape* self, Shape* shape);
static bool _line_is_defined_by(Shape* self, Shape* shape);
static bool _circle_is_defined_by(Shape* self, Shape* shape);
static bool _parallel_is_defined_by(Shape* self, Shape* shape);
static bool _perpendicular_is_defined_by(Shape* self, Shape* shape);
static bool _angle_bisector_is_defined_by(Shape* self, Shape* shape);
static bool _tangent_is_defined_by(Shape* self, Shape* shape);

static bool _equals(double a, double b);
static Vector2 _line_line_intersection(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3);
static Vector* _circle_circle_intersection(Vector2 center1, double radius1, Vector2 center2, double radius2);
static void _draw_line_on_screen(CoordinateSystem* cs, Vector2 p1, Vector2 p2, bool fixed, bool selected);

ShapeDraw shape_draw_funcs[ST_COUNT] = {_point_draw, _line_draw, _circle_draw, _parallel_draw, _perpendicular_draw, _angle_bisector_draw, _tangent_draw};
ShapeTranslate shape_translate_funcs[ST_COUNT] = {_point_translate, _line_translate, _circle_translate, _parallel_translate, _perpendicular_translate, _angle_bisector_translate, _tangent_translate};
ShapeDestroy shape_destroy_funcs[ST_COUNT] = {_point_destroy, _line_destroy, _circle_destroy, _parallel_destroy, _perpendicular_destroy, _angle_bisector_destroy, _tangent_destroy};
ShapeOverlapPoint shape_overlap_point_funcs[ST_COUNT] = {_point_overlap, _line_overlap, _circle_overlap, _parallel_overlap, _perpendicular_overlap, _angle_bisector_overlap, _tangent_overlap};
ShapeIsDefinedBy shape_is_defined_by_funcs[ST_COUNT] = {_point_is_defined_by, _line_is_defined_by, _circle_is_defined_by, _parallel_is_defined_by, _perpendicular_is_defined_by, _angle_bisector_is_defined_by, _tangent_is_defined_by};

Point* point_create(CoordinateSystem* cs, Vector2 coordinates)
{
    Point* point = malloc(sizeof(Point));
    point->base.type = ST_POINT;
    point->base.selected = false;
    point->base.dragged = false;
    point->coordinates = coordinates;
    vector_push_back(cs->shapes, point);
    return point;
}
Line* line_create(CoordinateSystem* cs, Point* p1, Point* p2)
{
    Line* line = malloc(sizeof(Line));
    line->base.type = ST_LINE;
    line->base.selected = false;
    line->base.dragged = false;
    line->p1 = p1;
    line->p2 = p2;
    vector_push_back(cs->shapes, line);
    return line;
}
Circle* circle_create(CoordinateSystem* cs, Point* center, Point* perimeter_point)
{
    Circle* circle = malloc(sizeof(Circle));
    circle->base.type = ST_CIRCLE;
    circle->base.selected = false;
    circle->base.dragged = false;
    circle->center = center;
    circle->perimeter_point = perimeter_point;
    vector_push_back(cs->shapes, circle);
    return circle;
}
Parallel* parallel_create(CoordinateSystem* cs, Line* line, Point* point)
{
    Parallel* parallel = malloc(sizeof(Parallel));
    parallel->base.type = ST_PARALLEL;
    parallel->base.selected = false;
    parallel->base.dragged = false;
    parallel->line = line;
    parallel->point = point;
    vector_push_back(cs->shapes, parallel);
    return parallel;
}
Perpendicular* perpendicular_create(CoordinateSystem* cs, Line* line, Point* point)
{
    Perpendicular* perpendicular = malloc(sizeof(Perpendicular));
    perpendicular->base.type = ST_PERPENDICULAR;
    perpendicular->base.selected = false;
    perpendicular->base.dragged = false;
    perpendicular->line = line;
    perpendicular->point = point;
    vector_push_back(cs->shapes, perpendicular);
    return perpendicular;
}
AngleBisector* angle_bisector_create(CoordinateSystem* cs, Line* line1, Line* line2)
{
    AngleBisector* angle_bisector = malloc(sizeof(AngleBisector));
    angle_bisector->base.type = ST_ANGLE_BISECTOR;
    angle_bisector->base.selected = false;
    angle_bisector->base.dragged = false;
    angle_bisector->line1 = line1;
    angle_bisector->line2 = line2;
    vector_push_back(cs->shapes, angle_bisector);
    return angle_bisector;
}
Tangent* tangent_create(CoordinateSystem* cs, Circle* circle, Point* point)
{
    Tangent* tangent = malloc(sizeof(Tangent));
    tangent->base.type = ST_TANGENT;
    tangent->base.selected = false;
    tangent->base.dragged = false;
    tangent->circle = circle;
    tangent->point = point;
    vector_push_back(cs->shapes, tangent);
    return tangent;
}

void shape_draw(CoordinateSystem* cs, Shape* self)
{
    shape_draw_funcs[self->type](cs, self);
}
void shape_update(CoordinateSystem* cs, Shape* self)
{
    if (self->dragged)
        shape_translate(cs, self, vector2_from_point(input_get_mouse_motion()));
}
void shape_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{
    shape_translate_funcs[self->type](cs, self, translation);
}
void shape_destroy(CoordinateSystem* cs, Shape* self)
{
    shape_destroy_funcs[self->type](cs, self);
}
bool shape_overlap_point(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    return shape_overlap_point_funcs[self->type](cs, self, point);
}
bool shape_is_defined_by(Shape* self, Shape* shape)
{
    return shape_is_defined_by_funcs[self->type](self, shape);
}

static void _point_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((Point*)self);
}
static void _line_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((Line*)self);
}
static void _circle_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((Circle*)self);
}
static void _parallel_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((Parallel*)self);
}
static void _perpendicular_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((Perpendicular*)self);
}
static void _angle_bisector_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((AngleBisector*)self);
}
static void _tangent_destroy(CoordinateSystem* cs __attribute__((unused)), Shape* self)
{
    free((Tangent*)self);
}

static void _point_draw(CoordinateSystem* cs, Shape* self)
{
    Point* point = (Point*)self;
    Vector2 position = coordinates_to_screen(cs, point->coordinates);
    if (self->selected)
        renderer_draw_filled_circle(position.x, position.y, 9, color_fade(BLACK, 0.3));
    renderer_draw_filled_circle(position.x, position.y, 5, GRAY);
    renderer_draw_circle(position.x, position.y, 5, BLACK);
}
static void _line_draw(CoordinateSystem* cs, Shape* self)
{
    Line* line = (Line*)self;
    _draw_line_on_screen(cs, coordinates_to_screen(cs, line->p1->coordinates), coordinates_to_screen(cs, line->p2->coordinates), false, self->selected);    
}
static void _circle_draw(CoordinateSystem* cs, Shape* self)
{
    Circle* circle = (Circle*)self;
    Vector2 position = coordinates_to_screen(cs, circle->center->coordinates);
    double radius = vector2_distance(position, coordinates_to_screen(cs, circle->perimeter_point->coordinates));
    if (self->selected)
        for (size_t r = radius - 2; r < radius + 4; r++)
            renderer_draw_circle(position.x, position.y, r, color_fade(BLACK, 0.3));
    for (size_t r = radius; r < radius + 2; r++)
        renderer_draw_circle(position.x, position.y, r, BLACK);
}
static void _parallel_draw(CoordinateSystem* cs, Shape* self)
{
    Parallel* parallel = (Parallel*)self;
    Vector2 p1 = coordinates_to_screen(cs, parallel->line->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, parallel->line->p2->coordinates);
    Vector2 pp1 = coordinates_to_screen(cs, parallel->point->coordinates);
    Vector2 pp2 = vector2_add(p2, vector2_subtract(pp1, p1));
    _draw_line_on_screen(cs, pp1, pp2, true, self->selected);
}
static void _perpendicular_draw(CoordinateSystem* cs, Shape* self)
{
    Perpendicular* perpendicular = (Perpendicular*)self;
    Vector2 p1 = coordinates_to_screen(cs, perpendicular->line->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, perpendicular->line->p2->coordinates);
    Vector2 pp1 = coordinates_to_screen(cs, perpendicular->point->coordinates);
    Vector2 normal = vector2_rotate90(vector2_subtract(p2, p1));
    Vector2 pp2 = vector2_add(pp1, normal);
    _draw_line_on_screen(cs, pp1, pp2, true, self->selected);
}
static void _angle_bisector_draw(CoordinateSystem* cs, Shape* self)
{
    AngleBisector* angle_bisector = (AngleBisector*)self;
    if (angle_bisector->line1 == NULL || angle_bisector->line2 == NULL)
        return;

    Vector2 p1 = coordinates_to_screen(cs, angle_bisector->line1->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, angle_bisector->line1->p2->coordinates);
    Vector2 p3 = coordinates_to_screen(cs, angle_bisector->line2->p1->coordinates);
    Vector2 p4 = coordinates_to_screen(cs, angle_bisector->line2->p2->coordinates);
    Vector2 m1 = _line_line_intersection(p1, p2, p3, p4);
    if (isnan(m1.x))
        return;
    double angle = vector2_angle(vector2_subtract(p2, p1)) - vector2_angle(vector2_subtract(p4, p3));
    Vector2 normal1 = vector2_rotate(vector2_subtract(p2, p1), -angle / 2);
    Vector2 normal2 = vector2_rotate90(normal1);
    Vector2 m2 = vector2_add(m1, normal1);
    Vector2 m3 = vector2_add(m1, normal2);
    _draw_line_on_screen(cs, m1, m2, true, self->selected);
    _draw_line_on_screen(cs, m1, m3, true, self->selected);
}
static void _tangent_draw(CoordinateSystem* cs, Shape* self)
{
    Tangent* tangent = (Tangent*)self;
    Vector2 center = coordinates_to_screen(cs, tangent->circle->center->coordinates);
    Vector2 from = coordinates_to_screen(cs, tangent->point->coordinates);
    Vector2 center2 = vector2_scale(vector2_add(center, from), 0.5);
    double radius2 = vector2_distance(center2, from);
    double radius = vector2_distance(center, coordinates_to_screen(cs, tangent->circle->perimeter_point->coordinates));
    Vector* intersections = _circle_circle_intersection(center, radius, center2, radius2);
    if (intersections == NULL)
        return;
    _draw_line_on_screen(cs, from, *(Vector2*)vector_get(intersections, 0), true, self->selected);
    _draw_line_on_screen(cs, from, *(Vector2*)vector_get(intersections, 1), true, self->selected);
    vector_destroy(intersections);
}

static void _point_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{
    Point* point = (Point*)self;
    point->coordinates = screen_to_coordinates(cs, vector2_add(coordinates_to_screen(cs, point->coordinates), translation));
}
static void _line_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{       
    Line* line = (Line*)self;
    if (!((Shape*)line->p1)->dragged) _point_translate(cs, (Shape*)line->p1, translation);
    if (!((Shape*)line->p2)->dragged) _point_translate(cs, (Shape*)line->p2, translation);
}
static void _circle_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{
    Circle* circle = (Circle*)self;
    if (!((Shape*)circle->center)->dragged) _point_translate(cs, (Shape*)circle->center, translation);
    if (!((Shape*)circle->perimeter_point)->dragged) _point_translate(cs, (Shape*)circle->perimeter_point, translation);
}
static void _parallel_translate(CoordinateSystem* cs __attribute__((unused)), Shape* self __attribute__((unused)), Vector2 translation __attribute__((unused)))
{
    return;
}
static void _perpendicular_translate(CoordinateSystem* cs __attribute__((unused)), Shape* self __attribute__((unused)), Vector2 translation __attribute__((unused)))
{
    return;
}
static void _angle_bisector_translate(CoordinateSystem* cs __attribute__((unused)), Shape* self __attribute__((unused)), Vector2 translation __attribute__((unused)))
{
    return;
}
static void _tangent_translate(CoordinateSystem* cs __attribute__((unused)), Shape* self __attribute__((unused)), Vector2 translation __attribute__((unused)))
{
    return;
}

static bool _point_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    return vector2_distance(coordinates_to_screen(cs, ((Point*)self)->coordinates), point) <= OVERLAP_DISTANCE;
}
static bool _line_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    Line* line = (Line*)self;
    Vector2 screen_normal = vector2_normalize(vector2_rotate90(vector2_subtract(coordinates_to_screen(cs, line->p2->coordinates), coordinates_to_screen(cs, line->p1->coordinates))));
    return fabs(vector2_dot(screen_normal, point) - 
                vector2_dot(screen_normal, coordinates_to_screen(cs, line->p1->coordinates))) 
                <= vector2_length(screen_normal) * OVERLAP_DISTANCE;
}
static bool _circle_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    Circle* circle = (Circle*)self;
    double radius = vector2_distance(coordinates_to_screen(cs, circle->center->coordinates), coordinates_to_screen(cs, circle->perimeter_point->coordinates));
    return fabs(vector2_distance(coordinates_to_screen(cs, circle->center->coordinates), point) - radius) <= OVERLAP_DISTANCE;
}
static bool _parallel_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    Parallel* parallel = (Parallel*)self;
    Vector2 p1 = coordinates_to_screen(cs, parallel->line->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, parallel->line->p2->coordinates);
    Vector2 pp1 = coordinates_to_screen(cs, parallel->point->coordinates);
    Vector2 pp2 = vector2_add(p2, vector2_subtract(pp1, p1));
    Vector2 screen_normal = vector2_normalize(vector2_rotate90(vector2_subtract(pp1, pp2)));
    return fabs(vector2_dot(screen_normal, point) - 
                vector2_dot(screen_normal, pp1))
                <= vector2_length(screen_normal) * OVERLAP_DISTANCE;
}
static bool _perpendicular_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    Perpendicular* perpendicular = (Perpendicular*)self;
    Vector2 p1 = coordinates_to_screen(cs, perpendicular->line->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, perpendicular->line->p2->coordinates);
    Vector2 pp1 = coordinates_to_screen(cs, perpendicular->point->coordinates);
    Vector2 normal = vector2_rotate90(vector2_subtract(p2, p1));
    Vector2 pp2 = vector2_add(pp1, normal);
    Vector2 screen_normal = vector2_normalize(vector2_rotate90(vector2_subtract(pp1, pp2)));
    return fabs(vector2_dot(screen_normal, point) - 
                vector2_dot(screen_normal, pp1))
                <= vector2_length(screen_normal) * OVERLAP_DISTANCE;
}
static bool _angle_bisector_overlap(CoordinateSystem* cs, Shape* self, Vector2 point __attribute__((unused)))
{
    AngleBisector* angle_bisector = (AngleBisector*)self;
    if (angle_bisector->line1 == NULL || angle_bisector->line2 == NULL)
        return false;
    Vector2 p1 = coordinates_to_screen(cs, angle_bisector->line1->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, angle_bisector->line1->p2->coordinates);
    Vector2 p3 = coordinates_to_screen(cs, angle_bisector->line2->p1->coordinates);
    Vector2 p4 = coordinates_to_screen(cs, angle_bisector->line2->p2->coordinates);
    Vector2 m1 = _line_line_intersection(p1, p2, p3, p4);
    if (isnan(m1.x))
        return false;
    double angle = vector2_angle(vector2_subtract(p2, p1)) - vector2_angle(vector2_subtract(p4, p3));
    Vector2 normal1 = vector2_rotate(vector2_subtract(p2, p1), -angle / 2);
    Vector2 normal2 = vector2_rotate90(normal1);
    Vector2 m2 = vector2_add(m1, normal1);
    Vector2 m3 = vector2_add(m1, normal2);
    Vector2 screen_normal1 = vector2_normalize(vector2_rotate90(vector2_subtract(m1, m2)));
    Vector2 screen_normal2 = vector2_normalize(vector2_rotate90(vector2_subtract(m1, m3)));

    return  fabs(vector2_dot(screen_normal1, point) - 
                vector2_dot(screen_normal1, m1))
                <= vector2_length(screen_normal1) * OVERLAP_DISTANCE ||
            fabs(vector2_dot(screen_normal2, point) - 
                vector2_dot(screen_normal2, m1))
                <= vector2_length(screen_normal2) * OVERLAP_DISTANCE;
}
static bool _tangent_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    Tangent* tangent = (Tangent*)self;
    Vector2 center = coordinates_to_screen(cs, tangent->circle->center->coordinates);
    Vector2 from = coordinates_to_screen(cs, tangent->point->coordinates);
    Vector2 center2 = vector2_scale(vector2_add(center, from), 0.5);
    double radius2 = vector2_distance(center2, from);
    double radius = vector2_distance(center, coordinates_to_screen(cs, tangent->circle->perimeter_point->coordinates));
    Vector* intersections = _circle_circle_intersection(center, radius, center2, radius2);
    if (intersections == NULL)
        return false;
    Vector2 screen_normal1 = vector2_normalize(vector2_rotate90(vector2_subtract(*(Vector2*)vector_get(intersections, 0), from)));
    Vector2 screen_normal2 = vector2_normalize(vector2_rotate90(vector2_subtract(*(Vector2*)vector_get(intersections, 1), from)));
    vector_destroy(intersections);
    return  fabs(vector2_dot(screen_normal1, point) - 
                vector2_dot(screen_normal1, from))
                <= vector2_length(screen_normal1) * OVERLAP_DISTANCE ||
            fabs(vector2_dot(screen_normal2, point) - 
                vector2_dot(screen_normal2, from))
                <= vector2_length(screen_normal2) * OVERLAP_DISTANCE;
}

static bool _point_is_defined_by(Shape* self __attribute__((unused)), Shape* shape __attribute__((unused)))
{
    return false;
}
static bool _line_is_defined_by(Shape* self, Shape* shape)
{
    Line* line = (Line*)self;
    return (Shape*)line->p1 == shape || (Shape*)line->p2 == shape;
}
static bool _circle_is_defined_by(Shape* self, Shape* shape)
{
    return (Shape*)((Circle*)self)->center == shape || (Shape*)((Circle*)self)->perimeter_point == shape;
}
static bool _parallel_is_defined_by(Shape* self, Shape* shape)
{
    Parallel* parallel = (Parallel*)self;
    return (Shape*)parallel->line == shape || (Shape*)parallel->point == shape;
}
static bool _perpendicular_is_defined_by(Shape* self, Shape* shape)
{
    Perpendicular* perpendicular = (Perpendicular*)self;
    return (Shape*)perpendicular->line == shape || (Shape*)perpendicular->point == shape;
}
static bool _angle_bisector_is_defined_by(Shape* self, Shape* shape)
{
    AngleBisector* angle_bisector = (AngleBisector*)self;
    return (Shape*)angle_bisector->line1 == shape || (Shape*)angle_bisector->line2 == shape;
}
static bool _tangent_is_defined_by(Shape* self, Shape* shape)
{
    Tangent* tangent = (Tangent*)self;
    return (Shape*)tangent->circle == shape || (Shape*)tangent->point == shape;
}

static bool _equals(double a, double b)
{
    return fabs(a - b) < EPSILON;
}
static Vector2 _line_line_intersection(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)
{
    Vector2 normal1 = vector2_rotate90(vector2_subtract(p1, p0));
    Vector2 normal2 = vector2_rotate90(vector2_subtract(p3, p2));
    double c1 = vector2_dot(normal1, p0);
    double c2 = vector2_dot(normal2, p2);
    double x, y;
    
    if (_equals(normal1.y, 0.0))
    {
        if (_equals(normal2.y, 0.0))
            return (Vector2){ NAN, NAN };
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
    return vector2_create(x, y);
}
static Vector* _circle_circle_intersection(Vector2 center1, double radius1, Vector2 center2, double radius2)
{
    double d = vector2_distance(center1, center2);
    if (d > radius1 + radius2 || d < fabs(radius1 - radius2))
        return NULL;
    
    double a = (radius1 * radius1 - radius2 * radius2 + d * d) / (2 * d);
    double h = sqrt(radius1 * radius1 - a * a);
    Vector2 p2 = vector2_add(center1, vector2_multiply(vector2_subtract(center2, center1), vector2_create(a / d, a / d)));
    Vector2 po = vector2_multiply(vector2_rotate90(vector2_subtract(center2, center1)), vector2_create(h / d, h / d));

    Vector2* intersection_point1 = malloc(sizeof(Vector2));
    *intersection_point1 = vector2_add(p2, po);
    Vector2* intersection_point2 = malloc(sizeof(Vector2));
    *intersection_point2 = vector2_subtract(p2, po);
    Vector* intersections = vector_create(2);
    vector_push_back(intersections, intersection_point1);
    vector_push_back(intersections, intersection_point2);
    return intersections;
}
static void _draw_line_on_screen(CoordinateSystem* cs, Vector2 p1, Vector2 p2, bool fixed, bool selected)
{
    Color color = fixed ? color_shift(BLACK, 150) : BLACK;
    Color selected_color = color_fade(color, 0.3);
    Vector2 m1 = _line_line_intersection((Vector2){ cs->position.x, cs->position.y }, (Vector2){ cs->position.x, cs->position.y + cs->size.y }, p1, p2);
    Vector2 m2 = _line_line_intersection((Vector2){ cs->position.x, cs->position.y }, (Vector2){ cs->position.x + cs->size.x, cs->position.y },p1, p2);
    if (!isnan(m1.x) && cs->position.y <= m1.y && m1.y <= cs->position.y + cs->size.y &&
        !isnan(m2.x) && cs->position.x <= m2.x && m2.x <= cs->position.x + cs->size.x)
    {
        if (selected)
            renderer_draw_line(m1.x, m1.y, m2.x, m2.y, 6, selected_color);
        renderer_draw_line(m1.x, m1.y, m2.x, m2.y, 2, color);
        return;
    }
    Vector2 m3 = _line_line_intersection((Vector2){ cs->position.x + cs->size.x, cs->position.y }, (Vector2){ cs->position.x + cs->size.x, cs->position.y + cs->size.y }, p1, p2);
    if (!isnan(m1.x) && cs->position.y <= m1.y && m1.y <= cs->position.y + cs->size.y && 
        !isnan(m3.x) && cs->position.y <= m3.y && m3.y <= cs->position.y + cs->size.y)
    {
        if (selected)
            renderer_draw_line(m1.x, m1.y, m3.x, m3.y, 6, selected_color);
        renderer_draw_line(m1.x, m1.y, m3.x, m3.y, 2, color);
        return;
    }
    else if (!isnan(m2.x) && cs->position.x <= m2.x && m2.x <= cs->position.x + cs->size.x &&
             !isnan(m3.x) && cs->position.y <= m3.y && m3.y <= cs->position.y + cs->size.y)
    {
        if (selected)
            renderer_draw_line(m2.x, m2.y, m3.x, m3.y, 6, selected_color);
        renderer_draw_line(m2.x, m2.y, m3.x, m3.y, 2, color);
        return;
    }
    Vector2 m4 = _line_line_intersection((Vector2){ cs->position.x, cs->position.y + cs->size.y }, (Vector2){ cs->position.x + cs->size.x, cs->position.y + cs->size.y }, p1, p2);
    if (!isnan(m1.x) && cs->position.y <= m1.y && m1.y <= cs->position.y + cs->size.y &&
        !isnan(m4.x) && cs->position.x <= m4.x && m4.x <= cs->position.x + cs->size.x)
    {
        if (selected)
            renderer_draw_line(m1.x, m1.y, m4.x, m4.y, 6, selected_color);
        renderer_draw_line(m1.x, m1.y, m4.x, m4.y, 2, color);
        return;
    }
    else if (!isnan(m2.x) && cs->position.x <= m2.x && m2.x <= cs->position.x + cs->size.x &&
             !isnan(m4.x) && cs->position.x <= m4.x && m4.x <= cs->position.x + cs->size.x)
    {
        if (selected)
            renderer_draw_line(m2.x, m2.y, m4.x, m4.y, 6, selected_color);
        renderer_draw_line(m2.x, m2.y, m4.x, m4.y, 2, color);
        return;
    }
    else if (!isnan(m3.x) && cs->position.y <= m3.y && m3.y <= cs->position.y + cs->size.y &&
             !isnan(m4.x) && cs->position.x <= m4.x && m4.x <= cs->position.x + cs->size.x)
    {
        if (selected)
            renderer_draw_line(m3.x, m3.y, m4.x, m4.y, 6, selected_color);
        renderer_draw_line(m3.x, m3.y, m4.x, m4.y, 2, color);
        return;
    }
}