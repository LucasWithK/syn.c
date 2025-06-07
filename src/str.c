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

#include "str.h"
#include "da.h"

void strb_push(strb *sb, char c) {
    da_append(sb, c);
}

void strb_append_cstr(strb *sb, char *cs) {
    da_append_many(sb, cs, strlen(cs));
}

void strb_append(strb *sb, str s) {
    da_append_many(sb, s.items, s.count);
}

str strb_build(strb *sb) {
    return (str) {
        .items = sb->items,
        .count = sb->count,
    };
}

void strb_free(strb sb) {
    da_free(sb);
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

bool str_eq(str a, str b) {
    if(a.count != b.count) return false;
    return memcmp(a.items, b.items, a.count) == 0;
}

void str_split(str s, size_t mid, str *a, str *b) {
    assert(mid <= s.count);
    a->items = s.items;
    a->count = mid;
    b->items = s.items + mid;
    b->count = s.count - mid;
}

bool str_find_char(str s, char c, size_t *i) {
    while(*i < s.count) {
        if(s.items[*i] == c) return true;
        (*i)++;
    }
    return false;
}

bool str_find(str s, str n, size_t *i) {
    if(n.count == 0 || s.count < n.count) return false;

    char c = n.items[0];
    while(str_find_char(s, c, i)) {
        if(str_starts_with(str_substr(s, *i, s.count), n)) return true;
        (*i)++;
    }
    return false;
}

str str_trim_n(str s, size_t n) {
    assert(n <= s.count);
    return (str) {
        .items = s.items + n,
        .count = s.count - n,
    };
}
