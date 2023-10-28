#ifndef UI_CONSTRAINT_H
#define UI_CONSTRAINT_H

typedef enum ConstraintType
{
    CT_PIXEL = 0,
    CT_CENTER,
    CT_RELATIVE,
    CT_ASPECT
} ConstraintType;

typedef struct UIConstraint
{
    ConstraintType constraint_type;
    double value;
    void (*recalculate)(void* self);
} UIConstraint;

typedef struct UIConstraints
{
    UIConstraint *x, *y, *width, *height;
} UIConstraints;

//API functions
UIConstraint* new_pixel_constraint(int value);
UIConstraint* new_center_constraint();
UIConstraint* new_relative_constraint(double value);
UIConstraint* new_aspect_constraint(double value);

//internal functions
void _ui_constraints_free(UIConstraints* constraints);

#endif