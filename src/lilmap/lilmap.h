//
// Created by nero on 10/9/21.
//

#ifndef LILMAP_LILMAP_H
#define LILMAP_LILMAP_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(__GNUC__) || defined(__llvm__)
#define GNU_EXTENSIONS
#endif

#ifdef GNU_EXTENSIONS
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define LM_INLINE inline __attribute__((always_inline))

#else
#define likely(x)   (x)
#define unlikely(x) (!!(x))

#define LM_INLINE inline
#endif

#define LM_LOAD_FACTOR (0.75f)
#define DEFAULT_BUCKETS_COUNT 75

typedef int64_t lilmap_int;
typedef float   lilmap_float;

struct LILMapBucket;
struct LILMapBucket {
    uint8_t is_used;

    lilmap_int key;
    void *data;

    struct LILMapBucket *next;
};

typedef struct {
    struct LILMapBucket *buckets;

    size_t buckets_allocated;
    size_t buckets_used;

    lilmap_float load_factor;
} LILMap;

LM_INLINE
size_t lilmap_size(LILMap *map) {
    return map->buckets_used;
}

LM_INLINE
bool lilmap_empty(LILMap *map) {
    return lilmap_size(map) == 0;
}

LM_INLINE
void lilmap_set_load_factor(LILMap *map, lilmap_float factor) {
    map->load_factor = factor;
}

LM_INLINE
size_t lilmap_get_index(LILMap *map, lilmap_int key) {
    size_t index = map->buckets_allocated % key;
    if (unlikely(index == map->buckets_allocated)) {
        return --index;
    }

    return index;
}

LM_INLINE
float lilmap_calc_load_factor(LILMap *map, float additional) {
    if (unlikely(lilmap_empty(map))) {
        return 0.f;
    }

    return ((lilmap_float)map->buckets_used + additional) / ((lilmap_float)map->buckets_allocated);
}

LM_INLINE
void lilmap_add_bucket(struct LILMapBucket *bucket,
        struct LILMapBucket *new_bucket) {
    new_bucket->next = bucket->next;
    bucket->next = new_bucket;
}

void lilmap_set(LILMap *map, lilmap_int key,
                void *value);
bool lilmap_lookup(LILMap *map, lilmap_int key,
                   void **value);


LM_INLINE
bool lilmap_erase(LILMap *map, lilmap_int key) {
    size_t index = map->buckets_allocated % key;
    struct LILMapBucket *bucket = &map->buckets[index];

    struct LILMapBucket *last_bucket = bucket;
    struct LILMapBucket *cur_bucket = bucket;

    do {
        if (cur_bucket->key == key) {
            cur_bucket->is_used = 0;
            last_bucket->next = cur_bucket->next;

            if (unlikely(bucket != cur_bucket)) free(cur_bucket);

            return true;
        }

        last_bucket = cur_bucket;
        cur_bucket = cur_bucket->next;
    } while (cur_bucket != NULL);

    return false;
}

void lilmap_grow(LILMap *map, size_t power);
void lilmap_init(LILMap *map, lilmap_float load_factor);
void lilmap_free(LILMap *map);

LM_INLINE
void lilmap_init_defaults(LILMap *map) {
    lilmap_init(map, LM_LOAD_FACTOR);
}

#endif //LILMAP_LILMAP_H
