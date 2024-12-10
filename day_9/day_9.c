#include "stdio.h"
#include "stdlib.h"

#include "../vec.h"
#include <stdint.h>

#define DISK_ID_EMPTY_GUARD -1
#define MIN(A, B) \
    ((A) + ((B) - (A)) * ((B) < (A)))

typedef struct DiskBlock {
    int32_t id;
    int32_t size;
} DiskBlock;

typedef struct DiskBlockVec {
    size_t size;
    size_t capacity;
    DiskBlock *data;
} DiskBlockVec;

void fragment(DiskBlockVec* db_vec) {
    size_t free_idx = 0;
    for (int32_t i = db_vec->size - 1; i > 0 && free_idx < i; i--) {
        if (db_vec->data[i].id == DISK_ID_EMPTY_GUARD) {
            continue;
        }

        do {
            while (db_vec->data[free_idx].id != DISK_ID_EMPTY_GUARD && 
                   db_vec->data[free_idx].size > 0 && free_idx < i) {
                free_idx += 1;
            }

            if (free_idx >= i) {
                return;
            }

            int32_t to_move = MIN(db_vec->data[free_idx].size, db_vec->data[i].size);

            db_vec->data[free_idx].size -= to_move;
            db_vec->data[i].size -= to_move;
            if (db_vec->data[free_idx].size <= 0) {
                db_vec->data[free_idx].size = to_move;
                db_vec->data[free_idx].id = db_vec->data[i].id;
            } else {
                DiskBlock db = {
                    .id = db_vec->data[i].id,
                    .size = to_move,
                };
                insert_vec(*db_vec, free_idx, &db, 1);
            }

            if (db_vec->data[i].size <= 0) {
                db_vec->data[i].size = 0;
                db_vec->data[i].id = DISK_ID_EMPTY_GUARD;
            }
        } while (db_vec->data[i].size > 0);

        db_vec->data[i].id = DISK_ID_EMPTY_GUARD;
        db_vec->data[i].size = 0;
    }

    return;
}

uint64_t checksum(DiskBlockVec* db_vec) {
    size_t idx = 0;
    uint64_t result = 0;
    for (int32_t i = 0; i < db_vec->size; i++) {
        for (int32_t j = 0; j < db_vec->data[i].size; j++) {
            result += idx * db_vec->data[i].id * (db_vec->data[i].id != DISK_ID_EMPTY_GUARD);
            idx++;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    FILE* file = stdin;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }

    DiskBlockVec db_vec;
    
    char* line = NULL;
    size_t line_size = 0;
    getline(&line, &line_size, file);

    int32_t id = 0;
    for (int32_t i = 0; line[i] != '\n' && line[i] != '\0'; i++) {
        DiskBlock db = {
            .id = i % 2 == 0 ? id++ : DISK_ID_EMPTY_GUARD,
            .size = line[i] - '0',
        };

        if (db.id == DISK_ID_EMPTY_GUARD && db.size <= 0) {
            continue;
        }

        push_vec(db_vec, &db, 1);
    }
    free(line);

    fragment(&db_vec);

    printf("PART 1: %zu\n", checksum(&db_vec));

    free(db_vec.data);
    if (argc > 1) {
        fclose(file);
    }
    return EXIT_SUCCESS;
}
