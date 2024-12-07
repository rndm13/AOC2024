#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t suffix_parse(const char *str, const char *suffix) {
    int32_t i = 0;

    for (; str[i] != '\0' && suffix[i] != '\0'; i++) {
        if (str[i] != (suffix)[i]) {
            return i;
        }
    }

    return i;
}

// positive - how many characters to advance
// 0 - end
int32_t parse_mul(const char *str, int32_t *first, int32_t *second) {
    *first = 0;
    *second = 0;

    if (str[0] == '\0') {
        return 0;
    }

    const char *prefix = "mul(";
    const char *postfix = ")";
    const char *separator = ",";

    int32_t current = 0;

#define SUFFIX_PARSE(suffix)                             \
    {                                                    \
        int32_t i = suffix_parse(str + current, suffix); \
        if (i < strlen(suffix)) {                        \
            *first = 0;                                  \
            *second = 0;                                 \
            return current + (i == 0) ? 1 : i;           \
        }                                                \
        current += i;                                    \
    }

#define NUMBER_PARSE(number)                     \
    {                                            \
        int32_t i = 0;                           \
        for (; isdigit(str[current + i]); i++) { \
            (number) *= 10;                      \
            (number) += str[current + i] - '0';  \
        }                                        \
        if (i <= 0) {                            \
            *first = 0;                          \
            *second = 0;                         \
            return current + i + 1;              \
        }                                        \
        current += i;                            \
    }

    SUFFIX_PARSE(prefix);
    NUMBER_PARSE(*first);
    SUFFIX_PARSE(separator);
    NUMBER_PARSE(*second);
    SUFFIX_PARSE(postfix);

    return current;
}

int32_t p1_parse_line(const char *line, size_t line_size) {
    int32_t first = 0;
    int32_t second = 0;

    int32_t i = parse_mul(line, &first, &second);
    int32_t sum = 0;
    for (; line[i] != '\0'; i += parse_mul(line + i, &first, &second)) {
        sum += first * second;
    }

    return sum;
}

int32_t p2_parse_line(const char *line, size_t line_size, bool *enable) {
    int32_t first = 0;
    int32_t second = 0;

    const char *dont_enable = "don't()";
    const char *do_enable = "do()";

    int32_t i = 0;

    if (suffix_parse(line, dont_enable) == strlen(dont_enable)) {
        *enable = false;
    } else if (suffix_parse(line, do_enable) == strlen(do_enable)) {
        *enable = true;
    }

    i = parse_mul(line, &first, &second);
    int32_t sum = 0;
    for (; line[i] != '\0'; i += parse_mul(line + i, &first, &second)) {
        // printf("%d\n", i);

        if (*enable) {
            sum += first * second;
        }

        if (suffix_parse(line + i, dont_enable) == strlen(dont_enable)) {
            // printf("DISABLE\n");
            *enable = false;
        } else if (suffix_parse(line + i, do_enable) == strlen(do_enable)) {
            // printf("ENABLE\n");
            *enable = true;
        }
    }

    return sum;
}

int main(void) {
    char *line = malloc(sizeof(char) * 1024);
    size_t line_size = 0;
    int32_t sum_p1 = 0;
    int32_t sum_p2 = 0;
    bool enable_p2 = true;
    while (getline(&line, &line_size, stdin) > 0) {
        sum_p1 += p1_parse_line(line, line_size);
        sum_p2 += p2_parse_line(line, line_size, &enable_p2);
    }
    free(line);

    printf("PART 1: %d\n", sum_p1);
    printf("PART 2: %d\n", sum_p2);

    return EXIT_SUCCESS;
}
