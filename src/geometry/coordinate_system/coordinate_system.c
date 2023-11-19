#include "coordinate_system.h"

#include "../../renderer/renderer.h"
#include "../intersection/intersection.h"
#include "../../utils/math/math.h"

static double _x_screen_to_coordinate(CoordinateSystem* cs, double x);
static double _y_screen_to_coordinate(CoordinateSystem* cs, double y);
static double _x_coordinate_to_screen(CoordinateSystem* cs, double x);
static double _y_coordinate_to_screen(CoordinateSystem* cs, double y);

static Point* _intersection_point_create(CoordinateSystem* cs, Vector2 coordinates);
static void _intersection_point_draw(CoordinateSystem* cs, Point* self);

CoordinateSystem* coordinate_system_create(Vector2 position, Vector2 size, Vector2 origin)
{
    CoordinateSystem* cs = (CoordinateSystem*)malloc(sizeof(CoordinateSystem));
    cs->position = position;
    cs->size = size;
    cs->origin = origin;
    cs->zoom = INITIAL_ZOOM;
    cs->shapes = vector_create(0);
    cs->intersection_points = vector_create(0);
    cs->dragged_shape = NULL;
    return cs;
}
void coordinate_system_destroy(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;
    for (size_t i = 0; i < vector_size(cs->intersection_points); i++)
    {
        Shape* intersection_point = vector_get(cs->intersection_points, i);
        shape_destroy(cs, intersection_point);
    }
    vector_free(cs->intersection_points);
    while (vector_size(cs->shapes) > 0)
    {
        Shape* shape = vector_get(cs->shapes, 0);
        coordinate_system_destroy_shape(cs, shape);
    }
    vector_free(cs->shapes);
    free(cs);
}

