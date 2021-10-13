#include <stdio.h>

#include "lilmap/lilmap.h"

int main() {
    int key1 = 100, key2 = 110;
    int val1 = 120, val2 = 130;

    LilMap map;
    lilmap_init(&map);

    lilmap_set(&map, key1, &val1);
    lilmap_set(&map, key2, &val2);

    void *ptr1, *ptr2;
    ptr1 = lilmap_lookup(&map, key1);
    ptr2 = lilmap_lookup(&map, key2);

    int *i1 = ptr1, *i2 = ptr2;

    lilmap_free(&map);

    return 0;
}
