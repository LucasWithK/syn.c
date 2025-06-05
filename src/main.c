#include <stdio.h>

#include "str.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} ints;

int main(void) {

    str s = {0};

    str_append(&s, "Hello, World");

    str_push(&s, '!');

    printf("CONTENT "STRV_FMT" END\n", STRV_ARG(s));

    str_free(s);

    return 0;
}