void coordinate_system_save(CoordinateSystem* cs, const char* path)
{
    FILE* file = fopen(path, "w");
    if (file == NULL)
        return;
    
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        switch (shape->type)
        {
        case ST_POINT:
        {
            fprintf(file, "point %lf %lf\n", ((Point*)shape)->coordinates.x, ((Point*)shape)->coordinates.y);
            break;
        }
        case ST_LINE:
        {
            int idx1 = vector_index_of(cs->shapes, ((Line*)shape)->p1);
            int idx2 = vector_index_of(cs->shapes, ((Line*)shape)->p2);
            fprintf(file, "line %d %d\n", idx1, idx2);
            break;
        }
        case ST_CIRCLE:
        {
            int idx1 = vector_index_of(cs->shapes, ((Circle*)shape)->center);
            int idx2 = vector_index_of(cs->shapes, ((Circle*)shape)->perimeter_point);
            fprintf(file, "circle %d %d\n", idx1, idx2);
            break;
        }
        default:
            break;
        }
    }
    fclose(file);
}
CoordinateSystem* coordinate_system_load(const char* path)
{
    CoordinateSystem* cs = coordinate_system_create(vector2_create(0, 0), vector2_create(0, 0), vector2_create(0.5, 0.5));
    FILE* file = fopen(path, "r");
    if (file == NULL)
        return cs;
    char buffer[256];
    while (fgets(buffer, 256, file) != NULL)
    {
        char* type = strtok(buffer, " ");
        if (strcmp(type, "point") == 0)
        {
            double x = atof(strtok(NULL, " "));
            double y = atof(strtok(NULL, " "));
            point_create(cs, vector2_create(x, y));
        }
        else if (strcmp(type, "line") == 0)
        {
            int idx1 = atoi(strtok(NULL, " "));
            int idx2 = atoi(strtok(NULL, " "));
            line_create(cs, vector_get(cs->shapes, idx1), vector_get(cs->shapes, idx2));
        }
        else if (strcmp(type, "circle") == 0)
        {
            int idx1 = atoi(strtok(NULL, " "));
            int idx2 = atoi(strtok(NULL, " "));
            circle_create(cs, vector_get(cs->shapes, idx1), vector_get(cs->shapes, idx2));
        }
    }
    fclose(file);
    return cs;
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

bool coordinate_system_is_hovered(CoordinateSystem* cs, Vector2 point)
{
    if (cs == NULL)
        return false;
    return check_collision_point_rect(point.x, point.y, cs->position.x, cs->position.y, cs->size.x, cs->size.y);
}
void coordinate_system_select_shape(CoordinateSystem* cs, Shape* shape)
{
    if (cs == NULL || shape == NULL)
        return;
    shape->selected = true;

}
void coordinate_system_set_dragged_shape(CoordinateSystem* cs, Shape* shape)
{
    if (cs == NULL)
        return;
    cs->dragged_shape = shape;
}
Shape* coordinate_system_get_hovered_shape(CoordinateSystem* cs, Vector2 point)
{
    if (cs == NULL || !coordinate_system_is_hovered(cs, point))
        return NULL;
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->type == ST_POINT && shape_overlap_point(cs, shape, point))
            return shape;
    }
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->type != ST_POINT && shape_overlap_point(cs, shape, point))
            return shape;
    }
    return NULL;
}
Vector* coordinate_system_get_selected_shapes(CoordinateSystem* cs)
{
    Vector* shapes = vector_create(0);
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->selected)
            vector_push_back(shapes, shape);
    }
    return shapes;
}
Shape* coordinate_system_get_dragged_shape(CoordinateSystem* cs)
{
    return cs->dragged_shape;
}
void coordinate_system_deselect_shapes(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        shape->selected = false;
    }

}
void coordinate_system_delete_selected_shapes(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->selected)
            coordinate_system_destroy_shape(cs, shape);
    }
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
void coordinate_system_update(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;

    for (size_t i = 0; i < vector_size(cs->shapes); i++)
        shape_update(cs, vector_get(cs->shapes, i));

    for (size_t i = 0; i < vector_size(cs->intersection_points); i++)
        shape_destroy(cs, (Shape*)vector_get(cs->intersection_points, i));
    vector_clear(cs->intersection_points);
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape1 = vector_get(cs->shapes, i);
        for (size_t j = i + 1; j < vector_size(cs->shapes); j++)
        {
            Shape* shape2 = vector_get(cs->shapes, j);
            Intersection intersection = intersection_get(shape1, shape2);
            if (intersection.points != NULL)
                for (size_t i = 0; i < vector_size(intersection.points); i++)
                    _intersection_point_create(cs, *(Vector2*)vector_get(intersection.points, i));
            intersection_destroy(&intersection);
        }
    }
}
void coordinate_system_draw(CoordinateSystem* cs)
{
    if (cs == NULL)
        return;

    Color grid_color = color_from_grayscale(240);
    double step = cs->zoom;
    double y = cs->origin.y * cs->size.y + cs->position.y;
    for (double x = cs->origin.x * cs->size.x; x > -step; x -= step)
    {
        renderer_draw_line(x, cs->position.y - 10, x, cs->position.y + cs->size.y + 10, 1, grid_color);
        renderer_draw_line(x, y - 5, x, y + 5, 1, BLACK);
    }
    for (double x = cs->origin.x * cs->size.x; x < cs->size.x + step; x += step)
    {
        renderer_draw_line(x, cs->position.y - 10, x, cs->position.y + cs->size.y + 10, 1, grid_color);
        renderer_draw_line(x, y - 5, x, y + 5, 1, BLACK);
    }

    double x = cs->origin.x * cs->size.x + cs->position.x;
    for (double y = cs->origin.y * cs->size.y + cs->position.y; y > -step; y -= step)
    {
        renderer_draw_line(cs->position.x - 10, y, cs->position.x - 10 + cs->size.x + 10, y, 1, grid_color);
        renderer_draw_line(x + 5, y, x - 5, y, 1, BLACK);
    }
    for (double y = cs->origin.y * cs->size.y + cs->position.y; y < cs->size.y + cs->position.y + step; y += step)
    {
        renderer_draw_line(cs->position.x - 10, y, cs->position.x - 10 + cs->size.x + 10, y, 1, grid_color);
        renderer_draw_line(x + 5, y, x - 5, y, 1, BLACK);
    }

    x = cs->position.x + cs->origin.x * cs->size.x;
    y = cs->position.y + cs->origin.y * cs->size.y;
    renderer_draw_line(x, cs->position.y - 10, x, cs->position.y + cs->size.y + 10, 1, BLACK);
    renderer_draw_line(cs->position.x - 10, y, cs->position.x + cs->size.x + 10, y, 1, BLACK);

    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->type != ST_POINT)
            shape_draw(cs, shape);
    }
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shape = vector_get(cs->shapes, i);
        if (shape->type == ST_POINT)
            shape_draw(cs, shape);
    }
    for (size_t i = 0; i < vector_size(cs->intersection_points); i++)
    {
        Point* point = vector_get(cs->intersection_points, i);
        _intersection_point_draw(cs, point);
    }
}
void coordinate_system_update_dimensions(CoordinateSystem* cs, Vector2 position, Vector2 size)
{
    if (cs == NULL)
        return;
    cs->position = position;
    cs->size = size;
}
void coordinate_system_destroy_shape(CoordinateSystem* cs, Shape* shape)
{
    shape_destroy(cs, shape);
    vector_remove(cs->shapes, shape);
    for (size_t i = 0; i < vector_size(cs->shapes); i++)
    {
        Shape* shp = vector_get(cs->shapes, i);
        if (shape_is_defined_by(shp, shape))
        {
            shape_destroy(cs, shp);
            vector_remove(cs->shapes, shp);
            i--;
        }
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

static Point* _intersection_point_create(CoordinateSystem* cs, Vector2 coordinates)
{
    Point* point = malloc(sizeof(Point));
    point->base.type = ST_POINT;
    point->base.selected = false;
    point->coordinates = coordinates;
    vector_push_back(cs->intersection_points, point);
    return point;
}
static void _intersection_point_draw(CoordinateSystem* cs, Point* self)
{
    Vector2 position = coordinates_to_screen(cs, self->coordinates);
    renderer_draw_circle(position.x, position.y, 6, WHITE);
    renderer_draw_circle(position.x, position.y, 5, DARK_GRAY);
    renderer_draw_circle(position.x, position.y, 4, DARK_GRAY);
    renderer_draw_filled_circle(position.x, position.y, 3, color_from_rgb(240, 240, 240));
}