#include "shape.h"

#include "../coordinate_system/coordinate_system.h"
#include "../../renderer/renderer.h"

#include <math.h>

static void _point_draw(CoordinateSystem* cs, IShape* self);
static void _line_draw(CoordinateSystem* cs, IShape* self);
static void _circle_draw(CoordinateSystem* cs, IShape* self);

static void _point_draw_selected(CoordinateSystem* cs, IShape* self);
static void _line_draw_selected(CoordinateSystem* cs, IShape* self);
static void _circle_draw_selected(CoordinateSystem* cs, IShape* self);

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
    point->base.draw_selected = _point_draw_selected;
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
    line->base.draw_selected = _line_draw_selected;
    line->base.translate = _line_translate;
    line->base.destroy = _line_destroy;
    line->base.overlap_point = _line_overlap;
    line->base.is_defined_by = _line_is_defined_by;
    line->p1 = p1;
    line->p2 = p2;
    vector_push_back(cs->shapes, line);
    return line;
}
Circle* circle_create(CoordinateSystem* cs, Point* center, Point* perimeter_point)
{
    Circle* circle = malloc(sizeof(Circle));
    circle->base.draw = _circle_draw;
    circle->base.draw_selected = _circle_draw_selected;
    circle->base.translate = _circle_translate;
    circle->base.destroy = _circle_destroy;
    circle->base.overlap_point = _circle_overlap;
    circle->base.is_defined_by = _circle_is_defined_by;
    circle->center = center;
    circle->perimeter_point = perimeter_point;
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
    Vector2 dir = vector2_normalize(vector2_subtract(p2, p1));
    double scale = fmax(cs->position.x + cs->size.x, cs->position.y + cs->size.y);
    Vector2 p1_translated = vector2_add(p1, vector2_scale(dir, -scale));
    Vector2 p2_translated = vector2_add(p2, vector2_scale(dir,  scale));
    renderer_draw_line(p1_translated.x, p1_translated.y, p2_translated.x, p2_translated.y, 2, BLACK);
}
static void _circle_draw(CoordinateSystem* cs, IShape* self)
{
    Circle* circle = (Circle*)self;
    Vector2 position = coordinates_to_screen(cs, circle->center->coordinates);
    double radius = vector2_distance(position, coordinates_to_screen(cs, circle->perimeter_point->coordinates));
    for (size_t r = radius - 1; r < radius + 2; r++)
        renderer_draw_circle(position.x, position.y, r, BLACK);
}

static void _point_draw_selected(CoordinateSystem* cs, IShape* self)
{
    Point* point = (Point*)self;
    Vector2 position = coordinates_to_screen(cs, point->coordinates);
    renderer_draw_filled_circle(position.x, position.y, 9, color_fade(BLACK, 0.3));
}
static void _line_draw_selected(CoordinateSystem* cs, IShape* self)
{
    Line* line = (Line*)self;
    Vector2 p1 = coordinates_to_screen(cs, line->p1->coordinates);
    Vector2 p2 = coordinates_to_screen(cs, line->p2->coordinates);
    Vector2 dir = vector2_normalize(vector2_subtract(p2, p1));
    double scale = fmax(cs->position.x + cs->size.x, cs->position.y + cs->size.y);
    Vector2 p1_translated = vector2_add(p1, vector2_scale(dir, -scale));
    Vector2 p2_translated = vector2_add(p2, vector2_scale(dir,  scale));
    renderer_draw_line(p1_translated.x, p1_translated.y, p2_translated.x, p2_translated.y, 6, color_fade(BLACK, 0.3));
}
static void _circle_draw_selected(CoordinateSystem* cs, IShape* self)
{
    Circle* circle = (Circle*)self;
    Vector2 position = coordinates_to_screen(cs, circle->center->coordinates);
    double radius = vector2_distance(position, coordinates_to_screen(cs, circle->perimeter_point->coordinates));
    for (size_t r = radius - 3; r < radius + 4; r++)
        renderer_draw_circle(position.x, position.y, r, color_fade(BLACK, 0.3));
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
    _point_translate(cs, (IShape*)circle->perimeter_point, translation);
}

static bool _point_overlap(CoordinateSystem* cs, IShape* self, Vector2 point)
{
    return vector2_distance(coordinates_to_screen(cs, ((Point*)self)->coordinates), point) <= OVERLAP_DISTANCE;
}
static bool _line_overlap(CoordinateSystem* cs, IShape* self, Vector2 point)
{
    Line* line = (Line*)self;
    Vector2 screen_normal = vector2_normalize(vector2_rotate90(vector2_subtract(coordinates_to_screen(cs, line->p2->coordinates), coordinates_to_screen(cs, line->p1->coordinates))));
    //My method
    return fabs(vector2_dot(screen_normal, point) - 
                vector2_dot(screen_normal, coordinates_to_screen(cs, line->p1->coordinates))) 
                <= vector2_length(screen_normal) * OVERLAP_DISTANCE;
}
static bool _circle_overlap(CoordinateSystem* cs, IShape* self, Vector2 point)
{
    Circle* circle = (Circle*)self;
    double radius = vector2_distance(coordinates_to_screen(cs, circle->center->coordinates), coordinates_to_screen(cs, circle->perimeter_point->coordinates));
    return fabs(vector2_distance(coordinates_to_screen(cs, circle->center->coordinates), point) - radius) <= OVERLAP_DISTANCE;
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
    return (IShape*)((Circle*)self)->center == shape || (IShape*)((Circle*)self)->perimeter_point == shape;
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