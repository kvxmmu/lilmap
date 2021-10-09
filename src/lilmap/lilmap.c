//
// Created by nero on 10/9/21.
//

#include "lilmap.h"

// grow & rehash
void lilmap_grow(LILMap *map, size_t power) {
    size_t before = map->buckets_allocated;

    map->buckets_allocated <<= power;
    map->buckets = realloc(map->buckets,
                           sizeof(struct LILMapBucket)*map->buckets_allocated);

    void *last_allocated_ptr = &map->buckets[before];
    size_t last_allocated_count = map->buckets_allocated - before;
    memset(last_allocated_ptr, 0, last_allocated_count * sizeof(struct LILMapBucket));
}

void lilmap_set(LILMap *map, lilmap_int key,
                void *value) {
    if (lilmap_calc_load_factor(map, 1.f) >= map->load_factor) {
        lilmap_grow(map, 1u);
    }

    size_t index = lilmap_get_index(map, key);
    if (unlikely(index == map->buckets_allocated)) {
        --index;
    }

    struct LILMapBucket *bucket = &map->buckets[index];

    if (unlikely(bucket->is_used)) {
        struct LILMapBucket *new_bucket = malloc(sizeof(struct LILMapBucket));
        new_bucket->is_used = 1;
        new_bucket->data = value;
        new_bucket->key = key;

        lilmap_add_bucket(bucket, new_bucket);
        return;
    }

    ++map->buckets_used;
    bucket->key = key;
    bucket->is_used = 1;
    bucket->next = NULL;
    bucket->data = value;
}

bool lilmap_lookup(LILMap *map, lilmap_int key,
                   void **value) {
    size_t index = lilmap_get_index(map, key);
    struct LILMapBucket *bucket = &map->buckets[index];

    if (likely(bucket->is_used)) {

        struct LILMapBucket *cur_bucket = bucket;
        do {
            if (cur_bucket->key == key) {
                *value = cur_bucket->data;
                return true;
            }

            cur_bucket = cur_bucket->next;
        } while (cur_bucket != NULL);

    }

    return false;
}

void lilmap_init(LILMap *map, lilmap_float load_factor) {
    map->load_factor = load_factor;
    map->buckets = calloc(sizeof(struct LILMapBucket),
                          DEFAULT_BUCKETS_COUNT);

    map->buckets_used = 0;
    map->buckets_allocated = DEFAULT_BUCKETS_COUNT;
}

void lilmap_free(LILMap *map) {
    for (size_t pos = 0; pos < map->buckets_allocated; ++pos) {
        struct LILMapBucket *bucket = &map->buckets[pos];
        if (!bucket->is_used) continue;

        struct LILMapBucket *victim = bucket->next;

        while (victim != NULL) {
            struct LILMapBucket *new_victim = victim->next;
            free(victim);

            victim = new_victim;
        }
    }

    free(map->buckets);
    map->buckets_allocated = 0;
    map->buckets_used = 0;
    map->load_factor = 0.f;
}
