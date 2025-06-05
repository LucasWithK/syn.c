/**
 * This file provides the macro implementations for a string and string view data structure.
 * A string is simply a dynamic array from `da.h` with the following shape:
 *
 * ```c
 * typedef struct {
 *  char *items;
 *  size_t count;
 *  size_t capacity;
 * } str;
 * ```
 *
 * This means any `da_*` macros can be used on a string.
 *
 * The following macros can be used on a string:
 *
 * `str_push(&str, char);`
 * `str_append(&str, cstr);`
 *
 * `str_free(str);`
 *
 * A string view is like a window into a string.
 * It is valid as long as the original string is not realoccated.
 * It has the following shape:
 *
 * ```c
 * typdef struct {
 *  char *items;
 *  size_t count;
 * } strv;
 * ```
 *
 * The following macros can be used on a string and a string view:
 *
 * `STRV_ARG(strv)` used togheter with `STRV_FMT` for `printf` formatting.
 *
 * -------------------------------------------------------------------------
 * IMPORTANT NOTICE:
 * This dynamic array implementation was originally sourced or heavily inspired
 * by code found here: `https://github.com/tsoding/nob.h`.
 *
 * Please ensure you comply with the original license terms of that repository
 * if you are reusing or distributing this code.
 * -------------------------------------------------------------------------
 */

#ifndef STR_H_
#define STR_H_

#include "da.h"

typedef struct {
    char *items;
    size_t count;
    size_t capacity;
} str;

#define str_push(str, char) da_append((str), (char))

#define str_append(str, cstr) da_append_many((str), (cstr), strlen((cstr)))

#define str_free(str) da_free((str))

typedef struct {
    char *items;
    size_t count;
} strv;

#define STRV_FMT "%.*s"
#define STRV_ARG(strv) (int) (strv).count, (strv).items


#endif // STR_H_
