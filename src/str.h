/**
 * This file provides the implementations for string builder and view data structures.
 *
 * A string builder is simply a dynamic array from `da.h` with the following shape:
 *
 * ```c
 * typedef struct {
 *  char *items;
 *  size_t count;
 *  size_t capacity;
 * } strb;
 * ```
 *
 * This means any `da_*` macros can be used on a string builder.
 *
 * A string view is like a window into any other string.
 * It has the following shape:
 *
 * ```c
 * typdef struct {
 *  char *items;
 *  size_t count;
 * } str;
 * ```
 *
 * The following definitions can be used on a string builder:
 *
 * `strb_push(&strb, char);`
 * `strb_append(&strb, cstr);`
 *
 * `str strb_build(strb sb);`
 *
 * `strb_free(strb);`
 *
 * The following definitions can be used on a string view:
 *
 * `str str_cstr(char *cstr);`
 *
 * `STR_ARG(str)` used togheter with `STR_FMT` for `printf` formatting.
 *
 * `str str_substr(str s, size_t start, size_t end);`
 * `bool str_starts_with(str a, str b);`
 *
 * It is recommended to construct a string using a string builder.
 * Then continue using it as an immutable data structure by creating a view from it.
 * And finally freeing the builder when no view exists anymore.
 * Never create a string view, just get it returned by a function.
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
    assert(start < end && end <= s.count);
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


