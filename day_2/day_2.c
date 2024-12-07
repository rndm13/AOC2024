#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_LINES 1024
#define MAX_COLUMNS 10
#define END_LINE_GUARD -1
int main(void) { 
    int32_t** numbers = malloc(sizeof(int32_t*) * MAX_LINES);

    for (int32_t i = 0; i < MAX_LINES; i++) {
        numbers[i] = malloc(sizeof(int32_t) * MAX_COLUMNS);
        for (int32_t j = 0; j < MAX_COLUMNS; j++) {
            numbers[i][j] = END_LINE_GUARD;
        }
    }

    int64_t line_count = 0;

    char* line = malloc(sizeof(char) * 128);
    uint64_t line_size = 0;
    while (true) {
        // Parse input
        int32_t res = getline(&line, &line_size, stdin);
        if (res < 0) {
            break;
        }

        int64_t col_count = 0;
        char* current = line;
        while (true) {
            // Parse a line
            int32_t number = 0;

            int parse = sscanf(current, "%d", &number);
            if (parse < 0) {
                break;
            }

            numbers[line_count][col_count] = number;
            col_count++;

            do {
                // Go to next number
                current++;
            } while (*current != ' ' && *current != '\n' && *current != '\0');
        }

        line_count++;
    }

    free(line);

    int32_t safe_count_p1 = 0;

    for (int32_t i = 0; i < line_count; i++) {
        bool safe = true;
        for (int32_t j = 1; j < MAX_COLUMNS; j++) {
            if (numbers[i][j + 1] == END_LINE_GUARD) {
                break;
            }
            
            int32_t diff_prev = numbers[i][j - 1] - numbers[i][j];
            int32_t diff_next = numbers[i][j] - numbers[i][j + 1];
            if ((diff_prev < 0) != (diff_next < 0)) {
                safe = false;
                break;
            }

            if (abs(diff_prev) < 1 || abs(diff_prev) > 3) {
                safe = false;
                break;
            }

            if (abs(diff_next) < 1 || abs(diff_next) > 3) {
                safe = false;
                break;
            }
        }

        safe_count_p1 += safe;
    }

    printf("PART 1: %d\n", safe_count_p1);

    int32_t safe_count_p2 = 0;
    for (int32_t i = 0; i < line_count; i++) {
        for (int32_t removed_column = 0; removed_column < MAX_COLUMNS; removed_column++) {
            if (numbers[i][removed_column] == END_LINE_GUARD) {
                // Reached the end
                break;
            }

            int32_t current_line[MAX_COLUMNS] = {END_LINE_GUARD};
            int32_t current_col = 0;
            for (int32_t j = 0; j < MAX_COLUMNS; j++) {
                if (j == removed_column) {
                    continue;
                }
                current_line[current_col++] = numbers[i][j];
            }

            bool safe = true;
            for (int32_t j = 1; j < MAX_COLUMNS; j++) {
                if (current_line[j + 1] == END_LINE_GUARD) {
                    break;
                }
                
                int32_t diff_prev = current_line[j - 1] - current_line[j];
                int32_t diff_next = current_line[j] - current_line[j + 1];
                if ((diff_prev < 0) != (diff_next < 0)) {
                    safe = false;
                    break;
                }

                if (abs(diff_prev) < 1 || abs(diff_prev) > 3) {
                    safe = false;
                    break;
                }

                if (abs(diff_next) < 1 || abs(diff_next) > 3) {
                    safe = false;
                    break;
                }
            }

            if (safe) {
                safe_count_p2++;
                break;
            }
        }
    }

    printf("PART 2: %d\n", safe_count_p2);

    for (int32_t i = 0; i < MAX_LINES; i++) {
        free(numbers[i]);
    }

    free(numbers);

    return EXIT_SUCCESS;
}
