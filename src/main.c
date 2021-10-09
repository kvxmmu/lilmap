#include <stdio.h>

#include "lilmap/lilmap.h"

int main() {
    int a = 10, b = 100;

    LILMap map;
    lilmap_init_defaults(&map);
    lilmap_set(&map, 20, &a);
    lilmap_set(&map, 110, &b);

    void *aptr, *bptr, *cptr = NULL;
    lilmap_lookup(&map, 20, &aptr);
    lilmap_lookup(&map, 110, &bptr);
    lilmap_lookup(&map, 120, &cptr);

    printf("a is %d; b is %d; cptr - %p\n", *(int *)aptr, *(int *)bptr, cptr);
    lilmap_erase(&map, 20);
    aptr = NULL;
    lilmap_lookup(&map, 20, &aptr);

    printf("a is %p\n", aptr);

    lilmap_free(&map);

    return 0;
}
