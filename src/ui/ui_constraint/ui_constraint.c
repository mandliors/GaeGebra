#include "ui_constraint.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UIConstraint new_pixel_constraint(int value)
{
    UIConstraint constraint = {.value = value, .constraint_type = CT_PIXEL};
    return constraint;
}
UIConstraint new_center_constraint()
{
    UIConstraint constraint = {.value = -1, .constraint_type = CT_CENTER};
    return constraint;
}
UIConstraint new_relative_constraint(double value)
{
    UIConstraint constraint = {.value = value, .constraint_type = CT_RELATIVE};
    return constraint;
}
UIConstraint new_offset_constraint(double value)
{
    UIConstraint constraint = {.value = value, .constraint_type = CT_OFFSET};
    return constraint;
}
UIConstraint new_aspect_constraint(double value)
{
    UIConstraint constraint = {.value = value, .constraint_type = CT_ASPECT};
    return constraint;
}
UIConstraints constraints_from_string(const char* string)
{
    UIConstraints constraints;
    char constraint_strings[4][10];
    sscanf(string, "%s %s %s %s", constraint_strings[0], constraint_strings[1], constraint_strings[2], constraint_strings[3]);
    for (size_t i = 0; i < 4; i++)
    {
        size_t length = strlen(constraint_strings[i]);
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