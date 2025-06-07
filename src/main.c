#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "da.h"
#include "str.h"
#include "req.h"
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
    strb reqb = {0};

    const size_t req_window = 4096;
    da_reserve(&reqb, req_window);

    for(int i=0; i<1; ++i) {
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

        const str crlf = str_cstr("\r\n");

        size_t start = 0, end = 0;
        assert(str_find(req_str, crlf, &end));
        while(start == end) { // ignore at least one empty line
            start = end += crlf.count;
            assert(str_find(req_str, crlf, &end));
        }
        str line = str_substr(req_str, start, end);
        if(!parse_start(line, &req)) {
            close(rfd);
            perror("ERROR: parse_req");
            continue;
        }

        while(1) {
            start = end += crlf.count;
            assert(str_find(req_str, crlf, &end));
            if(start == end) break;
            line = str_substr(req_str, start, end);
            assert(parse_field(line, &req));
        }

        const char* res_str = "HTTP/1.1 200 OK\r\n\r\nHELLO";

        if(send(rfd, res_str, strlen(res_str), 0) == -1) {
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
