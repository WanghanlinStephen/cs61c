#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./print_bmp bmp_file\n");
        return EXIT_FAILURE;
    }
    char *file_path = argv[1];
    Image bmp_image = load_image(file_path);
    print_image(bmp_image.data, bmp_image.width, bmp_image.height);
    free(bmp_image.data);

    return EXIT_SUCCESS;
}