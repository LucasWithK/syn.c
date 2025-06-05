#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "da.h"

#define STR_IMPLEMENTATION
#include "str.h"

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

    const int max_con_reqs = 10;
    if(listen(fd, max_con_reqs) == -1) {
        close(fd);
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    return fd;
}

int main(void) {
    int lfd = listenfd();

    int rfd, n;
    strb reqb = {0};
    for(int i=0; i<5; ++i) {
        if((rfd = accept(lfd, NULL, NULL)) == -1) {
            perror("ERROR: accept");
            continue;
        }

        const int req_window = 2048;

        reqb.count = 0;
        da_reserve(&reqb, req_window);
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

        str req = strb_build(reqb);

        printf(STR_FMT, STR_ARG(req));

        const char* res = "HTTP/1.1 200 OK\r\n\r\nHELLO";

        if(send(rfd, res, strlen(res), 0) == -1) {
            close(rfd);
            perror("ERROR: send");
            continue;
        }

        close(rfd);
    }

    close(lfd);
    strb_free(reqb);

    return 0;
}
