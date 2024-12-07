#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW_COUNT 150
#define MAX_COL_COUNT 150

int main(void) {
    size_t row_lengths[MAX_ROW_COUNT] = {0};
    char *map[MAX_ROW_COUNT] = {NULL};

    size_t row_count = 0;
    while (getline(&map[row_count], &row_lengths[row_count], stdin) > 0) {
        row_count++;
    }

    // Minus the new lines
    size_t col_count = row_lengths[0] - 1;

    const char *search_p1 = "XMAS";
    const char *rev_search_p1 = "SAMX";
    const int32_t search_length_p1 = strlen(search_p1);

    // printf("%zu\n", row_count);

#define MATCH(found, search_str, max_x, max_y, increment_x, increment_y) \
    for (int32_t it = 0, s_i = 0, s_j = 0;                               \
         s_i < (max_x) && s_j < (max_y) && (search_str)[it] != '\0';     \
         s_i += (increment_x), s_j += (increment_y), it++) {             \
        if ((search_str)[it] != map[i + s_i][j + s_j]) {                 \
            (found) = false;                                             \
            break;                                                       \
        }                                                                \
    }

    size_t matched_p1 = 0;

#define MATCH_BOTH_WAYS(found_cnt, part, max_x, max_y, incr_x, incr_y)    \
    {                                                                     \
        bool found = true;                                                \
        MATCH(found, search##part, max_x, max_y, incr_x, incr_y);         \
        found_cnt += found;                                               \
        if (!found) {                                                     \
            bool found = true;                                            \
            MATCH(found, rev_search##part, max_x, max_y, incr_x, incr_y); \
            found_cnt += found;                                           \
        }                                                                 \
    }

    for (size_t i = 0; i < row_count; i++) {
        // In a row
        for (size_t j = 0; j < col_count - (search_length_p1 - 1); j++) {
            MATCH_BOTH_WAYS(matched_p1, _p1, 1, search_length_p1, 0, 1);
        }
    }
    for (size_t i = 0; i < row_count - (search_length_p1 - 1); i++) {
        // In a column
        for (size_t j = 0; j < col_count; j++) {
            MATCH_BOTH_WAYS(matched_p1, _p1, search_length_p1, 1, 1, 0);
        }
    }
    for (size_t i = 0; i < row_count - (search_length_p1 - 1); i++) {
        // In a main diagonal
        for (size_t j = 0; j < col_count - (search_length_p1 - 1); j++) {
            MATCH_BOTH_WAYS(matched_p1, _p1, search_length_p1, search_length_p1, 1, 1);
        }
    }
    for (int32_t i = (search_length_p1 - 1); i < row_count; i++) {
        // In a side diagonal
        for (int32_t j = 0; j < col_count - (search_length_p1 - 1); j++) {
            MATCH_BOTH_WAYS(matched_p1, _p1, search_length_p1, search_length_p1, -1, 1);
        }
    }

    printf("PART 1: %zu\n", matched_p1);

    const char *search_p2 = "MAS";
    const char *rev_search_p2 = "SAM";
    const int32_t search_length_p2 = strlen(search_p2);

    size_t matched_p2 = 0;

    for (int32_t _i = 1; _i < row_count - 1; _i++) {
        for (int32_t _j = 1; _j < col_count - 1; _j++) {
            bool main_diag = false;
            bool side_diag = false;

            {
                int32_t i = _i - 1;
                int32_t j = _j - 1;

                MATCH_BOTH_WAYS(main_diag, _p2, search_length_p2, search_length_p2, 1, 1);
            }
            {
                int32_t i = _i + 1;
                int32_t j = _j - 1;

                MATCH_BOTH_WAYS(side_diag, _p2, search_length_p2, search_length_p2, -1, 1);
            }
            // printf("%d %d %d %d\n", _i, _j, main_diag, side_diag);
            matched_p2 += main_diag && side_diag;
        }
    }
    printf("PART 2: %zu\n", matched_p2);

    // Need to free the EOF line as well here
    for (size_t i = 0; i <= row_count; i++) {
        free(map[i]);
    }

    return EXIT_SUCCESS;
}
