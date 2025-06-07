#ifndef STR_H_
#define STR_H_

#include <stdlib.h>
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

void strb_push(strb *sb, char c);

void strb_append_cstr(strb *sb, char *cs);

void strb_append(strb *sb, str s);

str strb_build(strb *sb);

void strb_free(strb sb);

#define STR_FMT "%.*s"
#define STR_ARG(str) (int) (str).count, (str).items

str str_cstr(char *cstr);

str str_substr(str s, size_t start, size_t end);

bool str_starts_with(str a, str b);

bool str_eq(str a, str b);

void str_split(str s, size_t mid, str *a, str *b);

bool str_find_char(str s, char c, size_t *i);

bool str_contains_char(str s, char c);

bool str_find(str s, str n, size_t *i);

bool str_contains(str s, str n);

str str_trim_n(str s, size_t n);

str str_trim(str s, str ws);

#endif // STR_H_
