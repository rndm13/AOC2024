#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct vec_i32 {
    size_t size;
    size_t capacity;
    int32_t *data;
} vec_i32;
#define VEC_START_CAPACITY 4

#define push_vec(vec, elems, elem_count)                                         \
    {                                                                            \
        if ((vec).capacity == 0) {                                               \
            (vec).capacity = VEC_START_CAPACITY;                                 \
            (vec).data = malloc(sizeof(*(elems)) * (vec).capacity);              \
            (vec).size = 0;                                                      \
        }                                                                        \
        int32_t __new_capacity = (vec).capacity;                                 \
        while ((vec).size + (elem_count) > __new_capacity) {                     \
            __new_capacity *= 2;                                                 \
        }                                                                        \
        if (__new_capacity != (vec).capacity) {                                  \
            (vec).data = realloc((vec).data, sizeof(*(elems)) * __new_capacity); \
            (vec).capacity = __new_capacity;                                     \
        }                                                                        \
        for (int32_t __i = 0; __i < (elem_count); __i++) {                       \
            (vec).data[(vec).size] = (elems)[__i];                               \
            (vec).size++;                                                        \
        }                                                                        \
    }
