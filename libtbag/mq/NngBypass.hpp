/**
 * @file   NngBypass.hpp
 * @brief  NngBypass class prototype.
 * @author zer0
 * @date   2019-03-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGBYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGBYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#define TBAG_DISABLE_NNG_TLS

#if !defined(__TBAG_MQ_NNG_INCLUDED)
# include <libtbag/mq/nng/nng.h>
// nng-protocol
# include <libtbag/mq/nng/protocol/bus0/bus.h>
# ifndef nng_pair_open
#  define nng_pair_open // Don't use this macro.
# endif
# ifndef nng_pair_open_raw
#  define nng_pair_open_raw // Don't use this macro.
# endif
# include <libtbag/mq/nng/protocol/pair0/pair.h>
# include <libtbag/mq/nng/protocol/pair1/pair.h>
# include <libtbag/mq/nng/protocol/pipeline0/pull.h>
# include <libtbag/mq/nng/protocol/pipeline0/push.h>
# include <libtbag/mq/nng/protocol/pubsub0/pub.h>
# include <libtbag/mq/nng/protocol/pubsub0/sub.h>
# include <libtbag/mq/nng/protocol/reqrep0/rep.h>
# include <libtbag/mq/nng/protocol/reqrep0/req.h>
# include <libtbag/mq/nng/protocol/survey0/respond.h>
# include <libtbag/mq/nng/protocol/survey0/survey.h>
// nng-supplemental
# include <libtbag/mq/nng/supplemental/http/http.h>
# if !defined(TBAG_DISABLE_NNG_TLS)
# include <libtbag/mq/nng/supplemental/tls/tls.h>
# endif
# include <libtbag/mq/nng/supplemental/util/options.h>
# include <libtbag/mq/nng/supplemental/util/platform.h>
// nng-transport
# include <libtbag/mq/nng/transport/inproc/inproc.h>
# include <libtbag/mq/nng/transport/ipc/ipc.h>
# include <libtbag/mq/nng/transport/tcp/tcp.h>
# include <libtbag/mq/nng/transport/ws/websocket.h>
#endif  // #if !defined(__TBAG_MQ_NNG_INCLUDED)

#include <cstdbool>
#include <cstddef>
#include <cstdint>

#include <libtbag/Err.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

TBAG_API void nng_fini();
TBAG_API int  nng_close(nng_socket);
TBAG_API int  nng_socket_id(nng_socket);
TBAG_API void nng_closeall();

TBAG_API int nng_setopt(nng_socket, char const *, void const *, size_t);
TBAG_API int nng_setopt_bool(nng_socket, char const *, bool);
TBAG_API int nng_setopt_int(nng_socket, char const *, int);
TBAG_API int nng_setopt_ms(nng_socket, char const *, nng_duration);
TBAG_API int nng_setopt_size(nng_socket, char const *, size_t);
TBAG_API int nng_setopt_uint64(nng_socket, char const *, uint64_t);
TBAG_API int nng_setopt_string(nng_socket, char const *, char const *);
TBAG_API int nng_setopt_ptr(nng_socket, char const *, void *);

TBAG_API int nng_getopt(nng_socket, char const *, void *, size_t *);
TBAG_API int nng_getopt_bool(nng_socket, char const *, bool *);
TBAG_API int nng_getopt_int(nng_socket, char const *, int *);
TBAG_API int nng_getopt_ms(nng_socket, char const *, nng_duration *);
TBAG_API int nng_getopt_size(nng_socket, char const *, size_t *);
TBAG_API int nng_getopt_uint64(nng_socket, char const *, uint64_t *);
TBAG_API int nng_getopt_ptr(nng_socket, char const *, void **);

TBAG_API int nng_pipe_notify(nng_socket, nng_pipe_ev, nng_pipe_cb, void *);
TBAG_API int nng_getopt_string(nng_socket, char const *, char **);
TBAG_API int nng_listen(nng_socket, char const *, nng_listener *, int);
TBAG_API int nng_dial(nng_socket, char const *, nng_dialer *, int);
TBAG_API int nng_dialer_create(nng_dialer *, nng_socket, char const *);
TBAG_API int nng_listener_create(nng_listener *, nng_socket, char const *);
TBAG_API int nng_dialer_start(nng_dialer, int);
TBAG_API int nng_listener_start(nng_listener, int);
TBAG_API int nng_dialer_close(nng_dialer);
TBAG_API int nng_listener_close(nng_listener);
TBAG_API int nng_dialer_id(nng_dialer);
TBAG_API int nng_listener_id(nng_listener);

TBAG_API int nng_dialer_setopt(nng_dialer, char const *, void const *, size_t);
TBAG_API int nng_dialer_setopt_bool(nng_dialer, char const *, bool);
TBAG_API int nng_dialer_setopt_int(nng_dialer, char const *, int);
TBAG_API int nng_dialer_setopt_ms(nng_dialer, char const *, nng_duration);
TBAG_API int nng_dialer_setopt_size(nng_dialer, char const *, size_t);
TBAG_API int nng_dialer_setopt_uint64(nng_dialer, char const *, uint64_t);
TBAG_API int nng_dialer_setopt_ptr(nng_dialer, char const *, void *);
TBAG_API int nng_dialer_setopt_string(nng_dialer, char const *, char const *);

TBAG_API int nng_dialer_getopt(nng_dialer, char const *, void *, size_t *);
TBAG_API int nng_dialer_getopt_bool(nng_dialer, char const *, bool *);
TBAG_API int nng_dialer_getopt_int(nng_dialer, char const *, int *);
TBAG_API int nng_dialer_getopt_ms(nng_dialer, char const *, nng_duration *);
TBAG_API int nng_dialer_getopt_size(nng_dialer, char const *, size_t *);
TBAG_API int nng_dialer_getopt_sockaddr(nng_dialer, char const *, nng_sockaddr *);
TBAG_API int nng_dialer_getopt_uint64(nng_dialer, char const *, uint64_t *);
TBAG_API int nng_dialer_getopt_ptr(nng_dialer, char const *, void **);
TBAG_API int nng_dialer_getopt_string(nng_dialer, char const *, char **);

TBAG_API int nng_listener_setopt(nng_listener, char const *, void const *, size_t);
TBAG_API int nng_listener_setopt_bool(nng_listener, char const *, bool);
TBAG_API int nng_listener_setopt_int(nng_listener, char const *, int);
TBAG_API int nng_listener_setopt_ms(nng_listener, char const *, nng_duration);
TBAG_API int nng_listener_setopt_size(nng_listener, char const *, size_t);
TBAG_API int nng_listener_setopt_uint64(nng_listener, char const *, uint64_t);
TBAG_API int nng_listener_setopt_ptr(nng_listener, char const *, void *);
TBAG_API int nng_listener_setopt_string(nng_listener, char const *, char const *);

TBAG_API int nng_listener_getopt(nng_listener, char const *, void *, size_t *);
TBAG_API int nng_listener_getopt_bool(nng_listener, char const *, bool *);
TBAG_API int nng_listener_getopt_int(nng_listener, char const *, int *);
TBAG_API int nng_listener_getopt_ms(nng_listener, char const *, nng_duration *);
TBAG_API int nng_listener_getopt_size(nng_listener, char const *, size_t *);
TBAG_API int nng_listener_getopt_sockaddr(nng_listener, char const *, nng_sockaddr *);
TBAG_API int nng_listener_getopt_uint64(nng_listener, char const *, uint64_t *);
TBAG_API int nng_listener_getopt_ptr(nng_listener, char const *, void **);
TBAG_API int nng_listener_getopt_string(nng_listener, char const *, char **);

TBAG_API char const * nng_strerror(int);
TBAG_API int  nng_send(nng_socket, void *, size_t, int);
TBAG_API int  nng_recv(nng_socket, void *, size_t *, int);
TBAG_API int  nng_sendmsg(nng_socket, nng_msg *, int);
TBAG_API int  nng_recvmsg(nng_socket, nng_msg **, int);
TBAG_API void nng_send_aio(nng_socket, nng_aio *);
TBAG_API void nng_recv_aio(nng_socket, nng_aio *);
TBAG_API int  nng_ctx_open(nng_ctx *, nng_socket);
TBAG_API int  nng_ctx_close(nng_ctx);
TBAG_API int  nng_ctx_id(nng_ctx);
TBAG_API void nng_ctx_recv(nng_ctx, nng_aio *);
TBAG_API void nng_ctx_send(nng_ctx, nng_aio *);

TBAG_API int nng_ctx_getopt(nng_ctx, char const *, void *, size_t *);
TBAG_API int nng_ctx_getopt_bool(nng_ctx, char const *, bool *);
TBAG_API int nng_ctx_getopt_int(nng_ctx, char const *, int *);
TBAG_API int nng_ctx_getopt_ms(nng_ctx, char const *, nng_duration *);
TBAG_API int nng_ctx_getopt_size(nng_ctx, char const *, size_t *);

TBAG_API int nng_ctx_setopt(nng_ctx, char const *, void const *, size_t);
TBAG_API int nng_ctx_setopt_bool(nng_ctx, char const *, bool);
TBAG_API int nng_ctx_setopt_int(nng_ctx, char const *, int);
TBAG_API int nng_ctx_setopt_ms(nng_ctx, char const *, nng_duration);
TBAG_API int nng_ctx_setopt_size(nng_ctx, char const *, size_t);

TBAG_API void * nng_alloc(size_t);
TBAG_API void   nng_free(void *, size_t);
TBAG_API char * nng_strdup(char const *);
TBAG_API void   nng_strfree(char *);
TBAG_API int    nng_aio_alloc(nng_aio **, void (*)(void *), void *);
TBAG_API void   nng_aio_free(nng_aio *);
TBAG_API void   nng_aio_stop(nng_aio *);
TBAG_API int    nng_aio_result(nng_aio *);
TBAG_API size_t nng_aio_count(nng_aio *);
TBAG_API void   nng_aio_cancel(nng_aio *);

TBAG_API void      nng_aio_abort(nng_aio *, int);
TBAG_API void      nng_aio_wait(nng_aio *);
TBAG_API void      nng_aio_set_msg(nng_aio *, nng_msg *);
TBAG_API nng_msg * nng_aio_get_msg(nng_aio *);
TBAG_API int       nng_aio_set_input(nng_aio *, unsigned, void *);
TBAG_API void *    nng_aio_get_input(nng_aio *, unsigned);
TBAG_API int       nng_aio_set_output(nng_aio *, unsigned, void *);
TBAG_API void *    nng_aio_get_output(nng_aio *, unsigned);
TBAG_API void      nng_aio_set_timeout(nng_aio *, nng_duration);
TBAG_API int       nng_aio_set_iov(nng_aio *, unsigned, const nng_iov *);
TBAG_API bool      nng_aio_begin(nng_aio *);
TBAG_API void      nng_aio_finish(nng_aio *, int);
TBAG_API void      nng_aio_defer(nng_aio *, nng_aio_cancelfn, void *);

TBAG_API void nng_sleep_aio(nng_duration, nng_aio *);

TBAG_API int      nng_msg_alloc(nng_msg **, size_t);
TBAG_API void     nng_msg_free(nng_msg *);
TBAG_API int      nng_msg_realloc(nng_msg *, size_t);
TBAG_API void *   nng_msg_header(nng_msg *);
TBAG_API size_t   nng_msg_header_len(const nng_msg *);
TBAG_API void *   nng_msg_body(nng_msg *);
TBAG_API size_t   nng_msg_len(const nng_msg *);
TBAG_API int      nng_msg_append(nng_msg *, void const *, size_t);
TBAG_API int      nng_msg_insert(nng_msg *, void const *, size_t);
TBAG_API int      nng_msg_trim(nng_msg *, size_t);
TBAG_API int      nng_msg_chop(nng_msg *, size_t);
TBAG_API int      nng_msg_header_append(nng_msg *, void const *, size_t);
TBAG_API int      nng_msg_header_insert(nng_msg *, void const *, size_t);
TBAG_API int      nng_msg_header_trim(nng_msg *, size_t);
TBAG_API int      nng_msg_header_chop(nng_msg *, size_t);
TBAG_API int      nng_msg_header_append_u16(nng_msg *, uint16_t);
TBAG_API int      nng_msg_header_append_u32(nng_msg *, uint32_t);
TBAG_API int      nng_msg_header_append_u64(nng_msg *, uint64_t);
TBAG_API int      nng_msg_header_insert_u16(nng_msg *, uint16_t);
TBAG_API int      nng_msg_header_insert_u32(nng_msg *, uint32_t);
TBAG_API int      nng_msg_header_insert_u64(nng_msg *, uint64_t);
TBAG_API int      nng_msg_header_chop_u16(nng_msg *, uint16_t *);
TBAG_API int      nng_msg_header_chop_u32(nng_msg *, uint32_t *);
TBAG_API int      nng_msg_header_chop_u64(nng_msg *, uint64_t *);
TBAG_API int      nng_msg_header_trim_u16(nng_msg *, uint16_t *);
TBAG_API int      nng_msg_header_trim_u32(nng_msg *, uint32_t *);
TBAG_API int      nng_msg_header_trim_u64(nng_msg *, uint64_t *);
TBAG_API int      nng_msg_append_u16(nng_msg *, uint16_t);
TBAG_API int      nng_msg_append_u32(nng_msg *, uint32_t);
TBAG_API int      nng_msg_append_u64(nng_msg *, uint64_t);
TBAG_API int      nng_msg_insert_u16(nng_msg *, uint16_t);
TBAG_API int      nng_msg_insert_u32(nng_msg *, uint32_t);
TBAG_API int      nng_msg_insert_u64(nng_msg *, uint64_t);
TBAG_API int      nng_msg_chop_u16(nng_msg *, uint16_t *);
TBAG_API int      nng_msg_chop_u32(nng_msg *, uint32_t *);
TBAG_API int      nng_msg_chop_u64(nng_msg *, uint64_t *);
TBAG_API int      nng_msg_trim_u16(nng_msg *, uint16_t *);
TBAG_API int      nng_msg_trim_u32(nng_msg *, uint32_t *);
TBAG_API int      nng_msg_trim_u64(nng_msg *, uint64_t *);
TBAG_API int      nng_msg_dup(nng_msg **, const nng_msg *);
TBAG_API void     nng_msg_clear(nng_msg *);
TBAG_API void     nng_msg_header_clear(nng_msg *);
TBAG_API void     nng_msg_set_pipe(nng_msg *, nng_pipe);
TBAG_API nng_pipe nng_msg_get_pipe(const nng_msg *);
TBAG_API int      nng_msg_getopt(nng_msg *, int, void *, size_t *);

TBAG_API int          nng_pipe_getopt(nng_pipe, char const *, void *, size_t *);
TBAG_API int          nng_pipe_getopt_bool(nng_pipe, char const *, bool *);
TBAG_API int          nng_pipe_getopt_int(nng_pipe, char const *, int *);
TBAG_API int          nng_pipe_getopt_ms(nng_pipe, char const *, nng_duration *);
TBAG_API int          nng_pipe_getopt_size(nng_pipe, char const *, size_t *);
TBAG_API int          nng_pipe_getopt_sockaddr(nng_pipe, char const *, nng_sockaddr *);
TBAG_API int          nng_pipe_getopt_uint64(nng_pipe, char const *, uint64_t *);
TBAG_API int          nng_pipe_getopt_ptr(nng_pipe, char const *, void **);
TBAG_API int          nng_pipe_getopt_string(nng_pipe, char const *, char **);
TBAG_API int          nng_pipe_close(nng_pipe);
TBAG_API int          nng_pipe_id(nng_pipe);
TBAG_API nng_socket   nng_pipe_socket(nng_pipe);
TBAG_API nng_dialer   nng_pipe_dialer(nng_pipe);
TBAG_API nng_listener nng_pipe_listener(nng_pipe);

TBAG_API int          nng_stats_get(nng_stat **);
TBAG_API void         nng_stats_free(nng_stat *);
TBAG_API void         nng_stats_dump(nng_stat *);
TBAG_API nng_stat *   nng_stat_next(nng_stat *);
TBAG_API nng_stat *   nng_stat_child(nng_stat *);
TBAG_API char const * nng_stat_name(nng_stat *);
TBAG_API int          nng_stat_type(nng_stat *);
TBAG_API int          nng_stat_unit(nng_stat *);
TBAG_API uint64_t     nng_stat_value(nng_stat *);
TBAG_API char const * nng_stat_string(nng_stat *);
TBAG_API char const * nng_stat_desc(nng_stat *);
TBAG_API uint64_t     nng_stat_timestamp(nng_stat *);
TBAG_API int          nng_device(nng_socket, nng_socket);

TBAG_API int  nng_url_parse(nng_url **, char const *);
TBAG_API void nng_url_free(nng_url *);
TBAG_API int  nng_url_clone(nng_url **, const nng_url *);

TBAG_API char const * nng_version();

# /************/
# /* protocol */
# /************/

