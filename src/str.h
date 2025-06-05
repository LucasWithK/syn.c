/**
 * This file provides the implementations for a string builder (`strb`)
 * and a string view (`str`) data structure.
 *
 * A string builder is simply a dynamic array from `da.h`.
 *
 * This means any `da_*` definitions can be used on a string builder.
 *
 * A string view is like a window into any other string.
 *
 * It is recommended to construct a string using a string builder.
 * Then continue using it as an immutable data structure by creating a view from it.
 * And finally freeing the builder when no view exists anymore.
 */

#ifndef STR_H_
#define STR_H_

#include "da.h"
#include <stdbool.h>

typedef struct {
    char *items;
    size_t count;
    size_t capacity;
} strb;

#define strb_push(strb, char) da_append((strb), (char))

#define strb_append(strb, cstr) da_append_many((strb), (cstr), strlen((cstr)))

#define strb_free(strb) da_free((strb))

typedef struct {
    char *items;
    size_t count;
} str;

str strb_build(strb sb);

str str_cstr(char *cstr);

#define STR_FMT "%.*s"
#define STR_ARG(str) (int) (str).count, (str).items

str str_substr(str s, size_t start, size_t end);

bool str_starts_with(str a, str b);

#ifdef STR_IMPLEMENTATION

str strb_build(strb sb) {
    return (str) {
        .items = sb.items,
        .count = sb.count,
    };
}

str str_cstr(char *cstr) {
    return (str) {
        .items = cstr,
        .count = strlen(cstr),
    };
}

str str_substr(str s, size_t start, size_t end) {
    assert(start <= end && end <= s.count);
    return (str) {
        .items = s.items + start,
        .count = end - start,
    };
}

bool str_starts_with(str a, str b) {
    if(a.count < b.count) return false;
    return memcmp(a.items, b.items, b.count) == 0;
}

#endif // STR_IMPLEMENTATION

#endif // STR_H_
