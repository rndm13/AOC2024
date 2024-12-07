#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_table.h"

#define MAX_ROW_COUNT 131

typedef struct Point {
    int16_t x, y;
} Point;

Point find_guard(char **map, size_t row_count, size_t col_count) {
    for (int32_t i = 0; i < row_count; i++) {
        for (int32_t j = 0; j < col_count; j++) {
            if (map[i][j] == '^') {
                Point p = {.x = i, .y = j};
                return p;
            }
        }
    }
    Point p = {.x = -1, .y = -1};
    return p;
}

Point rotate_right(Point dir) {
    Point p = {
        .x = dir.y,
        .y = -dir.x,
    };
    return p;
}

char dir_mask(Point dir) {
    uint8_t mask =
        (dir.x < 0) << 3 | // up
        (dir.y > 0) << 2 | // right
        (dir.x > 0) << 1 | // down
        (dir.y < 0) << 0;  // left
    return mask;
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

    Point guard_dir = {.x = -1, .y = 0};
    Point guard_pos = find_guard(map, row_count, col_count);
    map[guard_pos.x][guard_pos.y] = dir_mask(guard_dir);

    size_t p1_res = 1; // Include starting square
    size_t p2_res = 0;

#define IN_BOUNDS(pos) \
    ((pos).x >= 0 && (pos).x < row_count && (pos).y >= 0 && (pos).y < col_count)

    // int dot =   0b00101110;
    // int block = 0b00100011;
    // int guard = 0b01011110;

    hash_table alt_points;
    hash_table_init(&alt_points);

    while (IN_BOUNDS(guard_pos)) {
        if (map[guard_pos.x][guard_pos.y] == '.') {
            p1_res += 1;
            map[guard_pos.x][guard_pos.y] = '\0';
        }
        map[guard_pos.x][guard_pos.y] |= dir_mask(guard_dir);
        Point next_pos = guard_pos;
        next_pos.x += guard_dir.x;
        next_pos.y += guard_dir.y;

        if (IN_BOUNDS(next_pos) && map[next_pos.x][next_pos.y] == '#') {
            guard_dir = rotate_right(guard_dir);
            map[guard_pos.x][guard_pos.y] |= dir_mask(guard_dir);
            continue;
        }

        // We cannot change the next to be a block if we already walked over it in the past
        if (IN_BOUNDS(next_pos) && map[next_pos.x][next_pos.y] == '.') {
            // If we were to turn right now will there be the exact same directions further on the way?
            map[next_pos.x][next_pos.y] = '#';
            Point alt_dir = rotate_right(guard_dir);
            Point alt_pos = guard_pos;

            // Hash table for storing temporary directions
            hash_table_clear(&alt_points);

            while (IN_BOUNDS(alt_pos)) {
                Point next_alt_pos = alt_pos;
                next_alt_pos.x += alt_dir.x;
                next_alt_pos.y += alt_dir.y;

                int32_t map_char = map[alt_pos.x][alt_pos.y];
                if (map_char == '.') {
                    map_char = '\0';
                }
                int32_t *alt_char = hash_table_get_or(
                    &alt_points, *(int32_t *)&alt_pos,
                    &map_char);

                if (IN_BOUNDS(next_alt_pos) && map[next_alt_pos.x][next_alt_pos.y] == '#') {
                    hash_table_set(&alt_points,
                            *(int32_t *)&alt_pos,
                            *alt_char | dir_mask(alt_dir));

                    alt_dir = rotate_right(alt_dir);
                    continue;
                }

                if (!(*alt_char & (0xf0)) && *alt_char & dir_mask(alt_dir)) {
                    // printf("%d %d\n", guard_pos.x, guard_pos.y);
                    // map[guard_pos.x][guard_pos.y] |= 0x80;
                    p2_res += 1;
                    break;
                }

                hash_table_set(&alt_points,
                        *(int32_t *)&alt_pos,
                        *alt_char | dir_mask(alt_dir));

                alt_pos = next_alt_pos;
            }

            map[next_pos.x][next_pos.y] = '.';
        }

        guard_pos = next_pos;
    }

    printf("PART 1: %zu\n", p1_res);
    printf("PART 2: %zu\n", p2_res);

    hash_table_deinit(&alt_points);

    // Free the EOF row as well
    for (int32_t i = 0; i <= row_count; i++) {
        free(map[i]);
    }
    free(map);

    if (file != stdin) {
        fclose(file);
    }

    return EXIT_SUCCESS;
}
