#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "da.h"
#include "str.h"
#include "req_res.h"
#include "strhm.h"
#include "handler.h"
#include "threads.h"

int listenfd() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(fd);
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    const int max_con_reqs = 32;
    if(listen(fd, max_con_reqs) == -1) {
        close(fd);
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    return fd;
}

int main(void) {
    int lfd = listenfd();

    th_init();

    int rfd;
    while(1) {
        if((rfd = accept(lfd, NULL, NULL)) == -1) {
            perror("ERROR: accept");
            continue;
        }
        if(th_queue(rfd)) {
            close(rfd);
            break;
        }
    }

    close(lfd);
    th_free();

    th_debug();

    return 0;
}
