#include "ngx_http_ssdb_handler.h"
#include "ngx_http_ssdb_module.h"
#include "ngx_http_ssdb_response.h"

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

static ngx_int_t ngx_http_ssdb_create_request(ngx_http_request_t *r);
static ngx_int_t ngx_http_ssdb_reinit_request(ngx_http_request_t *r);
static ngx_int_t ngx_http_ssdb_process_header(ngx_http_request_t *r);
static ngx_int_t ngx_http_ssdb_filter_init(void *data);
static ngx_int_t ngx_http_ssdb_filter(void *data, ssize_t bytes);
static void ngx_http_ssdb_abort_request(ngx_http_request_t *r);
static void ngx_http_ssdb_finalize_request(ngx_http_request_t *r,
    ngx_int_t rc);

ngx_int_t
ngx_http_ssdb_handler(ngx_http_request_t *r)
{
	ngx_int_t                   rc;
	ngx_http_upstream_t         *u;
	ngx_http_ssdb_ctx_t        *ctx;
	ngx_http_ssdb_loc_conf_t   *rlcf;

	if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
		return NGX_HTTP_NOT_ALLOWED;
	}

	rc = ngx_http_discard_request_body(r);

	if (rc != NGX_OK) {
		return rc;
	}

	if (ngx_http_set_content_type(r) != NGX_OK) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	if (ngx_http_upstream_create(r) != NGX_OK) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	u = r->upstream;

	ngx_str_set(&u->schema, "ssdb://");
	u->output.tag = (ngx_buf_tag_t) &ngx_http_ssdb_module;
	rlcf = ngx_http_get_module_loc_conf(r, ngx_http_ssdb_module);
	u->conf = &rlcf->upstream;

	u->create_request = ngx_http_ssdb_create_request;
	u->reinit_request = ngx_http_ssdb_reinit_request;
	u->process_header = ngx_http_ssdb_process_header;
	u->abort_request = ngx_http_ssdb_abort_request;
	u->finalize_request = ngx_http_ssdb_finalize_request;

	ctx = ngx_palloc(r->pool, sizeof(ngx_http_ssdb_ctx_t));
	if(ctx == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	ctx->request = r;
	ctx->state = NGX_ERROR;

	ngx_http_set_ctx(r, ctx, ngx_http_ssdb_module);

	u->input_filter_init = ngx_http_ssdb_filter_init;
	u->input_filter = ngx_http_ssdb_filter;
	u->input_filter_ctx = ctx;

	r->main->count++;

	ngx_http_upstream_init(r);

  return NGX_DONE;
}

static ngx_int_t
ngx_http_ssdb_create_request(ngx_http_request_t *r)
{
    ngx_http_ssdb_loc_conf_t *rlcf;
    ngx_buf_t                *b;
    ngx_chain_t              *cl;

    ngx_str_t str = ngx_string("3\nget\n3\nabc\n\n");

    rlcf = ngx_http_get_module_loc_conf(r, ngx_http_ssdb_module);

    b = ngx_create_temp_buf(r->pool, str.len);
    if(b == NULL) {
        return NGX_ERROR;
    }

    if(rlcf->queries) {}

    b->pos = str.data;
    b->last = b->pos + str.len;
    b->memory = 1;

    cl = ngx_alloc_chain_link(r->pool);
    if(cl == NULL) {
        return NGX_ERROR;
    }
    cl->buf = b;
    cl->next = NULL;
    r->upstream->request_bufs = cl;
    return NGX_OK;
}

static ngx_int_t
ngx_http_ssdb_reinit_request(ngx_http_request_t *r)
{
    return NGX_OK;    
}

static ngx_int_t
ngx_http_ssdb_process_header(ngx_http_request_t *r)
{
    ngx_http_upstream_t         *u;
	u_char 						*p;
    ngx_http_ssdb_ctx_t        *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_ssdb_module);

    u = r->upstream;

	for(p = u->buffer.pos; p < u->buffer.last; p++) {
		if(*p == LF) {
			goto found;
		}
	}
	return NGX_AGAIN;

found:
    ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0, "process header");
    ctx->filter = ngx_http_ssdb_process_reply;
	u->headers_in.status_n = NGX_HTTP_OK;
	u->state->status = NGX_HTTP_OK;
    return NGX_OK;
}

static ngx_int_t
ngx_http_ssdb_filter_init(void *data)
{
    return NGX_OK;
}

static ngx_int_t
ngx_http_ssdb_filter(void *data, ssize_t bytes)
{
	ngx_http_ssdb_ctx_t *ctx = data;
    ngx_log_error(NGX_LOG_NOTICE, ctx->request->connection->log, 0, "ssdb filter");
	return ctx->filter(ctx, bytes);
}

static void
ngx_http_ssdb_abort_request(ngx_http_request_t *r)
{
}

static void
ngx_http_ssdb_finalize_request(ngx_http_request_t *r, ngx_int_t rc)
{
}

