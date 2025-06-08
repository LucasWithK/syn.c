#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    pthread_t *items;
    size_t count;
    size_t capacity;
} tpool;

typedef struct {
    int *items;
    size_t count;
    size_t capacity;

    size_t front;
    size_t rear;
} tqueue;

typedef struct {
    tpool pool;
    tqueue queue;

    bool shutdown;

    pthread_cond_t notify;
    pthread_mutex_t lock;
} tdata;

void th_init();

bool th_queue(int rfd);

void* routine(void *_arg);

void th_free();

void th_debug();

#endif // THREADS_H_
