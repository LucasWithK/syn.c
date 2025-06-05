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

typedef struct {
    char *items;
    size_t count;
} str;

#define strb_push(strb, char) da_append((strb), (char))

#define strb_appendc(strb, cstr) da_append_many((strb), (cstr), strlen((cstr)))

#define strb_append(strb, str) da_append_many((strb), (str).items, (cstr).count)

#define strb_free(strb) da_free((strb))

str strb_build(strb sb);

#define STR_FMT "%.*s"
#define STR_ARG(str) (int) (str).count, (str).items

str str_cstr(char *cstr);

str str_substr(str s, size_t start, size_t end);

bool str_starts_with(str a, str b);

void str_split(str s, size_t mid, str *a, str *b);

bool str_findc(str s, char c, size_t *i);

bool str_find(str s, str n, size_t *i);

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

void str_split(str s, size_t mid, str *a, str *b) {
    assert(mid <= s.count);
    a->items = s.items;
    a->count = mid;
    b->items = s.items + mid;
    b->count = s.count - mid;
}

bool str_findc(str s, char c, size_t *i) {
    while(*i < s.count) {
        if(s.items[*i] == c) return true;
        (*i)++;
    }
    return false;
}

bool str_find(str s, str n, size_t *i) {
    if(n.count == 0 || s.count < n.count) return false;

    char c = n.items[0];
    while(str_findc(s, c, i)) {
        if(str_starts_with(str_substr(s, *i, s.count), n)) return true;
        (*i)++;
    }
    return false;
}

#endif // STR_IMPLEMENTATION

#endif // STR_H_
