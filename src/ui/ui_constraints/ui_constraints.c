#include "ui_constraints.h"

#include "../../debugmalloc.h"

UIConstraint* new_pixel_constraint(int value)
{
    UIConstraint* constraint = (UIConstraint*)malloc(sizeof(UIConstraint));
    constraint->value = value;
    constraint->constraint_type = CT_PIXEL;
    return constraint;
}
UIConstraint* new_center_constraint()
{
    UIConstraint* constraint = (UIConstraint*)malloc(sizeof(UIConstraint));
    constraint->value = -1;
    constraint->constraint_type = CT_CENTER;
    return constraint;
}
UIConstraint* new_relative_constraint(double value)
{
    UIConstraint* constraint = (UIConstraint*)malloc(sizeof(UIConstraint));
    constraint->value = value;
    constraint->constraint_type = CT_RELATIVE;
    return constraint;
}
UIConstraint* new_offset_constraint(double value)
{
    UIConstraint* constraint = (UIConstraint*)malloc(sizeof(UIConstraint));
    constraint->value = value;
    constraint->constraint_type = CT_OFFSET;
    return constraint;
}
UIConstraint* new_aspect_constraint(double value)
{
    UIConstraint* constraint = (UIConstraint*)malloc(sizeof(UIConstraint));
    constraint->value = value;
    constraint->constraint_type = CT_ASPECT;
    return constraint;
}

void _ui_constraints_free(UIConstraints* constraints)
{
    free(constraints->x);
    free(constraints->y);
    free(constraints->width);
    free(constraints->height);
}