TBAG_API int nng_bus0_open(nng_socket *);
TBAG_API int nng_bus0_open_raw(nng_socket *);

TBAG_API int nng_pair0_open(nng_socket *);
TBAG_API int nng_pair0_open_raw(nng_socket *);

TBAG_API int nng_pair1_open(nng_socket *);
TBAG_API int nng_pair1_open_raw(nng_socket *);

TBAG_API int nng_pull0_open(nng_socket *);
TBAG_API int nng_pull0_open_raw(nng_socket *);

TBAG_API int nng_push0_open(nng_socket *);
TBAG_API int nng_push0_open_raw(nng_socket *);

TBAG_API int nng_pub0_open(nng_socket *);
TBAG_API int nng_pub0_open_raw(nng_socket *);

TBAG_API int nng_sub0_open(nng_socket *);
TBAG_API int nng_sub0_open_raw(nng_socket *);

TBAG_API int nng_rep0_open(nng_socket *);
TBAG_API int nng_rep0_open_raw(nng_socket *);

TBAG_API int nng_req0_open(nng_socket *);
TBAG_API int nng_req0_open_raw(nng_socket *);

TBAG_API int nng_respondent0_open(nng_socket *);
TBAG_API int nng_respondent0_open_raw(nng_socket *);

TBAG_API int nng_surveyor0_open(nng_socket *);
TBAG_API int nng_surveyor0_open_raw(nng_socket *);

