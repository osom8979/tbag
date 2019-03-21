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
#include <libtbag/mq/nng-1.1.1/nng.h>

#include <cstdbool>
#include <cstddef>
#include <cstdint>

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

TBAG_API int nng_pipe_notify(nng_socket, int, nng_pipe_cb, void *);
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

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGBYPASS_HPP__

