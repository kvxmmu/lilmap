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

#ifdef GNU_EXTENSIONS
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (!!(x))
#endif

#define LM_TOMBSTONE ((void *)0xDEAD)
#define LM_DEFAULT_CAPACITY 64
#define LM_DEFAULT_LOAD_FACTOR 0.75f

#define LM_MAX_PROBES 16u
#define LM_USE_BIT_HASH

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

/* Initialize lilmap with custom settings */
void lilmap_init_custom(LilMap *map, size_t capacity,
                        lilmap_float load_factor);

/* Insert key-value pair to map */
void lilmap_set(LilMap *map, lilmap_int key,
                void *value);

/* Lookup for key in map */
void* lilmap_lookup(LilMap *map, lilmap_int key);

/* Erase key from map */
void lilmap_erase(LilMap *map, lilmap_int key);

/* Grow hash map by doubling its capacity */
void lilmap_grow(LilMap *map, lilmap_int key,
                 void *value);

/* Get bucket index from map */
inline static
size_t lilmap_index(LilMap *map, lilmap_int key) {
#ifndef LM_USE_BIT_HASH
    size_t index = key % map->buckets_allocated;
#else
    // mix bits to make result more random
    key ^= key >> 11u;
    key ^= key << 15u;

    size_t index = key & (map->buckets_allocated - 1);
#endif
    return index - (index == map->buckets_allocated);
}

/* Calculate load factor; WARNING: floating point exception may be thrown */
inline static
lilmap_float lilmap_load_factor(LilMap *map, size_t additional) {
    return ((lilmap_float)(map->buckets_used + additional)) / ((lilmap_float)map->buckets_allocated);
}

/* Initialize lilmap with default settings */
inline static
void lilmap_init(LilMap *map) {
    lilmap_init_custom(map, LM_DEFAULT_CAPACITY,
                       LM_DEFAULT_LOAD_FACTOR);
}

/* Check whether bucket is used or not */
inline static
bool lilmap_is_occupied(struct LilMapBucket *bucket) {
    return (bucket->value != NULL) && (bucket->value != LM_TOMBSTONE);
}

/* Free map */
inline static
void lilmap_free(LilMap *map) {
    free(map->buckets);
}


#endif //LILMAP2_LILMAP_H
