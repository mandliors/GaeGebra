#pragma once

/**
 * @brief The constraint types available for UIConstraints
 */
typedef enum ConstraintType
{
    CT_PIXEL = 0,
    CT_CENTER,
    CT_RELATIVE,
    CT_OFFSET,
    CT_ASPECT
} ConstraintType;


/**
 * @brief A constraint for a UIElement
 * A constraint is a value that can be used to calculate the position or size of a UIElement.
 * A pixel constraint is a fixed value in pixels (can be negative to measure it from the right).
 * A center constraint represents the center of the parent element.
 * A relative constraint is a value between 0 and 1 that represents the percentage of the parent element.
 * An offset constraint represents the offset from the parent element in pixels.
 * An aspect constraint is a value that represents the aspect ratio of a UIElement.
 */
typedef struct UIConstraint
{
    double value;
    ConstraintType constraint_type;
} UIConstraint;

/**
 * @brief A set of constraints for a UIElement
 */
typedef struct UIConstraints
{
    UIConstraint x, y, width, height;
} UIConstraints;

/**
 * @brief Creates a new pixel constraint
 * @param value The value of the constraint in pixels
 * @return The new constraint
 */
UIConstraint new_pixel_constraint(int value);
/**
 * @brief Creates a new center constraint
 * 
 * @return UIConstraint 
 */
UIConstraint new_center_constraint();
/**
 * @brief Creates a new relative constraint
 * @param value The value of the constraint between 0 and 1
 * @return The new constraint
 */
UIConstraint new_relative_constraint(double value);
/**
 * @brief Creates a new offset constraint
 * @param value The value of the constraint in pixels
 * @return The new constraint
 */
UIConstraint new_offset_constraint(double value);
/**
 * @brief Creates a new aspect constraint
 * @param value The ratio of the constraint between 0 and 1
 * @return The new constraint
 */
UIConstraint new_aspect_constraint(double value);

/**
 * @brief Creates a new set of constraints using a string (this is the recommended way to create constraints)
 * Pixel constraints are represented by a number followed by "p"
 * Center constraints are represented by "c"
 * Relative constraints are represented by a number followed by "r"
 * Offset constraints are represented by a number followed by "o"
 * Aspect constraints are represented by a number followed by "a"
 * Constraints are separated by spaces
 * example: "100p c 0.5r 1.0a" or "10o -100p 0.5a 0.8r"
 * 
 * @param string The string containing the constraints
 * @return UIConstraints The new constraints
 */
UIConstraints constraints_from_string(const char* string);