#ifndef REQ_H_
#define REQ_H_

#include "str.h"
#include "strhm.h"

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
    strhm fields;
} request;

bool parse_start(str start_line, request *req);

bool parse_field(str field_line, request *req);

#endif // REQ_H_
