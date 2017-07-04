/*
 * Copyright (C) Yichun Zhang (agentzh)
 */

#ifndef NGX_HTTP_SSDB_MODULE_H
#define NGX_HTTP_SSDB_MODULE_H

#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

typedef struct ngx_http_ssdb_ctx_s ngx_http_ssdb_ctx_t;

typedef ngx_int_t (*ngx_http_ssdb_filter_handler_ptr)
    (ngx_http_ssdb_ctx_t *ctx, ssize_t bytes);

struct ngx_http_ssdb_ctx_s {
	ngx_http_request_t *request;
    int                 state;
    size_t              chunk_length;
    size_t              chunk_read;
    ngx_http_ssdb_filter_handler_ptr filter;
};

typedef struct {
    ngx_http_upstream_conf_t upstream;
    ngx_array_t             *queries;
} ngx_http_ssdb_loc_conf_t;

extern ngx_module_t  ngx_http_ssdb_module;

#endif /* NGX_HTTP_SSDB_MODULE_H */
