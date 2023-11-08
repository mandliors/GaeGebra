#include "coordinate_system.h"

#include "../../renderer/renderer.h"

static double _x_screen_to_coordinate(CoordinateSystem* cs, double x);
static double _y_screen_to_coordinate(CoordinateSystem* cs, double y);
static double _x_coordinate_to_screen(CoordinateSystem* cs, double x);
static double _y_coordinate_to_screen(CoordinateSystem* cs, double y);

CoordinateSystem* coordinate_system_create(Vector2 position, Vector2 size, Vector2 origin)
{
    CoordinateSystem* cs = (CoordinateSystem*)malloc(sizeof(CoordinateSystem));
    cs->position = position;
    cs->size = size;
    cs->origin = origin;
    cs->zoom = INITIAL_ZOOM;
    cs->shapes = vector_create(0);
    return cs;
}
void coordinate_system_free(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        shape->free(shape);
    }
    vector_free(cs->shapes);
    free(cs);
}

Vector2 screen_to_coordinates(CoordinateSystem* cs, Vector2 point)
{
    if (cs == NULL)
        return vector2_create(-1, -1);

    return (Vector2){_x_screen_to_coordinate(cs, point.x),
                     _y_screen_to_coordinate(cs, point.y)};
}
Vector2 coordinates_to_screen(CoordinateSystem* cs, Vector2 point)
{
    if (cs == NULL)
        return vector2_create(-1, -1);

    return (Vector2){_x_coordinate_to_screen(cs, point.x),
                     _y_coordinate_to_screen(cs, point.y)};
}

void coordinate_system_add_shape(CoordinateSystem* cs, Shape* shape)
{
    if (cs == NULL)
        return;
    vector_push_back(cs->shapes, shape);
}
void coordinate_system_remove_shape(CoordinateSystem* cs, Shape* shape)
{
    if (cs == NULL)
        return;
    vector_remove(cs->shapes, shape);
}

Shape* coordinate_system_get_hovered_shape(CoordinateSystem* cs, Vector2 point)
{
    if (cs == NULL)
        return NULL;
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->overlap_point(shape, point))
            return shape;
    }
    return NULL;
}

void coordinate_system_translate(CoordinateSystem* cs, Vector2 translation)
{
    if (cs == NULL)
        return;
    translation = vector2_divide(translation, cs->size);
    cs->origin = vector2_add(cs->origin, translation);
}
void coordinate_system_zoom(CoordinateSystem* cs, double zoom)
{
    if (cs == NULL)
        return;
    cs->zoom *= zoom;
}
void coordinate_system_update_dimensions(CoordinateSystem* cs, Vector2 position, Vector2 size)
{
    if (cs == NULL)
        return;
    cs->position = position;
    cs->size = size;
}
void coordinate_system_draw(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;
    
    double step = cs->zoom;
    double y = cs->origin.y * cs->size.y + cs->position.y;
    for (double x = cs->origin.x * cs->size.x; x > -step; x -= step)
    {
        Vector2 p1 = vector2_create(x, y - 5);
        Vector2 p2 = vector2_create(x, y + 5);
        renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 1, BLACK);
    }
    for (double x = cs->origin.x * cs->size.x; x < cs->size.x + step; x += step)
    {
        Vector2 p1 = vector2_create(x, y - 5);
        Vector2 p2 = vector2_create(x, y + 5);
        renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 1, BLACK);
    }

    double x = cs->origin.x * cs->size.x + cs->position.x;
    for (double y = cs->origin.y * cs->size.y + cs->position.y; y > -step; y -= step)
    {
        Vector2 p1 = vector2_create(x + 5, y);
        Vector2 p2 = vector2_create(x - 5, y);
        renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 1, BLACK);
    }
    for (double y = cs->origin.y * cs->size.y + cs->position.y; y < cs->size.y + cs->position.y + step; y += step)
    {
        Vector2 p1 = vector2_create(x + 5, y);
        Vector2 p2 = vector2_create(x - 5, y);
        renderer_draw_line(p1.x, p1.y, p2.x, p2.y, 1, BLACK);
    }

    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        shape->draw(shape);
    }
}

static double _x_screen_to_coordinate(CoordinateSystem* cs, double x)
{
    x -= cs->position.x + cs->origin.x * cs->size.x;
    x /= cs->zoom;
    return x;
}
static double _y_screen_to_coordinate(CoordinateSystem* cs, double y)
{
    y -= cs->position.y + cs->origin.y * cs->size.y;
    y /= -cs->zoom;
    return y;
}
static double _x_coordinate_to_screen(CoordinateSystem* cs, double x)
{
    x *= cs->zoom;
    x += cs->origin.x * cs->size.x + cs->position.x;
    return x;
}
static double _y_coordinate_to_screen(CoordinateSystem* cs, double y)
{
    y *= -cs->zoom;
    y += cs->origin.y * cs->size.y + cs->position.y;
    return y;    
}