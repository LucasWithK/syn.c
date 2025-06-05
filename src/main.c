#include <stdio.h>

#define STR_IMPLEMENTATION
#include "str.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} ints;

int main(void) {

    strb sb = {0};

    strb_append(&sb, "Hello, World");

    strb_push(&sb, '!');

    str s = strb_build(sb);

    printf("CONTENT "STR_FMT" END\n", STR_ARG(s));

    str x = str_substr(s, 5, s.count);

    printf("CONTENT "STR_FMT" END\n", STR_ARG(x));

    putchar('0' + str_starts_with(s, x));
    putchar('0' + str_starts_with(s, str_cstr("Hello")));
    putchar('0' + str_starts_with(s, str_cstr("World")));
    putchar('0' + str_starts_with(s, str_cstr("")));
    putchar('0' + str_starts_with(s, s));
    putchar('0' + str_starts_with(x, s));
    putchar('0' + str_starts_with(x, str_cstr(",")));

    putchar('\n');

    strb_free(sb);

    return 0;
}
