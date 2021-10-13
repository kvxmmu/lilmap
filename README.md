# LILMap
Specific hashmap that works fine with monotonic integer values starting from zero.

# TODO

- Optimize division with bitwise operations

# Public API Doc

```c
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

/* Initialize lilmap with default settings */
inline static void lilmap_init(LilMap *map);

/* Free lilmap's memory */
inline static void lilmap_free(LilMap *map)
```