# /*********************/
# /* supplemental/http */
# /*********************/

TBAG_API int          nng_http_req_alloc(nng_http_req **, nng_url const *);
TBAG_API void         nng_http_req_free(nng_http_req *);
TBAG_API char const * nng_http_req_get_method(nng_http_req *);
TBAG_API char const * nng_http_req_get_version(nng_http_req *);
TBAG_API char const * nng_http_req_get_uri(nng_http_req *);
TBAG_API int          nng_http_req_set_header(nng_http_req *, char const *, char const *);
TBAG_API int          nng_http_req_add_header(nng_http_req *, char const *, char const *);
TBAG_API int          nng_http_req_del_header(nng_http_req *, char const *);
TBAG_API char const * nng_http_req_get_header(nng_http_req *, char const *);
TBAG_API int          nng_http_req_set_method(nng_http_req *, char const *);
TBAG_API int          nng_http_req_set_version(nng_http_req *, char const *);
TBAG_API int          nng_http_req_set_uri(nng_http_req *, char const *);
TBAG_API int          nng_http_req_set_data(nng_http_req *, void const *, size_t);
TBAG_API int          nng_http_req_copy_data(nng_http_req *, void const *, size_t);
TBAG_API void         nng_http_req_get_data(nng_http_req *, void **, size_t *);

