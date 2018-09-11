/*
 * Project 1-1: Computing a Displacement Map
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement)
            / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;
}

/* Helper function to return the square euclidean distance between two values. */
unsigned int square_euclidean_distance(unsigned char a, unsigned char b) {
   /*
    * This is an optional helper function which you may find useful. It
    * currently has an incomplete example CUnit test to help with debugging.
    * This test is not graded and is merely for your convenience. See the spec
    * for details on how to use CUnit.
    */
    return 0;
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {
    /* YOUR CODE HERE */
}
