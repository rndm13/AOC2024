#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define HASH_TABLE_BUCKET_COUNT 1024
#define HASH_TABLE_KEY_NULL_GUARD_VALUE 0x80000000

typedef struct hash_table_node {
    int32_t key;
    int32_t value;
    struct hash_table_node *next;
} hash_table_node;

void hash_table_node_init(hash_table_node *node) {
    node->key = HASH_TABLE_KEY_NULL_GUARD_VALUE;
    node->value = 0;
    node->next = NULL;
}

typedef struct hash_table {
    hash_table_node *buckets;
} hash_table;

void hash_table_init(hash_table *ht) {
    ht->buckets = (hash_table_node *)malloc(sizeof(hash_table_node) * HASH_TABLE_BUCKET_COUNT);

    for (int32_t i = 0; i < HASH_TABLE_BUCKET_COUNT; i++) {
        hash_table_node_init(&ht->buckets[i]);
    }
}

void hash_table_deinit_list(hash_table_node *node) {
    if (node->next) {
        hash_table_deinit_list(node->next);
    }

    free(node);
}

void hash_table_deinit(hash_table *ht) {
    for (int32_t i = 0; i < HASH_TABLE_BUCKET_COUNT; i++) {
        if (ht->buckets[i].next) {
            hash_table_deinit_list(ht->buckets[i].next);
        }
    }

    free(ht->buckets);
}

int32_t hash_function(int32_t i) {
    return i;
}

void hash_table_set(hash_table *ht, int32_t key, int32_t value) {
    int32_t bucket_idx = hash_function(key) % HASH_TABLE_BUCKET_COUNT;

    hash_table_node *node = &ht->buckets[bucket_idx];

    while (true) {
        if (node->key == key) {
            node->value = value;
            return;
        }

        if (node->key == HASH_TABLE_KEY_NULL_GUARD_VALUE) {
            node->key = key;
            node->value = value;
            return;
        }

        if (node->next == NULL) {
            node->next = (hash_table_node *)malloc(sizeof(hash_table_node));
            hash_table_node_init(node->next);
        }
        node = node->next;
    }
}

int32_t *hash_table_get_or(hash_table *ht, int32_t key, int32_t *otherwise) {
    int32_t bucket_idx = hash_function(key) % HASH_TABLE_BUCKET_COUNT;

    hash_table_node *node = &ht->buckets[bucket_idx];

    while (true) {
        if (node->key == key) {
            return &node->value;
        }

        if (!node->next) {
            return otherwise;
        }
        node = node->next;
    }

    return otherwise;
}

int32_t *hash_table_get(hash_table *ht, int32_t key) {
    return hash_table_get_or(ht, key, NULL);
}

void hash_table_clear(hash_table *ht) {
    for (int32_t i = 0; i < HASH_TABLE_BUCKET_COUNT; i++) {
        ht->buckets[i].key = HASH_TABLE_KEY_NULL_GUARD_VALUE;
        ht->buckets[i].value = 0;
        if (ht->buckets[i].next) {
            hash_table_deinit_list(ht->buckets[i].next);
        }
        ht->buckets[i].next = NULL;
    }
}

#endif
