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
#include <stdbool.h>
#include <limits.h>

#define DISPLACEMENT_INFINITY 0
#define DISPLACEMENT_ZERO 255

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(DISPLACEMENT_ZERO * sqrt(squared_displacement)
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
    int p1 = a;
    int p2 = b;
    int diff = p1 - p2;
    return diff * diff;
}

static bool has_feature(int image_width, int image_height, int x, int y,
        int feature_width, int feature_height);

static int index(int image_width, int image_height, int x, int y);

static int distance(unsigned char *left, unsigned char *right, int image_width,
    int image_height, int x_left, int y_left, int x_right, int y_right,
    int feature_width, int feature_height);

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {
    /* YOUR CODE HERE */
    for (int y_left = 0; y_left < image_height; y_left++) {
        for (int x_left = 0; x_left < image_width; x_left++) {
            int index_left = index(image_width, image_height, x_left, y_left);
            depth_map[index_left] = DISPLACEMENT_INFINITY;

            if (has_feature(image_width, image_height, x_left, y_left,
                    feature_width, feature_height) && maximum_displacement > 0) {
                int dist_min = INT_MAX;
                int displacement_min = INT_MAX;

                for (int y_offset = -maximum_displacement;
                        y_offset <= maximum_displacement; y_offset++) {
                    for (int x_offset = -maximum_displacement;
                            x_offset <= maximum_displacement; x_offset++) {
                        int y_right = y_left + y_offset;
                        int x_right = x_left + x_offset;

                        if (has_feature(image_width, image_height, x_right, y_right,
                                feature_width, feature_height)) {
                            int dist = distance(left, right, image_width,
                                image_height, x_left, y_left, x_right,
                                y_right, feature_width, feature_height);
                            int displacement = normalized_displacement(
                                x_left - x_right, y_left - y_right,
                                maximum_displacement);
                            
                            if (dist < dist_min
                                || (dist == dist_min && displacement < displacement_min)) {
                                dist_min = dist;
                                displacement_min = displacement;
                            }
                        }
                    }
                }

                depth_map[index_left] = displacement_min;
            }
        }
    }
}

static bool has_feature(int image_width, int image_height, int x, int y,
        int feature_width, int feature_height) {
    return y - feature_height >= 0
        && y + feature_height < image_height
        && x - feature_width >= 0
        && x + feature_width < image_width;
}

static int index(int image_width, int image_height, int x, int y) {
    return image_width * y + x;
}

static int distance(unsigned char *left, unsigned char *right, int image_width,
        int image_height, int x_left, int y_left, int x_right, int y_right,
        int feature_width, int feature_height) {
    int distance = 0;

    for (int row_offset = -feature_height; row_offset <= feature_height; row_offset++) {
        for (int col_offset = -feature_width; col_offset <= feature_width; col_offset++) {
            int yl = y_left + row_offset;
            int xl = x_left + col_offset;
            int yr = y_right + row_offset;
            int xr = x_right + col_offset;
            unsigned char pixel_left = left[index(image_width, image_height, xl, yl)];
            unsigned char pixel_right = right[index(image_width, image_height, xr, yr)];
            distance += square_euclidean_distance(pixel_left, pixel_right);
        }
    }

    return distance;
}