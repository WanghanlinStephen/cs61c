/*
 * Project 1-1: Quadtree Compression
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define HETEROGENEOUS -1

static int get_index(int map_width, int x, int y);

static qNode *quad(unsigned char *depth_map, int map_width, int x, int y, int section_width);

static qNode *create_qtree();

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    unsigned char color = depth_map[get_index(map_width, x, y)];

    for (int y_offset = 0; y_offset < section_width; y_offset++) {
        for (int x_offset = 0; x_offset < section_width; x_offset++) {
            if (color != depth_map[get_index(map_width, x + x_offset, y + y_offset)]) {
                return HETEROGENEOUS;
            }
        }
    }

    return (int) color;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    /* YOUR CODE HERE */
    return quad(depth_map, map_width, 0, 0, map_width);
}

void free_qtree(qNode *qtree_node) {
    /* YOUR CODE HERE */
    if (qtree_node != NULL && !(qtree_node-> leaf)) {
        free_qtree(qtree_node->child_NW);
        free_qtree(qtree_node->child_NE);
        free_qtree(qtree_node->child_SE);
        free_qtree(qtree_node->child_SW);
    }
    free(qtree_node);
}

static int get_index(int map_width, int x, int y) {
    return map_width * y + x;
}

static qNode *quad(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    if (section_width == 0) {
        return NULL;
    }

    qNode *root = create_qtree();
    root->size = section_width;
    root->x = x;
    root->y = y;
    if (section_width == 1
            || homogenous(depth_map, map_width, x, y, section_width) != HETEROGENEOUS) {
        root->leaf = 1;
        root->gray_value = depth_map[get_index(map_width, x, y)];
    } else {
        int sub_width = section_width / 2;
        root->leaf = 0;
        root->gray_value = HETEROGENEOUS;
        root->child_NW = quad(depth_map, map_width, x, y, sub_width);
        root->child_NE = quad(depth_map, map_width, x + sub_width, y,
            sub_width);
        root->child_SE = quad(depth_map, map_width, x + sub_width,
            y + sub_width, sub_width);
        root->child_SW = quad(depth_map, map_width, x, y + sub_width,
            sub_width);
    }

    return root;
}

static qNode *create_qtree() {
    qNode *node = calloc(1, sizeof(qNode));

    if (node == NULL) {
        allocation_failed();
    }

    return node;
}