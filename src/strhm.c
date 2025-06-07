/**
 * This file provides the implementation for a hashmap (`strhm`) data structure
 * with both keys and values as `str` from `str.h`.
 *
 * A hashmap is simply a dynamic array from `da.h`.
 * This means any `da_*` definitions can be used on a hashmap.
 */

#include "strhm.h"

#include "da.h"

size_t hm_djb2(str s) {
    size_t hash = 5381;
    for(size_t i=0; i<s.count; i++)
        hash = ((hash << 5) + hash) + s.items[i];
    return hash;
}

void hm_reserve(strhm *hm, size_t capacity) {
    if(capacity > hm->capacity) {
        strhm new_hm = {0};
        da_reserve(&new_hm, capacity);

        for(size_t i=0; i<hm->capacity; ++i)
            if(hm->items[i].hash != 0)
                hm_insert(&new_hm, hm->items[i].key, hm->items[i].value);

        free(hm->items);
        hm->items = new_hm.items;
        hm->capacity = new_hm.capacity;
    }
}

void hm_insert(strhm *hm, str key, str value) {
    if(hm->capacity == 0) da_reserve(hm, DA_INIT_CAPACITY);
    if(hm->count * HM_LOAD_FACTOR_D >= hm->capacity * HM_LOAD_FACTOR_N)
        hm_reserve(hm, hm->capacity * 2);
    size_t hash = HM_HASH(key);
    size_t idx = hash & (hm->capacity - 1);
    while(hm->items[idx].hash != hash)
        if(hm->items[idx].hash == 0) break;;
        idx = (idx + 1) & (hm->capacity - 1);
    hm->items[idx].hash = hash;
    hm->items[idx].key = key;
    hm->items[idx].value = value;
    hm->count++;
}

bool hm_get(strhm *hm, str key, str *value) {
    size_t hash = HM_HASH(key);
    size_t idx = hash & (hm->capacity - 1);
    while(hm->items[idx].hash != hash) {
        if(hm->items[idx].hash == 0) return false;
        idx = (idx + 1) & (hm->capacity - 1);
    }
    *value = hm->items[idx].value;
    return true;
}

void hm_free(strhm hm) {
    da_free(hm);
}
