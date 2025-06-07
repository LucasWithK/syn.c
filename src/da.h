/**
 * This file provides the implementations for a dynamic array data structure.
 * It requires a struct of the following shape for an array containing type `T`:
 *
 * ```c
 * typedef struct {
 *  T *items;
 *  size_t count;
 *  size_t capacity;
 * } ts;
 * ```
 *
 * -------------------------------------------------------------------------
 * IMPORTANT NOTICE:
 * This dynamic array implementation was originally sourced and heavily inspired
 * by code found here: `https://github.com/tsoding/nob.h`.
 *
 * Please ensure you comply with the original license terms of that repository
 * if you are reusing or distributing this code.
 * -------------------------------------------------------------------------
 */

#ifndef DA_H_
#define DA_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef DA_INIT_CAPACITY
#define DA_INIT_CAPACITY 256
#endif

#define da_reserve(da, new_capacity)                                                    \
    do {                                                                                \
        if ((new_capacity) > (da)->capacity) {                                          \
            if ((da)->capacity == 0) {                                                  \
                (da)->capacity = DA_INIT_CAPACITY;                                      \
            }                                                                           \
            while ((new_capacity) > (da)->capacity) {                                   \
                (da)->capacity *= 2;                                                    \
            }                                                                           \
            (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));  \
            assert((da)->items != NULL);                                                \
        }                                                                               \
    } while (0)

#define da_append(da, item)                     \
    do {                                        \
        da_reserve((da), (da)->count + 1);      \
        (da)->items[(da)->count++] = (item);    \
    } while (0)

#define da_free(da)         \
    do {                    \
        free((da).items);   \
        (da).items = 0;     \
    } while (0)             \

#define da_append_many(da, new_items, new_items_count)                                          \
    do {                                                                                        \
        da_reserve((da), (da)->count + (new_items_count));                                      \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); \
        (da)->count += (new_items_count);                                                       \
    } while (0)

#define da_resize(da, size)     \
    do {                        \
        da_reserve((da), size); \
        (da)->count = (size);   \
    } while (0)

#endif // DA_H_
