#include "shape.h"

#include "../coordinate_system/coordinate_system.h"
#include "../../renderer/renderer.h"

static void _point_draw(CoordinateSystem* cs, IShape* self);
static void _line_draw(CoordinateSystem* cs, IShape* self);
static void _circle_draw(CoordinateSystem* cs, IShape* self);

static void _point_translate(CoordinateSystem* cs, IShape* self, Vector2 translation);
static void _line_translate(CoordinateSystem* cs, IShape* self, Vector2 translation);
static void _circle_translate(CoordinateSystem* cs, IShape* self, Vector2 translation);

static void _point_destroy(CoordinateSystem* cs, IShape* self);
static void _line_destroy(CoordinateSystem* cs, IShape* self);
static void _circle_destroy(CoordinateSystem* cs, IShape* self);

static bool _point_overlap(CoordinateSystem* cs, IShape* self, Vector2 point);
static bool _line_overlap(CoordinateSystem* cs, IShape* self, Vector2 point);
static bool _circle_overlap(CoordinateSystem* cs, IShape* self, Vector2 point);

static bool _point_is_defined_by(IShape* self, IShape* shape);
static bool _line_is_defined_by(IShape* self, IShape* shape);
static bool _circle_is_defined_by(IShape* self, IShape* shape);

static void _coordinate_system_remove_shape(CoordinateSystem* cs, IShape* shape);

Point* point_create(CoordinateSystem* cs, Vector2 coordinates)
{
    Point* point = malloc(sizeof(Point));
    point->base.draw = _point_draw;
    point->base.translate = _point_translate;
    point->base.destroy = _point_destroy;
    point->base.overlap_point = _point_overlap;
    point->base.is_defined_by = _point_is_defined_by;
    point->coordinates = coordinates;
    vector_push_back(cs->shapes, point);
    return point;
}
Line* line_create(CoordinateSystem* cs, Point* p1, Point* p2)
{
    Line* line = malloc(sizeof(Line));
    line->base.draw = _line_draw;
    line->base.translate = _line_translate;
    line->base.destroy = _line_destroy;
    line->base.overlap_point = _line_overlap;
    line->base.is_defined_by = _line_is_defined_by;
    line->p1 = p1;
    line->p2 = p2;
    line->normal = vector2_normalize(vector2_rotate90(vector2_subtract(p2->coordinates, p1->coordinates)));
    vector_push_back(cs->shapes, line);
    return line;
}
Circle* circle_create(CoordinateSystem* cs, Point* center, double radius)
{
    Circle* circle = malloc(sizeof(Circle));
    circle->base.draw = _circle_draw;
    circle->base.translate = _circle_translate;
    circle->base.destroy = _circle_destroy;
    circle->base.overlap_point = _circle_overlap;
    circle->base.is_defined_by = _circle_is_defined_by;
    circle->center = center;
    circle->radius = radius;
    vector_push_back(cs->shapes, circle);
    return circle;
}

static void _point_destroy(CoordinateSystem* cs, IShape* self)
{
    _coordinate_system_remove_shape(cs, self);
    free((Point*)self);
    self = NULL;
}
static void _line_destroy(CoordinateSystem* cs, IShape* self)
{
    _coordinate_system_remove_shape(cs, self);
    free((Line*)self);
    self = NULL;
}
static void _circle_destroy(CoordinateSystem* cs, IShape* self)
{
    _coordinate_system_remove_shape(cs, self);
    free((Circle*)self);
    self = NULL;
}

static void _point_draw(CoordinateSystem* cs, IShape* self)
{
    Point* point = (Point*)self;
    Vector2 position = coordinates_to_screen(cs, point->coordinates);
    renderer_draw_filled_circle(position.x, position.y, 5, GRAY);
    renderer_draw_circle(position.x, position.y, 5, BLACK);
}
static void _line_draw(CoordinateSystem* cs, IShape* self)
{
    Line* line = (Line*)self;
    Vector2 p1 = coordinates_to_screen(cs, line->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, line->p2->coordinates);
    renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 2, BLACK);
}
static void _circle_draw(CoordinateSystem* cs, IShape* self)
{
    Circle* circle = (Circle*)self;
    Vector2 position = coordinates_to_screen(cs, circle->center->coordinates);
    renderer_draw_circle(position.x, position.y, circle->radius * cs->zoom, GRAY);
    for (size_t r = circle->radius * cs->zoom - 1; r < circle->radius * cs->zoom + 2; r++)
        renderer_draw_circle(position.x, position.y, r, BLACK);
}

static void _point_translate(CoordinateSystem* cs, IShape* self, Vector2 translation)
{
    Point* point = (Point*)self;
    point->coordinates = screen_to_coordinates(cs, vector2_add(coordinates_to_screen(cs, point->coordinates), translation));
}
static void _line_translate(CoordinateSystem* cs, IShape* self, Vector2 translation)
{
    Line* line = (Line*)self;
    _point_translate(cs, (IShape*)line->p1, translation);
    _point_translate(cs, (IShape*)line->p2, translation);
}
static void _circle_translate(CoordinateSystem* cs, IShape* self, Vector2 translation)
{
    Circle* circle = (Circle*)self;
    _point_translate(cs, (IShape*)circle->center, translation);
}

static bool _point_overlap(CoordinateSystem* cs, IShape* self, Vector2 point)
{
    return vector2_distance(coordinates_to_screen(cs, ((Point*)self)->coordinates), point) <= OVERLAP_DISTANCE;
}
static bool _line_overlap(CoordinateSystem* cs, IShape* self, Vector2 point)
{
    Line* line = (Line*)self;
    return fabs(vector2_dot(vector2_subtract(point, coordinates_to_screen(cs, line->p1->coordinates)), vector2_rotate90(line->normal))) <= OVERLAP_DISTANCE;

    //My method (also works)
    //return fabs(vector2_dot(line->normal, point) - 
    //            vector2_dot(line->normal, coordinates_to_screen(cs, line->p1->coordinates))) 
    //            <= vector2_length(line->normal) * OVERLAP_DISTANCE;
}
static bool _circle_overlap(CoordinateSystem* cs, IShape* self, Vector2 point)
{
    Circle* circle = (Circle*)self;
    return fabs(vector2_distance(coordinates_to_screen(cs, circle->center->coordinates), point) - circle->radius * cs->zoom) <= OVERLAP_DISTANCE;
}

static bool _point_is_defined_by(IShape* self __attribute__((unused)), IShape* shape __attribute__((unused)))
{
    return false;
}
static bool _line_is_defined_by(IShape* self, IShape* shape)
{
    Line* line = (Line*)self;
    return (IShape*)line->p1 == shape || (IShape*)line->p2 == shape;
}
static bool _circle_is_defined_by(IShape* self, IShape* shape)
{
    return (IShape*)((Circle*)self)->center == shape;
}

static void _coordinate_system_remove_shape(CoordinateSystem* cs, IShape* shape)
{
    vector_remove(cs->shapes, shape);
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        IShape* shp = vector_get(cs->shapes, i);
        if (shp->is_defined_by(shp, (IShape*)shape))
        {
            shp->destroy(cs, shp);
            i--;
        }
    }
}