TBAG_API int          nng_http_res_alloc(nng_http_res **);
TBAG_API int          nng_http_res_alloc_error(nng_http_res **, uint16_t);
TBAG_API void         nng_http_res_free(nng_http_res *);
TBAG_API uint16_t     nng_http_res_get_status(nng_http_res *);
TBAG_API int          nng_http_res_set_status(nng_http_res *, uint16_t);
TBAG_API char const * nng_http_res_get_reason(nng_http_res *);
TBAG_API int          nng_http_res_set_reason(nng_http_res *, char const *);
TBAG_API int          nng_http_res_set_header(nng_http_res *, char const *, char const *);
TBAG_API int          nng_http_res_add_header(nng_http_res *, char const *, char const *);
TBAG_API int          nng_http_res_del_header(nng_http_res *, char const *);
TBAG_API char const * nng_http_res_get_header(nng_http_res *, char const *);
TBAG_API int          nng_http_res_set_version(nng_http_res *, char const *);
TBAG_API char const * nng_http_res_get_version(nng_http_res *);
TBAG_API void         nng_http_res_get_data(nng_http_res *, void **, size_t *);
TBAG_API int          nng_http_res_set_data(nng_http_res *, void const *, size_t);
TBAG_API int          nng_http_res_copy_data(nng_http_res *, void const *, size_t);

