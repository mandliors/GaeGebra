#include "shape.h"

#include "../coordinate_system/coordinate_system.h"
#include "../../renderer/renderer.h"
#include "../../input/input.h"

#include <math.h>

#define EPSILON 0.0001

static void _point_draw(CoordinateSystem* cs, Shape* self);
static void _line_draw(CoordinateSystem* cs, Shape* self);
static void _circle_draw(CoordinateSystem* cs, Shape* self);

static void _point_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _line_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);
static void _circle_translate(CoordinateSystem* cs, Shape* self, Vector2 translation);

static void _point_destroy(CoordinateSystem* cs, Shape* self);
static void _line_destroy(CoordinateSystem* cs, Shape* self);
static void _circle_destroy(CoordinateSystem* cs, Shape* self);

static bool _point_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _line_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);
static bool _circle_overlap(CoordinateSystem* cs, Shape* self, Vector2 point);

static bool _point_is_defined_by(Shape* self, Shape* shape);
static bool _line_is_defined_by(Shape* self, Shape* shape);
static bool _circle_is_defined_by(Shape* self, Shape* shape);

static bool _equals(double a, double b);
static Vector2 _line_line_intersection(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3);

ShapeDraw shape_draw_funcs[ST_COUNT] = {_point_draw, _line_draw, _circle_draw};
ShapeTranslate shape_translate_funcs[ST_COUNT] = {_point_translate, _line_translate, _circle_translate};
ShapeDestroy shape_destroy_funcs[ST_COUNT] = {_point_destroy, _line_destroy, _circle_destroy};
ShapeOverlapPoint shape_overlap_point_funcs[ST_COUNT] = {_point_overlap, _line_overlap, _circle_overlap};
ShapeIsDefinedBy shape_is_defined_by_funcs[ST_COUNT] = {_point_is_defined_by, _line_is_defined_by, _circle_is_defined_by};

Point* point_create(CoordinateSystem* cs, Vector2 coordinates)
{
    Point* point = malloc(sizeof(Point));
    point->base.type = ST_POINT;
    point->base.selected = false;
    point->coordinates = coordinates;
    vector_push_back(cs->shapes, point);
    return point;
}
Line* line_create(CoordinateSystem* cs, Point* p1, Point* p2)
{
    Line* line = malloc(sizeof(Line));
    line->base.type = ST_LINE;
    line->base.selected = false;
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
    circle->center = center;
    circle->perimeter_point = perimeter_point;
    vector_push_back(cs->shapes, circle);
    return circle;
}

