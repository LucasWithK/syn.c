#ifndef REQ_H_
#define REQ_H_

#include "str.h"

typedef enum {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
} method;

typedef struct {
    method m;
    str target;
    str version;
} request;

bool parse_req(str req_line, request *req);

#endif
