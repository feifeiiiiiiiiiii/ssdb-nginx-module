#include "ngx_http_ssdb_response.h"

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

%%{
    machine ssdb_response;

    CR = "\r";
    LF = "\n";
    CRLF = CR LF;

    action test_len {
        ctx->chunk_read++ < ctx->chunk_length
    }

    action finalize {
        done = 1;
    }

    action start_reading_chunk_size {
        ctx->chunk_length = 0;
        ctx->chunk_read = 0;
    }

    action read_size {
        ctx->chunk_length *= 10;
        ctx->chunk_length += *p - '0';
    }

    action check_data_complete {
        ctx->chunk_read == ctx->chunk_length
    }

    chunk_length = ([1-9] digit*) >start_reading_chunk_size $read_size;
    chunk_data_octet = any when test_len;
    chunk_data = chunk_data_octet+;

    main := (chunk_length (LF|CRLF) chunk_data (LF|CRLF))+ LF @finalize
            ;
}%%

%% write data;

ngx_int_t
ngx_http_ssdb_process_reply(ngx_http_ssdb_ctx_t *ctx, ssize_t bytes)
{
	int cs;
	ngx_buf_t 					*b;
	ngx_http_upstream_t 		*u;
	signed char 				*p;
	signed char 				*pe;
	signed char              	*orig_p;
	ssize_t                   	 orig_len;
	ngx_str_t 					 buf;

	ngx_chain_t 				*cl = NULL;
	ngx_chain_t 				**ll = NULL;
	int done = 0;

	u = ctx->request->upstream;
	b = &u->buffer;

	if(ctx->state == NGX_ERROR) {
		%% write init;
		ctx->state = cs;
	} else {
		cs = ctx->state;
	}

	orig_p = (signed char *)b->last;
	orig_len = bytes;

	p = (signed char *) b->last;
	pe = (signed char *) b->last + bytes;

	%% write exec;

	if (!done && cs == ssdb_response_error) {
		if(cl) {
			cl->buf->last = cl->buf->pos;
			cl = NULL;
			*ll = NULL;
		}
		buf.data = b->pos;
		buf.len = b->last - b->pos + bytes;
		ngx_log_error(NGX_LOG_ERR, ctx->request->connection->log,
										0, "ssdb server returned extra bytes: \"%V\" (len %z)",
										&buf, buf.len);
		u->length = 0;
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	if (cl == NULL) {
		for (cl = u->out_bufs, ll = &u->out_bufs; cl; cl = cl->next) {
			ll = &cl->next;
		}

		cl = ngx_chain_get_free_buf(ctx->request->pool, &u->free_bufs);
		if (cl == NULL) {
			u->length = 0;
			return NGX_ERROR;
		}

		cl->buf->flush = 1;
		cl->buf->memory = 1;

		*ll = cl;

		cl->buf->pos = b->last;
		cl->buf->last = (u_char *) p;
		cl->buf->tag = u->output.tag;

	} else {
		cl->buf->last = (u_char *) p;
	}

	bytes -= (ssize_t)((u_char *)p - b->last);
	b->last = (u_char *)p;

	if (done) {
		if(cs == ssdb_response_error) {
			buf.data = (u_char *)p;
			buf.len = orig_p - p + orig_len;
			if(cl) {
				cl->buf->last = cl->buf->pos;
				cl = NULL;
				*ll = NULL;
			}
			u->length = 0;
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		} else {
#if defined(nginx_version) && nginx_version >= 1001004
            u->keepalive = 1;
#endif
		}
		u->length = 0;
		return NGX_OK;
	}

	if(ssdb_response_first_final) {}
	if(ssdb_response_en_main) {}

    return NGX_OK;
}


