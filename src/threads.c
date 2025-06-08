#include "threads.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "da.h"
#include "str.h"
#include "req_res.h"
#include "handler.h"

tdata _data = {0};

void th_init() {
    da_reserve(&_data.pool, DA_INIT_CAPACITY);

    da_reserve(&_data.queue, DA_INIT_CAPACITY);

    _data.shutdown = false;

    pthread_cond_init(&_data.notify, NULL);
    pthread_mutex_init(&_data.lock, NULL);

    for(; _data.pool.count<_data.pool.capacity; _data.pool.count++) {
        pthread_create(&_data.pool.items[_data.pool.count], NULL, routine, NULL);
    }
}

void* routine(void *_arg) {

    ssize_t n;
    strb reqb = {0}, resb = {0};

    const size_t req_window = 4096;
    da_reserve(&reqb, req_window);

    const size_t res_window = 1024;
    da_reserve(&resb, res_window);

    while(1) {
        pthread_mutex_lock(&_data.lock);

        while(!_data.shutdown && _data.queue.count == 0) {
            pthread_cond_wait(&_data.notify, &_data.lock);
        }

        if(_data.shutdown) {
            pthread_mutex_unlock(&_data.lock);
            break;
        }

        int rfd = _data.queue.items[_data.queue.front];
        _data.queue.front = (_data.queue.front + 1) & (_data.queue.capacity - 1);
        _data.queue.count--;

        pthread_mutex_unlock(&_data.lock);

        reqb.count = 0;
        while((n = recv(rfd, reqb.items + reqb.count, req_window, 0)) == req_window) {
            reqb.count += req_window;
            da_reserve(&reqb, reqb.count + req_window);
        }
        if(n == -1) {
            close(rfd);
            perror("ERROR: recv");
            continue;
        }
        reqb.count += n;
        str req_str = strb_build(&reqb);

        request req = {0};

        if(!req_new(req_str, &req)) {
            req_free(req);
            close(rfd);
            fprintf(stderr, "ERROR: req_new");
            continue;
        }

        response res = {0};

        switch (req.m) {
        case GET:
            if(handle_get(&req, &res)) {
                const str status = str_cstr("200 OK");
                const str body = str_cstr("SHUTTING DOWN ...");
                res_new(status, body, &res);

                pthread_mutex_lock(&_data.lock);
                _data.shutdown = true;
                pthread_mutex_unlock(&_data.lock);

                pthread_cond_broadcast(&_data.notify);
            } break;
        default: {
            const str status = str_cstr("501 Not Implemented");
            const str body = str_cstr("");
            res_new(status, body, &res);
        }};

        resb.count = 0;
        res_build(&res, &resb);

        str res_str = strb_build(&resb);
        if(send(rfd, res_str.items, res_str.count, 0) == -1) {
            req_free(req);
            res_free(res);
            close(rfd);
            perror("ERROR: send");
            continue;
        }

        req_free(req);
        res_free(res);
        close(rfd);

        sleep(1);
    }

    strb_free(reqb);
    strb_free(resb);

    return NULL;
}

bool th_queue(int rfd) {
    pthread_mutex_lock(&_data.lock);
    if(_data.shutdown) {
        pthread_mutex_unlock(&_data.lock);
        return true;
    }

    if(_data.queue.count == _data.queue.capacity) {
        size_t new_capacity = _data.queue.capacity * 2;
        da_reserve(&_data.queue, new_capacity);
        _data.queue.rear = _data.queue.count;
        _data.queue.front = 0;

        new_capacity = _data.pool.capacity * 2;
        da_reserve(&_data.pool, new_capacity);
        for(; _data.pool.count<_data.pool.capacity; _data.pool.count++) {
            pthread_create(&_data.pool.items[_data.pool.count], NULL, routine, NULL);
        }
    }

    _data.queue.items[_data.queue.rear] = rfd;
    _data.queue.rear = (_data.queue.rear + 1) & (_data.queue.capacity - 1);
    _data.queue.count++;

    pthread_cond_signal(&_data.notify);
    pthread_mutex_unlock(&_data.lock);

    return false;
}

void th_free() {
    for(size_t i=0; i<_data.pool.count; ++i) {
        pthread_join(_data.pool.items[i], NULL);
    }

    da_free(_data.pool);
    da_free(_data.queue);
    pthread_cond_destroy(&_data.notify);
    pthread_mutex_destroy(&_data.lock);
}
