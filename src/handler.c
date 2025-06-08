#include "handler.h"

#include "str.h"
#include "req_res.h"

bool handle_get(request *req, response *res) {
    const str exit = str_cstr("/exit");
    if(str_eq(req->target, exit)) return true;

    const str status = str_cstr("200 OK");
    const str body = str_cstr("OK");
    res_new(status, body, res);
    return false;
}
