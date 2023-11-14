#pragma once

typedef enum ConstraintType
{
    CT_PIXEL = 0,
    CT_CENTER,
    CT_RELATIVE,
    CT_OFFSET,
    CT_ASPECT
} ConstraintType;

typedef struct UIConstraint UIConstraint;
typedef struct UIConstraint
{
    double value;
    ConstraintType constraint_type;
} UIConstraint;

typedef struct UIConstraints
{
    UIConstraint x, y, width, height;
} UIConstraints;

//API functions
UIConstraint new_pixel_constraint(int value);
UIConstraint new_center_constraint();
UIConstraint new_relative_constraint(double value);
UIConstraint new_offset_constraint(double value);
UIConstraint new_aspect_constraint(double value);
UIConstraints constraints_from_string(const char* string);
