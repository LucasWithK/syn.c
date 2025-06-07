#ifndef REQ_RES_H_
#define REQ_RES_H_

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
    str body;
} request;

typedef struct {
    str version;
    str status;
    strhm fields;
    str body;
} response;

bool req_new(str req_str, request *req);

bool req_start(str start_line, request *req);

bool req_field(str field_line, request *req);

void req_free(request req);

void res_new(str status, str body, response *res);

void res_field(str key, str value, response *res);

void res_build(response *res, strb *buf);

void res_free(response res);

#endif // REQ_RES_H_
