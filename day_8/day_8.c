#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_table.h"
#include "../vec.h"

#define MAX_ROW_COUNT 256
#define ANTENNA_COUNT 256

typedef struct Point {
    int16_t x, y;
} Point;

typedef struct VecPoint {
    size_t size;
    size_t capacity;
    Point *data;
} VecPoint;

#define IN_BOUNDS(p) \
    (p.x >= 0 && p.x < row_count && p.y >= 0 && p.y < col_count)

size_t count_antinodes(VecPoint* antennas, size_t row_count, size_t col_count, size_t start_diff, size_t diff_count) {
    hash_table antinodes;
    hash_table_init(&antinodes);

    size_t antinode_count = 0;
    for (int32_t c = 0; c < ANTENNA_COUNT; c++) {
        if (antennas[c].data == NULL) {
            continue;
        }

        VecPoint *antenna_points = &antennas[c];

        for (int32_t i = 0; i < antenna_points->size; i++) {
            for (int32_t j = i + 1; j < antenna_points->size; j++) {
                Point diff1 = {
                    .x = antenna_points->data[i].x - antenna_points->data[j].x,
                    .y = antenna_points->data[i].y - antenna_points->data[j].y,
                };
                Point n1 = antenna_points->data[i];
                n1.x += diff1.x * start_diff;
                n1.y += diff1.y * start_diff;

                for (size_t it = 0; it < diff_count && IN_BOUNDS(n1); it++) {
                    if (!hash_table_get(&antinodes, *(int32_t *)&n1)) {
                        hash_table_set(&antinodes, *(int32_t *)&n1, 1);
                        antinode_count++;
                    }
                    n1.x += diff1.x;
                    n1.y += diff1.y;
                }

                Point diff2 = {
                    .x = antenna_points->data[j].x - antenna_points->data[i].x,
                    .y = antenna_points->data[j].y - antenna_points->data[i].y,
                };
                Point n2 = antenna_points->data[j];
                n2.x += diff2.x * start_diff;
                n2.y += diff2.y * start_diff;

                for (size_t it = 0; it < diff_count && IN_BOUNDS(n2); it++) {
                    if (!hash_table_get(&antinodes, *(int32_t *)&n2)) {
                        hash_table_set(&antinodes, *(int32_t *)&n2, 1);
                        antinode_count++;
                    }
                    n2.x += diff2.x;
                    n2.y += diff2.y;
                }
            }
        }
    }

    hash_table_deinit(&antinodes);

    return antinode_count;
}

int main(int argc, char *argv[]) {
    FILE *file = stdin;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }

    char **map = malloc(sizeof(char *) * MAX_ROW_COUNT);
    memset(map, 0, sizeof(char *) * MAX_ROW_COUNT);
    size_t size = 0;

    size_t col_count = 0;
    size_t row_count = 0;

    while (getline(&map[row_count], &size, file) > 0) {
        size = 0;
        row_count++;
    }
    col_count = strlen(map[0]) - 1; // Except newline

    VecPoint *antennas = malloc(ANTENNA_COUNT * sizeof(vec_i32));
    memset(antennas, 0, sizeof(vec_i32) * ANTENNA_COUNT);

    for (int16_t i = 0; i < row_count; i++) {
        for (int16_t j = 0; j < col_count; j++) {
            if (map[i][j] != '.') {
                Point p = {
                    .x = i,
                    .y = j,
                };

                push_vec(antennas[map[i][j]], &p, 1);
            }
        }
    }

    printf("PART 1: %zu\n", count_antinodes(antennas, row_count, col_count, 1, 1));
    printf("PART 2: %zu\n", count_antinodes(antennas, row_count, col_count, 0, -1));

    for (int32_t i = 0; i < ANTENNA_COUNT; i++) {
        free(antennas[i].data);
    }
    free(antennas);

    // Free with EOF row
    for (int32_t i = 0; i <= row_count; i++) {
        free(map[i]);
    }
    free(map);

    if (file != stdin) {
        fclose(file);
    }
    return EXIT_SUCCESS;
}