TBAG_API void nng_http_conn_close(nng_http_conn *);
TBAG_API void nng_http_conn_read(nng_http_conn *, nng_aio *);
TBAG_API void nng_http_conn_read_all(nng_http_conn *, nng_aio *);
TBAG_API void nng_http_conn_write(nng_http_conn *, nng_aio *);
TBAG_API void nng_http_conn_write_all(nng_http_conn *, nng_aio *);
TBAG_API void nng_http_conn_write_req(nng_http_conn *, nng_http_req *, nng_aio *);
TBAG_API void nng_http_conn_write_res(nng_http_conn *, nng_http_res *, nng_aio *);
TBAG_API void nng_http_conn_read_req(nng_http_conn *, nng_http_req *, nng_aio *);
TBAG_API void nng_http_conn_read_res(nng_http_conn *, nng_http_res *, nng_aio *);
TBAG_API void nng_http_req_reset(nng_http_req *);
TBAG_API void nng_http_res_reset(nng_http_res *);

TBAG_API int    nng_http_handler_alloc(nng_http_handler **, char const *, void (*)(nng_aio *));
TBAG_API void   nng_http_handler_free(nng_http_handler *);
TBAG_API int    nng_http_handler_alloc_file(nng_http_handler **, char const *, char const *);
TBAG_API int    nng_http_handler_alloc_static(nng_http_handler **, char const *, void const *, size_t, char const *);
TBAG_API int    nng_http_handler_alloc_redirect(nng_http_handler **, char const *, uint16_t, char const *);
TBAG_API int    nng_http_handler_alloc_directory(nng_http_handler **, char const *, char const *);
TBAG_API int    nng_http_handler_set_method(nng_http_handler *, char const *);
TBAG_API int    nng_http_handler_set_host(nng_http_handler *, char const *);
TBAG_API int    nng_http_handler_collect_body(nng_http_handler *, bool, size_t);
TBAG_API int    nng_http_handler_set_tree(nng_http_handler *);
TBAG_API int    nng_http_handler_set_data(nng_http_handler *, void *, void (*)(void *));
TBAG_API void * nng_http_handler_get_data(nng_http_handler *);

