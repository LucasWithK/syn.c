#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "da.h"
#include "str.h"
#include "req_res.h"
#include "strhm.h"

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

    int rfd;
    ssize_t n;
    strb reqb = {0}, resb = {0};

    const size_t req_window = 4096;
    da_reserve(&reqb, req_window);

    const size_t res_window = 1024;
    da_reserve(&resb, res_window);

    bool run = true;
    while(run) {
        if((rfd = accept(lfd, NULL, NULL)) == -1) {
            perror("ERROR: accept");
            continue;
        }

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
            perror("ERROR: req_new");
            continue;
        }

        response res = {0};

        const str ok = str_cstr("200 OK");
        const str slash = str_cstr("/");
        const str exit_str = str_cstr("/exit");

        switch (req.m) {
        case GET: if(str_eq(req.target, slash)) {
            const str body = str_cstr("<p>HOME</p><a href=\"/exit\">EXIT</a>");
            res_new(ok, body, &res);
            break;
        } else if(str_eq(req.target, exit_str)) {
            const str body = str_cstr("EXIT");
            res_new(ok, body, &res);
            run = false;
            break;
        }
        default: {
            const str not_impl = str_cstr("501 Not Implemented");
            const str empty = str_cstr("");
            res_new(not_impl, empty, &res);
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
    }

    close(lfd);
    strb_free(reqb);
    strb_free(resb);

    return 0;
}
