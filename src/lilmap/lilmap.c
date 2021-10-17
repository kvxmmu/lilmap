//
// Created by nero on 13.10.2021.
//

#include "lilmap.h"

void lilmap_init_custom(LilMap *map, size_t capacity,
                        lilmap_float load_factor) {
    map->buckets = calloc(sizeof(struct LilMapBucket),
            capacity);
    map->buckets_allocated = capacity;
    map->load_factor = load_factor;
    map->buckets_used = 0;
}

void lilmap_set(LilMap *map, lilmap_int key,
                void *value) {
    if (lilmap_load_factor(map, 1u) >= map->load_factor) {
        lilmap_grow(map, key, value);

        return;
    }

    size_t index = lilmap_index(map, key);

    while (index < map->buckets_allocated) {
        struct LilMapBucket *bucket = &map->buckets[index++];
        if (!lilmap_is_occupied(bucket) || (bucket->key == key)) {
            bucket->value = value;
            bucket->key = key;
            ++map->buckets_used;

            return;
        }
    }

    lilmap_grow(map, key,
                value);
}

void* lilmap_lookup(LilMap *map, lilmap_int key) {
    size_t index = lilmap_index(map, key);

    for (size_t pos = index; ((pos - index) < LM_MAX_PROBES) && (pos < map->buckets_allocated); ++pos) {
        struct LilMapBucket *bucket = &map->buckets[pos];
        if (lilmap_is_occupied(bucket) && (bucket->key == key)) { return bucket->value; }
    }

    return NULL;
}

void lilmap_erase(LilMap *map, lilmap_int key) {
    size_t index = lilmap_index(map, key);
    for (size_t pos = index; ((pos - index) < LM_MAX_PROBES) && (pos < map->buckets_allocated); ++pos) {
        struct LilMapBucket *bucket = &map->buckets[pos];
        if (bucket->key == key) {
            bucket->value = LM_TOMBSTONE;
            --map->buckets_used;

            break;
        }
    }
}

void lilmap_grow(LilMap *map, lilmap_int key,
                 void *value) {
    LilMap new_map;
    lilmap_init_custom(&new_map, map->buckets_allocated << 1u,
                       map->load_factor);

    for (size_t pos = 0; pos < map->buckets_allocated; ++pos) {
        struct LilMapBucket *bucket = &map->buckets[pos];
        if (!lilmap_is_occupied(bucket)) { continue; }

        lilmap_set(&new_map, bucket->key,
                   bucket->value);
    }

    lilmap_set(&new_map, key, value);

    lilmap_free(map);
    *map = new_map;
}
