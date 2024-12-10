#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../hash_table.h"

#define MAX_LIST_SIZE 1024

int compare_int32(const void* a, const void* b) {
    return (*(int32_t*)a) - (*(int32_t*)b);
}

int main(void) { 
    int32_t list_a[MAX_LIST_SIZE];
    int32_t list_b[MAX_LIST_SIZE];

    int32_t list_size = 0;
    while (list_size < MAX_LIST_SIZE) {
        int32_t first, second;
        if (scanf("%d %d", &first, &second) <= 0) {
            break;
        }

        list_a[list_size] = first;
        list_b[list_size] = second;

        list_size++;
    }

    qsort(list_a, list_size, sizeof(list_a[0]), compare_int32);
    qsort(list_b, list_size, sizeof(list_b[0]), compare_int32);

    int32_t diff_sum = 0;
    for (int i = 0; i < list_size; i++) {
        diff_sum += abs(list_a[i] - list_b[i]);
    }
    
    printf("PART 1: %d\n", diff_sum);

    HashTable count_ht_b;
    hash_table_init(&count_ht_b);

    {
        // Fill hash table with element appearance count's
        int32_t cur_value = list_b[0];
        int32_t cur_count = 1;
        for (int i = 1; i < list_size; i++) {
            if (cur_value == list_b[i]) {
                cur_count += 1;
                continue;
            }

            // cur_value != list_b[i]

            hash_table_set(&count_ht_b, cur_value, cur_count);
            
            cur_value = list_b[i];
            cur_count = 1;
        }

        hash_table_set(&count_ht_b, cur_value, cur_count);
    }

    int32_t count_mul_sum = 0;
    for (int i = 0; i < list_size; i++) {
        int32_t* count_ptr = hash_table_get(&count_ht_b, list_a[i]);
        int32_t count = count_ptr ? *count_ptr : 0;

        count_mul_sum += count * list_a[i];
    }

    printf("PART 2: %d\n", count_mul_sum);

    hash_table_deinit(&count_ht_b);
    return EXIT_SUCCESS; 
}
