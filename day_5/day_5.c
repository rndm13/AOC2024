#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../vec.h"

#define MAX_PAGE 100
#define MAX_PAGE_LIST_COUNT 256

int main(int32_t argc, char *argv[]) {
    // [any number][cannot be after that number]
    vec_i32 rules[MAX_PAGE];
    vec_i32 page_lists[MAX_PAGE_LIST_COUNT];

    memset(rules, 0, sizeof(rules));
    memset(page_lists, 0, sizeof(page_lists));

    FILE *file = stdin;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }

    char *line = NULL;
    size_t line_size = 0;
    while (getline(&line, &line_size, file) > 0) {
        if (strlen(line) <= 1) { // Only new line
            break;
        }

        int32_t first = (line[0] - '0') * 10 + (line[1] - '0');
        int32_t second = (line[3] - '0') * 10 + (line[4] - '0');

        push_vec(rules[second], &first, 1);
    }

    size_t page_list_count = 0;

    while (getline(&line, &line_size, file) > 0) {
        for (size_t it = 0; line[it * 3] != '\0'; it++) {
            int32_t num = (line[it * 3 + 0] - '0') * 10 + (line[it * 3 + 1] - '0');

            push_vec(page_lists[page_list_count], &num, 1);
        }

        page_list_count++;
    }

    free(line);

    size_t p1_result = 0;
    for (int32_t i = 0; i < page_list_count; i++) {
        // Iterate over lists
        int32_t middle = page_lists[i].data[page_lists[i].size / 2];

        bool right_order = true;
        for (int32_t j = 0; j < page_lists[i].size && right_order; j++) {
            // Iterate over all numbers in a list
            int32_t j_num = page_lists[i].data[j];
            if (rules[j_num].data == NULL) {
                continue;
            }

            for (int32_t k = j + 1; k < page_lists[i].size && right_order; k++) {
                int32_t k_num = page_lists[i].data[k];
                // Iterate over all next numbers

                for (int32_t r_idx = 0; r_idx < rules[j_num].size; r_idx++) {
                    // Iterate over all rules for j number
                    if (rules[j_num].data[r_idx] == k_num) {
                        right_order = false;
                        break;
                    }
                }
            }
        }

        p1_result += right_order * middle;
    }

    printf("PART 1: %zu\n", p1_result);

    size_t p2_result = 0;
    for (int32_t i = 0; i < page_list_count; i++) {
        // Iterate over lists

        bool swapped = false;
        for (int32_t j = 0; j < page_lists[i].size; j++) {
            // Iterate over all numbers in a list
            int32_t j_num = page_lists[i].data[j];
            if (rules[j_num].data == NULL) {
                continue;
            }

            for (int32_t k = j + 1; k < page_lists[i].size; k++) {
                // Iterate over all next numbers
                int32_t k_num = page_lists[i].data[k];

                for (int32_t r_idx = 0; r_idx < rules[j_num].size; r_idx++) {
                    // Iterate over all rules for j number
                    if (rules[j_num].data[r_idx] == k_num) {
                        // Swap them around
                        swapped = true;
                        page_lists[i].data[j] = k_num;
                        page_lists[i].data[k] = j_num;
                        k_num = j_num;
                        j_num = page_lists[i].data[j];
                        break;
                    }
                }
            }
        }

        int32_t middle = page_lists[i].data[page_lists[i].size / 2];

        p2_result += swapped * middle;
    }

    printf("PART 2: %zu\n", p2_result);

    for (int32_t i = 0; i < MAX_PAGE; i++) {
        if (rules[i].data != NULL) {
            free(rules[i].data);
        }
    }

    for (int32_t i = 0; i < page_list_count; i++) {
        free(page_lists[i].data);
    }

    if (file != stdin) {
        fclose(file);
    }

    return EXIT_SUCCESS;
}
