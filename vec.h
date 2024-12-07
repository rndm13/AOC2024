#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct vec_i32 {
    size_t size;
    size_t capacity;
    int32_t *data;
} vec_i32;
#define VEC_START_CAPACITY 4

#define push_vec(vec, elems, elem_count)                                       \
    {                                                                          \
        if ((vec).capacity == 0) {                                             \
            (vec).capacity = VEC_START_CAPACITY;                               \
            (vec).data = malloc(sizeof(*(elems)) * (vec).capacity);            \
            (vec).size = 0;                                                    \
        }                                                                      \
        int32_t new_capacity = (vec).capacity;                                 \
        while ((vec).size + (elem_count) > new_capacity) {                     \
            new_capacity *= 2;                                                 \
        }                                                                      \
        if (new_capacity != (vec).capacity) {                                  \
            (vec).data = realloc((vec).data, sizeof(*(elems)) * new_capacity); \
            (vec).capacity = new_capacity;                                     \
        }                                                                      \
        for (int32_t i = 0; i < (elem_count); i++) {                           \
            (vec).data[(vec).size] = (elems)[i];                               \
            (vec).size++;                                                      \
        }                                                                      \
    }