TBAG_API int  nng_http_server_hold(nng_http_server **, nng_url const *);
TBAG_API void nng_http_server_release(nng_http_server *);
TBAG_API int  nng_http_server_start(nng_http_server *);
TBAG_API void nng_http_server_stop(nng_http_server *);
TBAG_API int  nng_http_server_add_handler(nng_http_server *, nng_http_handler *);
TBAG_API int  nng_http_server_del_handler(nng_http_server *, nng_http_handler *);
TBAG_API int  nng_http_server_set_tls(nng_http_server *, struct nng_tls_config *);
TBAG_API int  nng_http_server_get_tls(nng_http_server *, struct nng_tls_config **);
TBAG_API int  nng_http_server_set_error_page(nng_http_server *, uint16_t, char const *);
TBAG_API int  nng_http_server_set_error_file(nng_http_server *, uint16_t, char const *);
TBAG_API int  nng_http_server_res_error(nng_http_server *, nng_http_res *);
TBAG_API int  nng_http_hijack(nng_http_conn *);

TBAG_API int  nng_http_client_alloc(nng_http_client **, nng_url const *);
TBAG_API void nng_http_client_free(nng_http_client *);
TBAG_API int  nng_http_client_set_tls(nng_http_client *, struct nng_tls_config *);
TBAG_API int  nng_http_client_get_tls(nng_http_client *, struct nng_tls_config **);
TBAG_API void nng_http_client_connect(nng_http_client *, nng_aio *);
TBAG_API void nng_http_conn_transact(nng_http_conn *, nng_http_req *, nng_http_res *, nng_aio *);
TBAG_API void nng_http_client_transact(nng_http_client *, nng_http_req *, nng_http_res *, nng_aio *);

