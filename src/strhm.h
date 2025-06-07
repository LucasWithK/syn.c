#ifndef HM_H_
#define HM_H_

#include "str.h"
#include <stdlib.h>

typedef struct {
    size_t hash;
    str key;
    str value;
} strhm_item;

typedef struct {
    strhm_item *items;
    size_t count;
    size_t capacity;
} strhm;

#define HM_LOAD_FACTOR_N 3
#define HM_LOAD_FACTOR_D 4

#define HM_HASH hm_djb2

size_t hm_djb2(str s);

void hm_reserve(strhm *hm, size_t capacity);

void hm_insert(strhm *hm, str key, str value);

bool hm_get(strhm *hm, str key, str *value);

void hm_free(strhm hm);

#endif // HM_H_
