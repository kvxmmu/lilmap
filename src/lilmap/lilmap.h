//
// Created by nero on 12.10.2021.
//

#ifndef LILMAP_LILMAP_H
#define LILMAP_LILMAP_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define LILMAP_DEAD ((void*)0x0)
#define LILMAP_MAX_ITER (64)
#define LILMAP_LOAD_FACTOR (0.75f)

typedef size_t lilmap_int;
typedef float  lilmap_float;

struct LILMapBucket;
struct LILMapBucket {
    lilmap_int key;
    void *value;
};

typedef struct {
    struct LILMapBucket *buckets;

    size_t buckets_allocated;
    size_t buckets_count;
} Buckets;

typedef struct LILMap {
    Buckets buckets;

    lilmap_float load_factor;
} LILMap;

//

void lilmap_init(LILMap *map, size_t capacity,
                 lilmap_float load_factor);
void lilmap_resize(LILMap *map, size_t new_capacity);

void lilmap_set(LILMap *map, lilmap_int key,
                void *data);
void *lilmap_get(LILMap *map, lilmap_int key);


static inline
size_t lilmap_to_limited_space(size_t buckets_allocated,
                               size_t hash) {
    return (buckets_allocated % hash) - (hash == buckets_allocated);
}

inline
size_t lilmap_size(LILMap *map) {
    return map->buckets.buckets_count;
}

inline
size_t lilmap_empty(LILMap *map) {
    return lilmap_size(map) == 0;
}

inline
lilmap_float lilmap_calculate_load_factor(Buckets *buckets) {
    return ((lilmap_float)buckets->buckets_count) / ((lilmap_float)buckets->buckets_allocated);
}

static
size_t lilmap_mix(lilmap_int key) {
    key ^= key << 13u;
    key ^= key >> 12u;

    return key;
}

//

#endif //LILMAP_LILMAP_H
