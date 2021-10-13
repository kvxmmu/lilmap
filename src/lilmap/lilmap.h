//
// Created by nero on 13.10.2021.
//

#ifndef LILMAP2_LILMAP_H
#define LILMAP2_LILMAP_H

#include <string.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>


#define LM_DEFAULT_CAPACITY 1
#define LM_DEFAULT_LOAD_FACTOR 0.75f

#define LM_MAX_PROBES 64u

typedef float lilmap_float;
typedef size_t lilmap_int;

struct LilMapBucket {
    lilmap_int key;
    void *value;
};

typedef struct {
    struct LilMapBucket *buckets;

    size_t buckets_allocated;
    size_t buckets_used;

    lilmap_float load_factor;
} LilMap;


void lilmap_init_custom(LilMap *map, size_t capacity,
                        lilmap_float load_factor);

void lilmap_set(LilMap *map, lilmap_int key,
                void *value);
void* lilmap_lookup(LilMap *map, lilmap_int key);

void lilmap_grow(LilMap *map, lilmap_int key,
                 void *value);

inline static
size_t lilmap_index(LilMap *map, lilmap_int key) {
    size_t index = key % map->buckets_allocated;

    return index - (index == map->buckets_allocated);
}

inline static
lilmap_float lilmap_load_factor(LilMap *map, size_t additional) {
    return ((lilmap_float)(map->buckets_used + additional)) / ((lilmap_float)map->buckets_allocated);
}

inline static
void lilmap_init(LilMap *map) {
    lilmap_init_custom(map, LM_DEFAULT_CAPACITY,
                       LM_DEFAULT_LOAD_FACTOR);
}

inline static
void lilmap_free(LilMap *map) {
    free(map->buckets);
}


#endif //LILMAP2_LILMAP_H