# /********************/
# /* supplemental/tls */
# /********************/

#if !defined(TBAG_DISABLE_NNG_TLS)
TBAG_API int  nng_tls_config_alloc(nng_tls_config **, nng_tls_mode);
TBAG_API void nng_tls_config_hold(nng_tls_config *);
TBAG_API void nng_tls_config_free(nng_tls_config *);
TBAG_API int  nng_tls_config_server_name(nng_tls_config *, const char *);
TBAG_API int  nng_tls_config_ca_chain(nng_tls_config *, const char *, const char *);
TBAG_API int  nng_tls_config_own_cert(nng_tls_config *, const char *, const char *, const char *);
TBAG_API int  nng_tls_config_key(nng_tls_config *, const uint8_t *, size_t);
TBAG_API int  nng_tls_config_pass(nng_tls_config *, const char *);
TBAG_API int  nng_tls_config_auth_mode(nng_tls_config *, nng_tls_auth_mode);
TBAG_API int  nng_tls_config_ca_file(nng_tls_config *, const char *);
TBAG_API int  nng_tls_config_cert_key_file(nng_tls_config *, const char *, const char *);
#endif

# /*********************/
# /* supplemental/util */
# /*********************/

TBAG_API int nng_opts_parse(int argc, char * const * argv,
                            nng_optspec const * opts,
                            int * val,
                            char ** optarg,
                            int * optidx);

TBAG_API nng_time nng_clock();
TBAG_API void     nng_msleep(nng_duration);
TBAG_API int      nng_thread_create(nng_thread **, void (*)(void *), void *);
TBAG_API void     nng_thread_destroy(nng_thread *);
TBAG_API int      nng_mtx_alloc(nng_mtx **);
TBAG_API void     nng_mtx_free(nng_mtx *);
TBAG_API void     nng_mtx_lock(nng_mtx *);
TBAG_API void     nng_mtx_unlock(nng_mtx *);
TBAG_API int      nng_cv_alloc(nng_cv **, nng_mtx *);
TBAG_API void     nng_cv_free(nng_cv *);
TBAG_API void     nng_cv_wait(nng_cv *);
TBAG_API int      nng_cv_until(nng_cv *, nng_time);
TBAG_API void     nng_cv_wake(nng_cv *);
TBAG_API void     nng_cv_wake1(nng_cv *);
TBAG_API uint32_t nng_random();

# /*************/
# /* transport */
# /*************/

//TBAG_API int nng_inproc_register();
//TBAG_API int nng_ipc_register();
//TBAG_API int nng_tcp_register();
//TBAG_API int nng_ws_register();
//TBAG_API int nng_wss_register();

# /*************/
# /* extension */
# /*************/

TBAG_API Err nng_code_err(int code);

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGBYPASS_HPP__

