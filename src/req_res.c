#include "req_res.h"

bool req_new(str req_str, request *req) {
    const str crlf = str_cstr("\r\n");

    size_t start = 0, end = 0;
    if(!str_find(req_str, crlf, &end)) return false;
    while(start == end) { // ignore at least one empty line
        start = end += crlf.count;
        if(!str_find(req_str, crlf, &end)) return false;
    }
    str line = str_substr(req_str, start, end);
    if(!req_start(line, req)) return false;

    while(1) {
        start = end += crlf.count;
        if(!str_find(req_str, crlf, &end)) return false;
        if(start == end) break;
        line = str_substr(req_str, start, end);
        if(!req_res_field(line, &req->fields)) return false;
    }

    end += crlf.count;
    req->body = str_substr(req_str, end, req_str.count);

    return true;
}

bool req_start(str start_line, request *req) {
    const str sp = str_cstr(" ");

    size_t start = 0, end = 0;
    if(!str_find(start_line, sp, &end)) return false;
    if(start == end) return false;
    str m_str = str_substr(start_line, start, end);

    start = end += sp.count;
    if(!str_find(start_line, sp, &end)) return false;
    if(start == end) return false;
    str target = str_substr(start_line, start, end);

    end += sp.count;
    if(end == start_line.count) return false;
    str version = str_substr(start_line, end, start_line.count);

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

void req_free(request req) {
    hm_free(req.fields);
}

void res_new(str status, str body, response *res) {
    const str version = str_cstr("HTTP/1.1");

    res->version = version;
    res->status = status;
    res->body = body;
}

void res_build(response *res, strb *buf) {
    const str sp = str_cstr(" ");
    const str crlf = str_cstr("\r\n");
    const str colon = str_cstr(": ");

    strb_append(buf, res->version);
    strb_append(buf, sp);
    strb_append(buf, res->status);
    strb_append(buf, crlf);

    for(size_t i=0; i<res->fields.capacity; ++i)
        if(res->fields.items[i].hash != 0) {
            strb_append(buf, res->fields.items[i].key);
            strb_append(buf, colon);
            strb_append(buf, res->fields.items[i].value);
            strb_append(buf, crlf);
        }
    strb_append(buf, crlf);
    if(res->body.count) strb_append(buf, res->body);
}

void res_free(response res) {
    hm_free(res.fields);
}

bool req_res_field(str field_line, strhm *fields) {
    const str colon = str_cstr(":");
    const str ws = str_cstr(" \t");

    size_t start = 0, end = 0;
    if(!str_find(field_line, colon, &end)) return false;
    if(start == end) return false;
    str key = str_substr(field_line, start, end);

    end += colon.count;
    if(end == field_line.count) return false;
    str value = str_trim(str_substr(field_line, end, field_line.count), ws);

    hm_insert(fields, key, value);
    return true;
}
