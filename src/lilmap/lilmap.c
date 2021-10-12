//
// Created by nero on 12.10.2021.
//

#include "lilmap.h"

void lilmap_init(LILMap *map, size_t capacity,
                 lilmap_float load_factor) {
    map->buckets.buckets = calloc(sizeof(struct LILMapBucket),
            capacity);
    map->buckets.buckets_allocated = capacity;
    map->buckets.buckets_count = 0;

    map->load_factor = load_factor;
}

void lilmap_set(LILMap *map, lilmap_int key,
                void *data) {
    size_t index = lilmap_to_limited_space(map->buckets.buckets_allocated,
                                           key);
    while (index < (map->buckets.buckets_allocated - 1u)) {
        struct LILMapBucket *bucket = &map->buckets.buckets[index++];
        if (bucket->value == LILMAP_DEAD) {
            bucket->key = key;
            bucket->value = data;

            return;
        }
    }
}

void lilmap_resize(LILMap *map, size_t new_capacity) {
    LILMap new_map;
    new_map.buckets.buckets_count = 0;
    new_map.buckets.buckets_allocated = new_capacity;
    new_map.buckets.buckets = calloc(sizeof(struct LILMapBucket),
                                     new_capacity);


}

