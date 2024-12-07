#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBERS_COUNT 12
#define MAX_EQUATION_COUNT 1024

typedef struct Equation {
    uint8_t number_count;
    uint64_t numbers[MAX_NUMBERS_COUNT];
    uint64_t result;
} Equation;

#define EQ_MATRIX_ROW_COUNT (MAX_NUMBERS_COUNT - 1)

uint64_t solve_p1(Equation *equations, size_t equation_count) {
    uint64_t **eq_matrix = malloc(EQ_MATRIX_ROW_COUNT * sizeof(uint64_t *));
    uint32_t pow_2 = 2;
    for (uint64_t i = 0; i < EQ_MATRIX_ROW_COUNT; i++) {
        eq_matrix[i] = malloc(pow_2 * sizeof(uint64_t));
        pow_2 *= 2;
    }

    size_t p1_res = 0;
    for (int32_t e_idx = 0; e_idx < equation_count; e_idx++) {
        Equation *eq = &equations[e_idx];

        eq_matrix[0][0] = eq->numbers[0] + eq->numbers[1];
        eq_matrix[0][1] = eq->numbers[0] * eq->numbers[1];

        size_t pow_2 = 2;
        for (int32_t i = 1; i < eq->number_count - 1; i++) {
            for (int32_t j = 0; j < pow_2; j++) {
                eq_matrix[i][j * 2 + 0] = eq_matrix[i - 1][j] + eq->numbers[i + 1];
                eq_matrix[i][j * 2 + 1] = eq_matrix[i - 1][j] * eq->numbers[i + 1];
            }
            pow_2 *= 2;
        }

        for (int32_t j = 0; j < pow_2; j++) {
            if (eq_matrix[eq->number_count - 2][j] == eq->result) {
                p1_res += eq->result;
                break;
            }
        }

        // {
        //     size_t pow_2 = 1;
        //     for (int32_t i = 0; i < eq->number_count - 1; i++) {
        //         for (int32_t j = 0; j < pow_2; j++) {
        //             printf("%d %d ", eq_matrix[i][j * 2 + 0], eq_matrix[i][j * 2 + 1]);
        //         }
        //         printf("\n");
        //         pow_2 *= 2;
        //     }
        //     printf("\n\n");
        // }
    }

    for (int32_t i = 0; i < EQ_MATRIX_ROW_COUNT; i++) {
        free(eq_matrix[i]);
    }
    free(eq_matrix);

    return p1_res;
}

uint64_t concat_nums(uint64_t a, uint64_t b) {
    uint64_t b_cpy = b;
    uint64_t b_mul = 1;
    while (b_cpy > 0) {
        b_cpy /= 10;
        b_mul *= 10;
    }

    return a * b_mul + b;
}

uint64_t solve_p2(Equation *equations, size_t equation_count) {
    uint64_t **eq_matrix = malloc(EQ_MATRIX_ROW_COUNT * sizeof(uint64_t *));
    uint32_t pow_3 = 3;
    for (uint64_t i = 0; i < EQ_MATRIX_ROW_COUNT; i++) {
        eq_matrix[i] = malloc(pow_3 * sizeof(uint64_t));
        pow_3 *= 3;
    }

    size_t p2_res = 0;
    for (int32_t e_idx = 0; e_idx < equation_count; e_idx++) {
        Equation *eq = &equations[e_idx];

        eq_matrix[0][0] = eq->numbers[0] + eq->numbers[1];
        eq_matrix[0][1] = eq->numbers[0] * eq->numbers[1];
        eq_matrix[0][2] = concat_nums(eq->numbers[0], eq->numbers[1]);

        size_t pow_3 = 3;
        for (int32_t i = 1; i < eq->number_count - 1; i++) {
            for (int32_t j = 0; j < pow_3; j++) {
                eq_matrix[i][j * 3 + 0] = eq_matrix[i - 1][j] + eq->numbers[i + 1];
                eq_matrix[i][j * 3 + 1] = eq_matrix[i - 1][j] * eq->numbers[i + 1];
                eq_matrix[i][j * 3 + 2] = concat_nums(eq_matrix[i - 1][j], eq->numbers[i + 1]);
            }
            pow_3 *= 3;
        }

        for (int32_t j = 0; j < pow_3; j++) {
            if (eq_matrix[eq->number_count - 2][j] == eq->result) {
                p2_res += eq->result;
                break;
            }
        }

        // {
        //     size_t pow_3 = 1;
        //     for (int32_t i = 0; i < eq->number_count - 1; i++) {
        //         for (int32_t j = 0; j < pow_3; j++) {
        //             printf("%zu %zu %zu ", eq_matrix[i][j * 3 + 0], eq_matrix[i][j * 3 + 1], eq_matrix[i][j * 3 + 2]);
        //         }
        //         printf("\n");
        //         pow_3 *= 3;
        //     }
        //     printf("\n\n");
        // }
    }

    for (int32_t i = 0; i < EQ_MATRIX_ROW_COUNT; i++) {
        free(eq_matrix[i]);
    }
    free(eq_matrix);

    return p2_res;
}

int main(int32_t argc, char *argv[]) {
    FILE *file = stdin;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }

    size_t equation_count = 0;
    Equation *equations = malloc(MAX_EQUATION_COUNT * sizeof(Equation));
    memset(equations, 0, MAX_EQUATION_COUNT * sizeof(Equation));

    char *line = NULL;
    size_t line_size = 0;
    while (getline(&line, &line_size, file) > 0) {
        Equation *eq = &equations[equation_count];

        int32_t idx = 0;
        for (; line[idx] != '\0' && line[idx] != ':'; idx++) {
            eq->result *= 10;
            eq->result += line[idx] - '0';
        }

        idx += strlen(": "); // Skip over

        while (line[idx] != '\n' && line[idx] != '\0') {
            uint64_t *num = &eq->numbers[eq->number_count];

            for (; line[idx] != '\0' && line[idx] != ' ' && line[idx] != '\n'; idx++) {
                *num *= 10;
                *num += line[idx] - '0';
            }

            eq->number_count += 1;
            idx += strlen(" ");
        }

        equation_count++;
    }

    free(line);

    printf("PART 1: %zu\n", solve_p1(equations, equation_count));
    printf("PART 2: %zu\n", solve_p2(equations, equation_count));

    free(equations);

    if (file != stdin) {
        fclose(file);
    }
    return EXIT_SUCCESS;
}
