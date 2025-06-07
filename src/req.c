#include "req.h"

#include <stdio.h>
#include <assert.h>

bool parse_start(str start_line, request *req) {
    const str sp = str_cstr(" ");

    size_t start = 0, end = 0;
    assert(str_find(start_line, sp, &end));
    if(start == end) return false;
    str m_str = str_substr(start_line, start, end);
    printf(STR_FMT"\n", STR_ARG(m_str));

    start = end += sp.count;
    assert(str_find(start_line, sp, &end));
    if(start == end) return false;
    str target = str_substr(start_line, start, end);
    printf(STR_FMT"\n", STR_ARG(target));

    end += sp.count;
    if(end == start_line.count) return false;
    str version = str_substr(start_line, end, start_line.count);
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

bool parse_field(str field_line, request *req) {
    const str colon = str_cstr(":");
    const str ws = str_cstr(" \t");

    size_t start = 0, end = 0;
    assert(str_find(field_line, colon, &end));
    if(start == end) return false;
    str key = str_substr(field_line, start, end);
    printf(STR_FMT"\n", STR_ARG(key));

    end += colon.count;
    if(end == field_line.count) return false;
    str value = str_trim(str_substr(field_line, end, field_line.count), ws);
    printf(STR_FMT"\n", STR_ARG(value));

    hm_insert(&req->fields, key, value);
    return true;
}
