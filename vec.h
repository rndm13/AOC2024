#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct VecI32 {
    size_t size;
    size_t capacity;
    int32_t *data;
} VecI32;
#define VEC_START_CAPACITY 4

#define assure_capacity_vec(vec, elem_count, elem_size)                     \
    {                                                                       \
        if ((vec).capacity == 0) {                                          \
            (vec).capacity = VEC_START_CAPACITY;                            \
            (vec).data = malloc((elem_size) * (vec).capacity);              \
            (vec).size = 0;                                                 \
        }                                                                   \
        int32_t __new_capacity = (vec).capacity;                            \
        while ((vec).size + (elem_count) > __new_capacity) {                \
            __new_capacity *= 2;                                            \
        }                                                                   \
        if (__new_capacity != (vec).capacity) {                             \
            (vec).data = realloc((vec).data, (elem_size) * __new_capacity); \
            (vec).capacity = __new_capacity;                                \
        }                                                                   \
    }

#define push_vec(vec, elems, elem_count)                                           \
    {                                                                              \
        assure_capacity_vec(vec, elem_count, sizeof(*(elems)));                    \
        memcpy((vec).data + (vec).size, (elems), (elem_count) * sizeof(*(elems))); \
        (vec).size += elem_count;                                                  \
    }

#define insert_vec(vec, idx, elems, elem_count)                                  \
    {                                                                            \
        assure_capacity_vec(vec, elem_count, sizeof(*(elems)));                  \
        (vec).size += (elem_count);                                              \
        for (int32_t __i = (vec).size - 1; __i >= (idx) + (elem_count); __i--) { \
            (vec).data[__i] = (vec).data[__i - (elem_count)];                    \
        }                                                                        \
        for (int32_t __i = 0; __i < (elem_count); __i++) {                       \
            (vec).data[__i + (idx)] = (elems)[__i];                              \
        }                                                                        \
    }
