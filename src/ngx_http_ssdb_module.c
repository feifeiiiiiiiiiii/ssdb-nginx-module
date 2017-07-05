/*
 * Copyright (C) Yichun Zhang (agentzh)
 */

#ifndef DDEBUG
#define DDEBUG 0
#endif

#include "ddebug.h"
#include "ngx_http_ssdb_module.h"
#include "ngx_http_ssdb_handler.h"

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

static void *ngx_http_ssdb_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssdb_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_http_ssdb_pass(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_ssdb_query(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


static ngx_command_t ngx_http_ssdb_commands[] = {
    { ngx_string("ssdb_pass"),
      NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
      ngx_http_ssdb_pass,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },
    { ngx_string("ssdb_query"),
      NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_1MORE,
      ngx_http_ssdb_query,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    ngx_null_command
};

static ngx_http_module_t ngx_http_ssdb_module_ctx = {
    NULL,
    NULL,

    NULL,
    NULL,
    
    NULL,
    NULL,

    ngx_http_ssdb_create_loc_conf,
    ngx_http_ssdb_merge_loc_conf
};

ngx_module_t ngx_http_ssdb_module = {
    NGX_MODULE_V1,
    &ngx_http_ssdb_module_ctx,
    ngx_http_ssdb_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};

static void *
ngx_http_ssdb_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_ssdb_loc_conf_t *conf;
	conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssdb_loc_conf_t));

	if (conf == NULL) {
		return NULL;
	}

	conf->upstream.local = NGX_CONF_UNSET_PTR;
	conf->upstream.next_upstream_tries = NGX_CONF_UNSET_UINT;
	conf->upstream.connect_timeout = NGX_CONF_UNSET_MSEC;
	conf->upstream.send_timeout = NGX_CONF_UNSET_MSEC;
	conf->upstream.read_timeout = NGX_CONF_UNSET_MSEC;
	conf->upstream.next_upstream_timeout = NGX_CONF_UNSET_MSEC;

	conf->upstream.buffer_size = NGX_CONF_UNSET_SIZE;

	/* the hardcoded values */
	conf->upstream.cyclic_temp_file = 0;
	conf->upstream.buffering = 0;
	conf->upstream.ignore_client_abort = 0;
	conf->upstream.send_lowat = 0;
	conf->upstream.bufs.num = 0;
	conf->upstream.busy_buffers_size = 0;
	conf->upstream.max_temp_file_size = 0;
	conf->upstream.temp_file_write_size = 0;
	conf->upstream.intercept_errors = 1;
	conf->upstream.intercept_404 = 1;
	conf->upstream.pass_request_headers = 0;
	conf->upstream.pass_request_body = 0;
	return conf;
}

static char *
ngx_http_ssdb_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_ssdb_loc_conf_t *prev = parent;
	ngx_http_ssdb_loc_conf_t *conf = child;

	ngx_conf_merge_ptr_value(conf->upstream.local,
			prev->upstream.local, NULL);

	ngx_conf_merge_uint_value(conf->upstream.next_upstream_tries,
			prev->upstream.next_upstream_tries, 0);

	ngx_conf_merge_msec_value(conf->upstream.connect_timeout,
			prev->upstream.connect_timeout, 60000);

	ngx_conf_merge_msec_value(conf->upstream.send_timeout,
			prev->upstream.send_timeout, 60000);

	ngx_conf_merge_msec_value(conf->upstream.read_timeout,
			prev->upstream.read_timeout, 60000);

	ngx_conf_merge_msec_value(conf->upstream.next_upstream_timeout,
			prev->upstream.next_upstream_timeout, 0);

	ngx_conf_merge_size_value(conf->upstream.buffer_size,
			prev->upstream.buffer_size,
			(size_t) ngx_pagesize);

	ngx_conf_merge_bitmask_value(conf->upstream.next_upstream,
			prev->upstream.next_upstream,
			(NGX_CONF_BITMASK_SET
			 |NGX_HTTP_UPSTREAM_FT_ERROR
			 |NGX_HTTP_UPSTREAM_FT_TIMEOUT));

	if (conf->upstream.next_upstream & NGX_HTTP_UPSTREAM_FT_OFF) {
		conf->upstream.next_upstream = NGX_CONF_BITMASK_SET
			|NGX_HTTP_UPSTREAM_FT_OFF;
	}

	if (conf->upstream.upstream == NULL) {
		conf->upstream.upstream = prev->upstream.upstream;
	}
	return NGX_CONF_OK;
}

static char *
ngx_http_ssdb_pass(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	ngx_http_ssdb_loc_conf_t 	*rlcf = conf;
	ngx_str_t                 	*value;
	ngx_http_core_loc_conf_t  	*clcf;
	ngx_url_t                  	 url;

	if(rlcf->upstream.upstream) {
		return "is duplicate";
	}

	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

	clcf->handler = ngx_http_ssdb_handler;

	if (clcf->name.data[clcf->name.len - 1] == '/') {
		clcf->auto_redirect = 1;
	}

	value = cf->args->elts;

	ngx_memzero(&url, sizeof(ngx_url_t));
	url.url = value[1];
	url.no_resolve = 1;

	rlcf->upstream.upstream = ngx_http_upstream_add(cf, &url, 0);
	if (rlcf->upstream.upstream == NULL) {
		return NGX_CONF_ERROR;
	}

	return NGX_CONF_OK;
}

static char *
ngx_http_ssdb_query(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	
	ngx_http_ssdb_loc_conf_t *rlcf = conf;
	ngx_str_t 							 *value;

	ngx_http_compile_complex_value_t ccv;

	value = cf->args->elts;

	rlcf->complex_query = ngx_palloc(cf->pool, sizeof(ngx_http_complex_value_t));
	
	if (rlcf->complex_query == NULL) {
		return NGX_CONF_ERROR;
	}

	ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

	ccv.cf = cf;
	ccv.value = &value[1];
	ccv.complex_value = rlcf->complex_query;

	if(ngx_http_compile_complex_value(&ccv) != NGX_OK) {
		return NGX_CONF_ERROR;
	}


	return NGX_CONF_OK;
}

