#include <stdio.h>

#include "da.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} ints;


int main(void) {
    ints is = {0};

    da_append(&is, 0);

    int as[] = {1, 2};
    da_append_many(&is, as, 2);

    da_resize(&is, 15);

    for(size_t i=0; i<is.count; ++i) {
        printf("%d\n", is.items[i]);
    }

    da_free(is);

    return 0;
}
