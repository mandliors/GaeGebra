#include "ui_constraints.h"

#include "../../debugmalloc.h"

UIConstraints constraints_from_string(const char* string)
{
    UIConstraints constraints = {NULL, NULL, NULL, NULL};
    char constraint_strings[4][10];
    sscanf(string, "%s %s %s %s", constraint_strings[0], constraint_strings[1], constraint_strings[2], constraint_strings[3]);
    for (int i = 0; i < 4; i++)
    {
        int length = strlen(constraint_strings[i]);
        switch(constraint_strings[i][length - 1])
        {
            case 'p':
                *(&constraints.x + i) = new_pixel_constraint(atoi(constraint_strings[i]));
                break;
            case 'c':
                *(&constraints.x + i) = new_center_constraint();
                break;
            case 'r':
                *(&constraints.x + i) = new_relative_constraint(atof(constraint_strings[i]));
                break;
            case 'o':
                *(&constraints.x + i) = new_offset_constraint(atoi(constraint_strings[i]));
                break;
            case 'a':
                *(&constraints.x + i) = new_aspect_constraint(atof(constraint_strings[i]));
                break;
        }
    }
    return constraints;
}
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