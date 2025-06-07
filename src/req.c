#include "req.h"

#include <stdio.h>
#include <assert.h>

bool parse_req(str req_line, request *req) {
    const str sp = str_cstr(" ");

    size_t start = 0, end = 0;
    assert(str_find(req_line, sp, &end));
    if(start == end) return false;
    str m_str = str_substr(req_line, start, end);
    printf(STR_FMT"\n", STR_ARG(m_str));

    start = end += sp.count;
    assert(str_find(req_line, sp, &end));
    if(start == end) return false;
    str target = str_substr(req_line, start, end);
    printf(STR_FMT"\n", STR_ARG(target));

    end += sp.count;
    if(end == req_line.count) return false;
    str version = str_substr(req_line, end, req_line.count);
    printf(STR_FMT"\n", STR_ARG(version));

    method m;
    if(str_eq(m_str, str_cstr("GET"))) m = GET;
    else if(str_eq(m_str, str_cstr("HEAD"))) m = HEAD;
    else if(str_eq(m_str, str_cstr("POST"))) m = POST;
    else if(str_eq(m_str, str_cstr("PUT"))) m = PUT;
    else if(str_eq(m_str, str_cstr("DELETE"))) m = DELETE;
    else if(str_eq(m_str, str_cstr("CONNECT"))) m = CONNECT;
    else if(str_eq(m_str, str_cstr("OPTIONS"))) m = OPTIONS;
    else if(str_eq(m_str, str_cstr("TRACE"))) m = TRACE;
    else return false;

    req->m = m;
    req->target = target;
    req->version = version;

    return true;
}