void shape_draw(CoordinateSystem* cs, Shape* self)
{
    shape_draw_funcs[self->type](cs, self);
}
void shape_update(CoordinateSystem* cs, Shape* self)
{
    if (cs->dragged_shape == self)
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
    //also check if the intersection is between the endpoints
    Line* line = (Line*)self;

    Vector2 m1 = _line_line_intersection((Vector2){ cs->position.x, cs->position.y }, (Vector2){ cs->position.x, cs->position.y + cs->size.y },
                                        coordinates_to_screen(cs, line->p1->coordinates), coordinates_to_screen(cs, line->p2->coordinates));
    Vector2 m2 = _line_line_intersection((Vector2){ cs->position.x, cs->position.y }, (Vector2){ cs->position.x + cs->size.x, cs->position.y },
                                    coordinates_to_screen(cs, line->p1->coordinates), coordinates_to_screen(cs, line->p2->coordinates));
    if (!isnan(m1.x) && cs->position.y <= m1.y && m1.y <= cs->position.y + cs->size.y &&
        !isnan(m2.x) && cs->position.x <= m2.x && m2.x <= cs->position.x + cs->size.x)
    {
        if (self->selected)
            renderer_draw_line(m1.x, m1.y, m2.x, m2.y, 6, color_fade(BLACK, 0.3));
        renderer_draw_line(m1.x, m1.y, m2.x, m2.y, 2, BLACK);
        return;
    }
    Vector2 m3 = _line_line_intersection((Vector2){ cs->position.x + cs->size.x, cs->position.y }, (Vector2){ cs->position.x + cs->size.x, cs->position.y + cs->size.y },
                                        coordinates_to_screen(cs, line->p1->coordinates), coordinates_to_screen(cs, line->p2->coordinates));
    if (!isnan(m1.x) && cs->position.y <= m1.y && m1.y <= cs->position.y + cs->size.y && 
        !isnan(m3.x) && cs->position.y <= m3.y && m3.y <= cs->position.y + cs->size.y)
    {
        if (self->selected)
            renderer_draw_line(m1.x, m1.y, m3.x, m3.y, 6, color_fade(BLACK, 0.3));
        renderer_draw_line(m1.x, m1.y, m3.x, m3.y, 2, BLACK);
        return;
    }
    else if (!isnan(m2.x) && cs->position.x <= m2.x && m2.x <= cs->position.x + cs->size.x &&
             !isnan(m3.x) && cs->position.y <= m3.y && m3.y <= cs->position.y + cs->size.y)
    {
        if (self->selected)
            renderer_draw_line(m2.x, m2.y, m3.x, m3.y, 6, color_fade(BLACK, 0.3));
        renderer_draw_line(m2.x, m2.y, m3.x, m3.y, 2, BLACK);
        return;
    }
    Vector2 m4 = _line_line_intersection((Vector2){ cs->position.x, cs->position.y + cs->size.y }, (Vector2){ cs->position.x + cs->size.x, cs->position.y + cs->size.y },
                                            coordinates_to_screen(cs, line->p1->coordinates), coordinates_to_screen(cs, line->p2->coordinates));
    if (!isnan(m1.x) && cs->position.y <= m1.y && m1.y <= cs->position.y + cs->size.y &&
        !isnan(m4.x) && cs->position.x <= m4.x && m4.x <= cs->position.x + cs->size.x)
    {
        if (self->selected)
            renderer_draw_line(m1.x, m1.y, m4.x, m4.y, 6, color_fade(BLACK, 0.3));
        renderer_draw_line(m1.x, m1.y, m4.x, m4.y, 2, BLACK);
        return;
    }
    else if (!isnan(m2.x) && cs->position.x <= m2.x && m2.x <= cs->position.x + cs->size.x &&
             !isnan(m4.x) && cs->position.x <= m4.x && m4.x <= cs->position.x + cs->size.x)
    {
        if (self->selected)
            renderer_draw_line(m2.x, m2.y, m4.x, m4.y, 6, color_fade(BLACK, 0.3));
        renderer_draw_line(m2.x, m2.y, m4.x, m4.y, 2, BLACK);
        return;
    }
    else if (!isnan(m3.x) && cs->position.y <= m3.y && m3.y <= cs->position.y + cs->size.y &&
             !isnan(m4.x) && cs->position.x <= m4.x && m4.x <= cs->position.x + cs->size.x)
    {
        if (self->selected)
            renderer_draw_line(m3.x, m3.y, m4.x, m4.y, 6, color_fade(BLACK, 0.3));
        renderer_draw_line(m3.x, m3.y, m4.x, m4.y, 2, BLACK);
        return;
    }
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

static void _point_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{
    Point* point = (Point*)self;
    point->coordinates = screen_to_coordinates(cs, vector2_add(coordinates_to_screen(cs, point->coordinates), translation));
}
static void _line_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{       
    Line* line = (Line*)self;
    _point_translate(cs, (Shape*)line->p1, translation);
    _point_translate(cs, (Shape*)line->p2, translation);
}
static void _circle_translate(CoordinateSystem* cs, Shape* self, Vector2 translation)
{
    Circle* circle = (Circle*)self;
    _point_translate(cs, (Shape*)circle->center, translation);
    _point_translate(cs, (Shape*)circle->perimeter_point, translation);
}

static bool _point_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    return vector2_distance(coordinates_to_screen(cs, ((Point*)self)->coordinates), point) <= OVERLAP_DISTANCE;
}
static bool _line_overlap(CoordinateSystem* cs, Shape* self, Vector2 point)
{
    Line* line = (Line*)self;
    Vector2 screen_normal = vector2_normalize(vector2_rotate90(vector2_subtract(coordinates_to_screen(cs, line->p2->coordinates), coordinates_to_screen(cs, line->p1->coordinates))));
    //My method
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

static bool _equals(double a, double b)
{
    return fabs(a - b) < EPSILON;
}
static Vector2 _line_line_intersection(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)
{
    //My method
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