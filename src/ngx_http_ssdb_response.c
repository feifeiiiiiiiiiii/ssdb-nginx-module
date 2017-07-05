
#line 1 "ngx_http_ssdb_response.rl"
#include "ngx_http_ssdb_response.h"

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>


#line 43 "ngx_http_ssdb_response.rl"



#line 16 "ngx_http_ssdb_response.c"
static const int ssdb_response_start = 1;
static const int ssdb_response_first_final = 13;
static const int ssdb_response_error = 0;

static const int ssdb_response_en_main = 1;


#line 46 "ngx_http_ssdb_response.rl"

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
		
#line 47 "ngx_http_ssdb_response.c"
	{
	cs = ssdb_response_start;
	}

#line 68 "ngx_http_ssdb_response.rl"
		ctx->state = cs;
	} else {
		cs = ctx->state;
	}

	orig_p = (signed char *)b->last;
	orig_len = bytes;

	p = (signed char *) b->last;
	pe = (signed char *) b->last + bytes;

	
#line 65 "ngx_http_ssdb_response.c"
	{
	short _widec;
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 23 "ngx_http_ssdb_response.rl"
	{
        ctx->chunk_length = 0;
        ctx->chunk_read = 0;
    }
#line 28 "ngx_http_ssdb_response.rl"
	{
        ctx->chunk_length *= 10;
        ctx->chunk_length += *p - '0';
    }
	goto st2;
tr4:
#line 28 "ngx_http_ssdb_response.rl"
	{
        ctx->chunk_length *= 10;
        ctx->chunk_length += *p - '0';
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 102 "ngx_http_ssdb_response.c"
	switch( (*p) ) {
		case 10: goto st3;
		case 13: goto st12;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	if ( 384 <= _widec && _widec <= 639 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto st5;
		case 269: goto st6;
		case 522: goto st7;
	}
	if ( 384 <= _widec && _widec <= 639 )
		goto st4;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 10 )
		goto tr9;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr0;
	goto st0;
tr9:
#line 19 "ngx_http_ssdb_response.rl"
	{
        done = 1;
    }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 162 "ngx_http_ssdb_response.c"
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 10 )
		goto st5;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr10;
		case 269: goto st6;
		case 522: goto tr11;
	}
	if ( _widec < 384 ) {
		if ( 305 <= _widec && _widec <= 313 )
			goto tr0;
	} else if ( _widec > 560 ) {
		if ( _widec > 569 ) {
			if ( 570 <= _widec && _widec <= 639 )
				goto st4;
		} else if ( _widec >= 561 )
			goto tr12;
	} else
		goto st4;
	goto st0;
tr10:
#line 19 "ngx_http_ssdb_response.rl"
	{
        done = 1;
    }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 209 "ngx_http_ssdb_response.c"
	if ( (*p) == 10 )
		goto tr9;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr0;
	goto st0;
tr11:
#line 19 "ngx_http_ssdb_response.rl"
	{
        done = 1;
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 225 "ngx_http_ssdb_response.c"
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr10;
		case 269: goto st6;
		case 522: goto tr11;
	}
	if ( _widec < 384 ) {
		if ( 305 <= _widec && _widec <= 313 )
			goto tr0;
	} else if ( _widec > 560 ) {
		if ( _widec > 569 ) {
			if ( 570 <= _widec && _widec <= 639 )
				goto st4;
		} else if ( _widec >= 561 )
			goto tr12;
	} else
		goto st4;
	goto st0;
tr12:
#line 23 "ngx_http_ssdb_response.rl"
	{
        ctx->chunk_length = 0;
        ctx->chunk_read = 0;
    }
#line 28 "ngx_http_ssdb_response.rl"
	{
        ctx->chunk_length *= 10;
        ctx->chunk_length += *p - '0';
    }
	goto st8;
tr16:
#line 28 "ngx_http_ssdb_response.rl"
	{
        ctx->chunk_length *= 10;
        ctx->chunk_length += *p - '0';
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 273 "ngx_http_ssdb_response.c"
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto st9;
		case 269: goto st10;
		case 522: goto st7;
		case 525: goto st11;
	}
	if ( _widec < 384 ) {
		if ( 304 <= _widec && _widec <= 313 )
			goto tr4;
	} else if ( _widec > 559 ) {
		if ( _widec > 569 ) {
			if ( 570 <= _widec && _widec <= 639 )
				goto st4;
		} else if ( _widec >= 560 )
			goto tr16;
	} else
		goto st4;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr9;
		case 522: goto tr17;
	}
	if ( _widec < 384 ) {
		if ( 305 <= _widec && _widec <= 313 )
			goto tr0;
	} else if ( _widec > 560 ) {
		if ( _widec > 569 ) {
			if ( 570 <= _widec && _widec <= 639 )
				goto st4;
		} else if ( _widec >= 561 )
			goto tr12;
	} else
		goto st4;
	goto st0;
tr17:
#line 19 "ngx_http_ssdb_response.rl"
	{
        done = 1;
    }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 336 "ngx_http_ssdb_response.c"
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto st5;
		case 269: goto st6;
		case 522: goto st7;
	}
	if ( 384 <= _widec && _widec <= 639 )
		goto st4;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 10 )
		goto st9;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	_widec = (*p);
	_widec = (short)(128 + ((*p) - -128));
	if ( 
#line 15 "ngx_http_ssdb_response.rl"

        ctx->chunk_read++ < ctx->chunk_length
     ) _widec += 256;
	switch( _widec ) {
		case 266: goto st9;
		case 269: goto st6;
		case 522: goto st7;
	}
	if ( 384 <= _widec && _widec <= 639 )
		goto st4;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 10 )
		goto st3;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 80 "ngx_http_ssdb_response.rl"

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


