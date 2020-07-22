/**
 * @file   NngBypass.cpp
 * @brief  NngBypass class implementation.
 * @author zer0
 * @date   2019-03-21
 */

// [WARNING] Don't change the order of including.
#include <nng/nng.h>

// nng-protocol
#include <nng/protocol/bus0/bus.h>
#ifndef nng_pair_open
#define nng_pair_open // Don't use this macro.
#endif
#ifndef nng_pair_open_raw
#define nng_pair_open_raw // Don't use this macro.
#endif
#include <nng/protocol/pair0/pair.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/protocol/pipeline0/pull.h>
#include <nng/protocol/pipeline0/push.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/survey0/respond.h>
#include <nng/protocol/survey0/survey.h>

// nng-supplemental
#include <nng/supplemental/http/http.h>
#include <nng/supplemental/tls/tls.h>
#include <nng/supplemental/util/options.h>
#include <nng/supplemental/util/platform.h>

// nng-transport
#include <nng/transport/inproc/inproc.h>
#include <nng/transport/ipc/ipc.h>
#include <nng/transport/tcp/tcp.h>
#include <nng/transport/ws/websocket.h>

#include <libtbag/mq/NngBypass.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

void nng_fini()
{
    ::nng_fini();
}

int nng_close(nng_socket v0)
{
    return ::nng_close(v0);
}

int nng_socket_id(nng_socket v0)
{
    return ::nng_socket_id(v0);
}

void nng_closeall()
{
    ::nng_closeall();
}

int nng_setopt(nng_socket v0, char const * v1, void const * v2, size_t v3)
{
    return ::nng_setopt(v0, v1, v2, v3);
}

int nng_setopt_bool(nng_socket v0, char const * v1, bool v2)
{
    return ::nng_setopt_bool(v0, v1, v2);
}

int nng_setopt_int(nng_socket v0, char const * v1, int v2)
{
    return ::nng_setopt_int(v0, v1, v2);
}

int nng_setopt_ms(nng_socket v0, char const * v1, nng_duration v2)
{
    return ::nng_setopt_ms(v0, v1, v2);
}

int nng_setopt_size(nng_socket v0, char const * v1, size_t v2)
{
    return ::nng_setopt_size(v0, v1, v2);
}

int nng_setopt_uint64(nng_socket v0, char const * v1, uint64_t v2)
{
    return ::nng_setopt_uint64(v0, v1, v2);
}

int nng_setopt_string(nng_socket v0, char const * v1, char const * v2)
{
    return ::nng_setopt_string(v0, v1, v2);
}

int nng_setopt_ptr(nng_socket v0, char const * v1, void * v2)
{
    return ::nng_setopt_ptr(v0, v1, v2);
}

int nng_getopt(nng_socket v0, char const * v1, void * v2, size_t * v3)
{
    return ::nng_getopt(v0, v1, v2, v3);
}

int nng_getopt_bool(nng_socket v0, char const * v1, bool * v2)
{
    return ::nng_getopt_bool(v0, v1, v2);
}

int nng_getopt_int(nng_socket v0, char const * v1, int * v2)
{
    return ::nng_getopt_int(v0, v1, v2);
}

int nng_getopt_ms(nng_socket v0, char const * v1, nng_duration * v2)
{
    return ::nng_getopt_ms(v0, v1, v2);
}

int nng_getopt_size(nng_socket v0, char const * v1, size_t * v2)
{
    return ::nng_getopt_size(v0, v1, v2);
}

int nng_getopt_uint64(nng_socket v0, char const * v1, uint64_t * v2)
{
    return ::nng_getopt_uint64(v0, v1, v2);
}

int nng_getopt_ptr(nng_socket v0, char const * v1, void ** v2)
{
    return ::nng_getopt_ptr(v0, v1, v2);
}

int nng_pipe_notify(nng_socket v0, nng_pipe_ev v1, nng_pipe_cb v2, void * v3)
{
    return ::nng_pipe_notify(v0, v1, v2, v3);
}

int nng_getopt_string(nng_socket v0, char const * v1, char ** v2)
{
    return ::nng_getopt_string(v0, v1, v2);
}

int nng_listen(nng_socket v0, char const * v1, nng_listener * v2, int v3)
{
    return ::nng_listen(v0, v1, v2, v3);
}

int nng_dial(nng_socket v0, char const * v1, nng_dialer * v2, int v3)
{
    return ::nng_dial(v0, v1, v2, v3);
}

int nng_dialer_create(nng_dialer * v0, nng_socket v1, char const * v2)
{
    return ::nng_dialer_create(v0, v1, v2);
}

int nng_listener_create(nng_listener * v0, nng_socket v1, char const * v2)
{
    return ::nng_listener_create(v0, v1, v2);
}

int nng_dialer_start(nng_dialer v0, int v1)
{
    return ::nng_dialer_start(v0, v1);
}

int nng_listener_start(nng_listener v0, int v1)
{
    return ::nng_listener_start(v0, v1);
}

int nng_dialer_close(nng_dialer v0)
{
    return ::nng_dialer_close(v0);
}

int nng_listener_close(nng_listener v0)
{
    return ::nng_listener_close(v0);
}

int nng_dialer_id(nng_dialer v0)
{
    return ::nng_dialer_id(v0);
}

int nng_listener_id(nng_listener v0)
{
    return ::nng_listener_id(v0);
}

int nng_dialer_setopt(nng_dialer v0, char const * v1, void const * v2, size_t v3)
{
    return ::nng_dialer_setopt(v0, v1, v2, v3);
}

int nng_dialer_setopt_bool(nng_dialer v0, char const * v1, bool v2)
{
    return ::nng_dialer_setopt_bool(v0, v1, v2);
}

int nng_dialer_setopt_int(nng_dialer v0, char const * v1, int v2)
{
    return ::nng_dialer_setopt_int(v0, v1, v2);
}

int nng_dialer_setopt_ms(nng_dialer v0, char const * v1, nng_duration v2)
{
    return ::nng_dialer_setopt_ms(v0, v1, v2);
}

int nng_dialer_setopt_size(nng_dialer v0, char const * v1, size_t v2)
{
    return ::nng_dialer_setopt_size(v0, v1, v2);
}

int nng_dialer_setopt_uint64(nng_dialer v0, char const * v1, uint64_t v2)
{
    return ::nng_dialer_setopt_uint64(v0, v1, v2);
}

int nng_dialer_setopt_ptr(nng_dialer v0, char const * v1, void * v2)
{
    return ::nng_dialer_setopt_ptr(v0, v1, v2);
}

int nng_dialer_setopt_string(nng_dialer v0, char const * v1, char const * v2)
{
    return ::nng_dialer_setopt_string(v0, v1, v2);
}

int nng_dialer_getopt(nng_dialer v0, char const * v1, void * v2, size_t * v3)
{
    return ::nng_dialer_getopt(v0, v1, v2, v3);
}

int nng_dialer_getopt_bool(nng_dialer v0, char const * v1, bool * v2)
{
    return ::nng_dialer_getopt_bool(v0, v1, v2);
}

int nng_dialer_getopt_int(nng_dialer v0, char const * v1, int * v2)
{
    return ::nng_dialer_getopt_int(v0, v1, v2);
}

int nng_dialer_getopt_ms(nng_dialer v0, char const * v1, nng_duration * v2)
{
    return ::nng_dialer_getopt_ms(v0, v1, v2);
}

int nng_dialer_getopt_size(nng_dialer v0, char const * v1, size_t * v2)
{
    return ::nng_dialer_getopt_size(v0, v1, v2);
}

int nng_dialer_getopt_sockaddr(nng_dialer v0, char const * v1, nng_sockaddr * v2)
{
    return ::nng_dialer_getopt_sockaddr(v0, v1, v2);
}

int nng_dialer_getopt_uint64(nng_dialer v0, char const * v1, uint64_t * v2)
{
    return ::nng_dialer_getopt_uint64(v0, v1, v2);
}

int nng_dialer_getopt_ptr(nng_dialer v0, char const * v1, void ** v2)
{
    return ::nng_dialer_getopt_ptr(v0, v1, v2);
}

int nng_dialer_getopt_string(nng_dialer v0, char const * v1, char ** v2)
{
    return ::nng_dialer_getopt_string(v0, v1, v2);
}

int nng_listener_setopt(nng_listener v0, char const * v1, void const * v2, size_t v3)
{
    return ::nng_listener_setopt(v0, v1, v2, v3);
}

int nng_listener_setopt_bool(nng_listener v0, char const * v1, bool v2)
{
    return ::nng_listener_setopt_bool(v0, v1, v2);
}

int nng_listener_setopt_int(nng_listener v0, char const * v1, int v2)
{
    return ::nng_listener_setopt_int(v0, v1, v2);
}

int nng_listener_setopt_ms(nng_listener v0, char const * v1, nng_duration v2)
{
    return ::nng_listener_setopt_ms(v0, v1, v2);
}

int nng_listener_setopt_size(nng_listener v0, char const * v1, size_t v2)
{
    return ::nng_listener_setopt_size(v0, v1, v2);
}

int nng_listener_setopt_uint64(nng_listener v0, char const * v1, uint64_t v2)
{
    return ::nng_listener_setopt_uint64(v0, v1, v2);
}

int nng_listener_setopt_ptr(nng_listener v0, char const * v1, void * v2)
{
    return ::nng_listener_setopt_ptr(v0, v1, v2);
}

int nng_listener_setopt_string(nng_listener v0, char const * v1, char const * v2)
{
    return ::nng_listener_setopt_string(v0, v1, v2);
}

int nng_listener_getopt(nng_listener v0, char const * v1, void * v2, size_t * v3)
{
    return ::nng_listener_getopt(v0, v1, v2, v3);
}

int nng_listener_getopt_bool(nng_listener v0, char const * v1, bool * v2)
{
    return ::nng_listener_getopt_bool(v0, v1, v2);
}

int nng_listener_getopt_int(nng_listener v0, char const * v1, int * v2)
{
    return ::nng_listener_getopt_int(v0, v1, v2);
}

int nng_listener_getopt_ms(nng_listener v0, char const * v1, nng_duration * v2)
{
    return ::nng_listener_getopt_ms(v0, v1, v2);
}

int nng_listener_getopt_size(nng_listener v0, char const * v1, size_t * v2)
{
    return ::nng_listener_getopt_size(v0, v1, v2);
}

int nng_listener_getopt_sockaddr(nng_listener v0, char const * v1, nng_sockaddr * v2)
{
    return ::nng_listener_getopt_sockaddr(v0, v1, v2);
}

int nng_listener_getopt_uint64(nng_listener v0, char const * v1, uint64_t * v2)
{
    return ::nng_listener_getopt_uint64(v0, v1, v2);
}

int nng_listener_getopt_ptr(nng_listener v0, char const * v1, void ** v2)
{
    return ::nng_listener_getopt_ptr(v0, v1, v2);
}

int nng_listener_getopt_string(nng_listener v0, char const * v1, char ** v2)
{
    return ::nng_listener_getopt_string(v0, v1, v2);
}

char const * nng_strerror(int v1)
{
    return ::nng_strerror(v1);
}

int nng_send(nng_socket v0, void * v1, size_t v2, int v3)
{
    return ::nng_send(v0, v1, v2, v3);
}

int nng_recv(nng_socket v0, void * v1, size_t * v2, int v3)
{
    return ::nng_recv(v0, v1, v2, v3);
}

int nng_sendmsg(nng_socket v0, nng_msg * v1, int v2)
{
    return ::nng_sendmsg(v0, v1, v2);
}

int nng_recvmsg(nng_socket v0, nng_msg ** v1, int v2)
{
    return ::nng_recvmsg(v0, v1, v2);
}

void nng_send_aio(nng_socket v0, nng_aio * v1)
{
    ::nng_send_aio(v0, v1);
}

void nng_recv_aio(nng_socket v0, nng_aio * v1)
{
    ::nng_recv_aio(v0, v1);
}

int nng_ctx_open(nng_ctx * v0, nng_socket v1)
{
    return ::nng_ctx_open(v0, v1);
}

int nng_ctx_close(nng_ctx v0)
{
    return ::nng_ctx_close(v0);
}

int nng_ctx_id(nng_ctx v0)
{
    return ::nng_ctx_id(v0);
}

void nng_ctx_recv(nng_ctx v0, nng_aio * v1)
{
    ::nng_ctx_recv(v0, v1);
}

void nng_ctx_send(nng_ctx v0, nng_aio * v1)
{
    ::nng_ctx_send(v0, v1);
}

int nng_ctx_getopt(nng_ctx v0, char const * v1, void * v2, size_t * v3)
{
    return ::nng_ctx_getopt(v0, v1, v2, v3);
}

int nng_ctx_getopt_bool(nng_ctx v0, char const * v1, bool * v2)
{
    return ::nng_ctx_getopt_bool(v0, v1, v2);
}

int nng_ctx_getopt_int(nng_ctx v0, char const * v1, int * v2)
{
    return ::nng_ctx_getopt_int(v0, v1, v2);
}

int nng_ctx_getopt_ms(nng_ctx v0, char const * v1, nng_duration * v2)
{
    return ::nng_ctx_getopt_ms(v0, v1, v2);
}

int nng_ctx_getopt_size(nng_ctx v0, char const * v1, size_t * v2)
{
    return ::nng_ctx_getopt_size(v0, v1, v2);
}

int nng_ctx_setopt(nng_ctx v0, char const * v1, void const * v2, size_t v3)
{
    return ::nng_ctx_setopt(v0, v1, v2, v3);
}

int nng_ctx_setopt_bool(nng_ctx v0, char const * v1, bool v2)
{
    return ::nng_ctx_setopt_bool(v0, v1, v2);
}

int nng_ctx_setopt_int(nng_ctx v0, char const * v1, int v2)
{
    return ::nng_ctx_setopt_int(v0, v1, v2);
}

int nng_ctx_setopt_ms(nng_ctx v0, char const * v1, nng_duration v2)
{
    return ::nng_ctx_setopt_ms(v0, v1, v2);
}

int nng_ctx_setopt_size(nng_ctx v0, char const * v1, size_t v2)
{
    return ::nng_ctx_setopt_size(v0, v1, v2);
}

void * nng_alloc(size_t v0)
{
    return ::nng_alloc(v0);
}

void nng_free(void * v0, size_t v1)
{
    ::nng_free(v0, v1);
}

char * nng_strdup(char const * v0)
{
    return ::nng_strdup(v0);
}

void nng_strfree(char * v0)
{
    ::nng_strfree(v0);
}

int nng_aio_alloc(nng_aio ** v0, void (*v1)(void *), void * v2)
{
    return ::nng_aio_alloc(v0, v1, v2);
}

void nng_aio_free(nng_aio * v0)
{
    ::nng_aio_free(v0);
}

void nng_aio_stop(nng_aio * v0)
{
    ::nng_aio_stop(v0);
}

int nng_aio_result(nng_aio * v0)
{
    return ::nng_aio_result(v0);
}

size_t nng_aio_count(nng_aio * v0)
{
    return ::nng_aio_count(v0);
}

void nng_aio_cancel(nng_aio * v0)
{
    ::nng_aio_cancel(v0);
}

void nng_aio_abort(nng_aio * v0, int v1)
{
    ::nng_aio_abort(v0, v1);
}

void nng_aio_wait(nng_aio * v0)
{
    ::nng_aio_wait(v0);
}

void nng_aio_set_msg(nng_aio * v0, nng_msg * v1)
{
    ::nng_aio_set_msg(v0, v1);
}

nng_msg * nng_aio_get_msg(nng_aio * v0)
{
    return ::nng_aio_get_msg(v0);
}

int nng_aio_set_input(nng_aio * v0, unsigned v1, void * v2)
{
    return ::nng_aio_set_input(v0, v1, v2);
}

void * nng_aio_get_input(nng_aio * v0, unsigned v1)
{
    return ::nng_aio_get_input(v0, v1);
}

int nng_aio_set_output(nng_aio * v0, unsigned v1, void * v2)
{
    return ::nng_aio_set_output(v0, v1, v2);
}

void * nng_aio_get_output(nng_aio * v0, unsigned v1)
{
    return ::nng_aio_get_output(v0, v1);
}

void nng_aio_set_timeout(nng_aio * v0, nng_duration v1)
{
    ::nng_aio_set_timeout(v0, v1);
}

int nng_aio_set_iov(nng_aio * v0, unsigned v1, nng_iov const * v2)
{
    return ::nng_aio_set_iov(v0, v1, v2);
}

bool nng_aio_begin(nng_aio * v0)
{
    return ::nng_aio_begin(v0);
}

void nng_aio_finish(nng_aio * v0, int v1)
{
    ::nng_aio_finish(v0, v1);
}

void nng_aio_defer(nng_aio * v0, nng_aio_cancelfn v1, void * v2)
{
    ::nng_aio_defer(v0, v1, v2);
}

void nng_sleep_aio(nng_duration v0, nng_aio * v1)
{
    ::nng_sleep_aio(v0, v1);
}

int nng_msg_alloc(nng_msg ** v0, size_t v1)
{
    return ::nng_msg_alloc(v0, v1);
}

void nng_msg_free(nng_msg * v0)
{
    ::nng_msg_free(v0);
}

int nng_msg_realloc(nng_msg * v0, size_t v1)
{
    return ::nng_msg_realloc(v0, v1);
}

void * nng_msg_header(nng_msg * v0)
{
    return ::nng_msg_header(v0);
}

size_t nng_msg_header_len(nng_msg const * v0)
{
    return ::nng_msg_header_len(v0);
}

void * nng_msg_body(nng_msg * v0)
{
    return ::nng_msg_body(v0);
}

size_t nng_msg_len(nng_msg const * v0)
{
    return ::nng_msg_len(v0);
}

int nng_msg_append(nng_msg * v0, void const * v1, size_t v2)
{
    return ::nng_msg_append(v0, v1, v2);
}

int nng_msg_insert(nng_msg * v0, void const * v1, size_t v2)
{
    return ::nng_msg_insert(v0, v1, v2);
}

int nng_msg_trim(nng_msg * v0, size_t v1)
{
    return ::nng_msg_trim(v0, v1);
}

int nng_msg_chop(nng_msg * v0, size_t v1)
{
    return ::nng_msg_chop(v0, v1);
}

int nng_msg_header_append(nng_msg * v0, void const * v1, size_t v2)
{
    return ::nng_msg_header_append(v0, v1, v2);
}

int nng_msg_header_insert(nng_msg * v0, void const * v1, size_t v2)
{
    return ::nng_msg_header_insert(v0, v1, v2);
}

int nng_msg_header_trim(nng_msg * v0, size_t v1)
{
    return ::nng_msg_header_trim(v0, v1);
}

int nng_msg_header_chop(nng_msg * v0, size_t v1)
{
    return ::nng_msg_header_chop(v0, v1);
}

int nng_msg_header_append_u16(nng_msg * v0, uint16_t v1)
{
    return ::nng_msg_header_append_u16(v0, v1);
}

int nng_msg_header_append_u32(nng_msg * v0, uint32_t v1)
{
    return ::nng_msg_header_append_u32(v0, v1);
}

int nng_msg_header_append_u64(nng_msg * v0, uint64_t v1)
{
    return ::nng_msg_header_append_u64(v0, v1);
}

int nng_msg_header_insert_u16(nng_msg * v0, uint16_t v1)
{
    return ::nng_msg_header_insert_u16(v0, v1);
}

int nng_msg_header_insert_u32(nng_msg * v0, uint32_t v1)
{
    return ::nng_msg_header_insert_u32(v0, v1);
}

int nng_msg_header_insert_u64(nng_msg * v0, uint64_t v1)
{
    return ::nng_msg_header_insert_u64(v0, v1);
}

int nng_msg_header_chop_u16(nng_msg * v0, uint16_t * v1)
{
    return ::nng_msg_header_chop_u16(v0, v1);
}

int nng_msg_header_chop_u32(nng_msg * v0, uint32_t * v1)
{
    return ::nng_msg_header_chop_u32(v0, v1);
}

int nng_msg_header_chop_u64(nng_msg * v0, uint64_t * v1)
{
    return ::nng_msg_header_chop_u64(v0, v1);
}

int nng_msg_header_trim_u16(nng_msg * v0, uint16_t * v1)
{
    return ::nng_msg_header_trim_u16(v0, v1);
}

int nng_msg_header_trim_u32(nng_msg * v0, uint32_t * v1)
{
    return ::nng_msg_header_trim_u32(v0, v1);
}

int nng_msg_header_trim_u64(nng_msg * v0, uint64_t * v1)
{
    return ::nng_msg_header_trim_u64(v0, v1);
}

int nng_msg_append_u16(nng_msg * v0, uint16_t v1)
{
    return ::nng_msg_append_u16(v0, v1);
}

int nng_msg_append_u32(nng_msg * v0, uint32_t v1)
{
    return ::nng_msg_append_u32(v0, v1);
}

int nng_msg_append_u64(nng_msg * v0, uint64_t v1)
{
    return ::nng_msg_append_u64(v0, v1);
}

int nng_msg_insert_u16(nng_msg * v0, uint16_t v1)
{
    return ::nng_msg_insert_u16(v0, v1);
}

int nng_msg_insert_u32(nng_msg * v0, uint32_t v1)
{
    return ::nng_msg_insert_u32(v0, v1);
}

int nng_msg_insert_u64(nng_msg * v0, uint64_t v1)
{
    return ::nng_msg_insert_u64(v0, v1);
}

int nng_msg_chop_u16(nng_msg * v0, uint16_t * v1)
{
    return ::nng_msg_chop_u16(v0, v1);
}

int nng_msg_chop_u32(nng_msg * v0, uint32_t * v1)
{
    return ::nng_msg_chop_u32(v0, v1);
}

int nng_msg_chop_u64(nng_msg * v0, uint64_t * v1)
{
    return ::nng_msg_chop_u64(v0, v1);
}

int nng_msg_trim_u16(nng_msg * v0, uint16_t * v1)
{
    return ::nng_msg_trim_u16(v0, v1);
}

int nng_msg_trim_u32(nng_msg * v0, uint32_t * v1)
{
    return ::nng_msg_trim_u32(v0, v1);
}

int nng_msg_trim_u64(nng_msg * v0, uint64_t * v1)
{
    return ::nng_msg_trim_u64(v0, v1);
}

int nng_msg_dup(nng_msg ** v0, nng_msg const * v1)
{
    return ::nng_msg_dup(v0, v1);
}

void nng_msg_clear(nng_msg * v0)
{
    return ::nng_msg_clear(v0);
}

void nng_msg_header_clear(nng_msg * v0)
{
    ::nng_msg_header_clear(v0);
}

void nng_msg_set_pipe(nng_msg * v0, nng_pipe v1)
{
    ::nng_msg_set_pipe(v0, v1);
}

nng_pipe nng_msg_get_pipe(nng_msg const * v0)
{
    return ::nng_msg_get_pipe(v0);
}

int nng_msg_getopt(nng_msg * v0, int v1, void * v2, size_t * v3)
{
    return ::nng_msg_getopt(v0, v1, v2, v3);
}

int nng_pipe_getopt(nng_pipe v0, char const * v1, void * v2, size_t * v3)
{
    return ::nng_pipe_getopt(v0, v1, v2, v3);
}

int nng_pipe_getopt_bool(nng_pipe v0, char const * v1, bool * v2)
{
    return ::nng_pipe_getopt_bool(v0, v1, v2);
}

int nng_pipe_getopt_int(nng_pipe v0, char const * v1, int * v2)
{
    return ::nng_pipe_getopt_int(v0, v1, v2);
}

int nng_pipe_getopt_ms(nng_pipe v0, char const * v1, nng_duration * v2)
{
    return ::nng_pipe_getopt_ms(v0, v1, v2);
}

int nng_pipe_getopt_size(nng_pipe v0, char const * v1, size_t * v2)
{
    return ::nng_pipe_getopt_size(v0, v1, v2);
}

int nng_pipe_getopt_sockaddr(nng_pipe v0, char const * v1, nng_sockaddr * v2)
{
    return ::nng_pipe_getopt_sockaddr(v0, v1, v2);
}

int nng_pipe_getopt_uint64(nng_pipe v0, char const * v1, uint64_t * v2)
{
    return ::nng_pipe_getopt_uint64(v0, v1, v2);
}

int nng_pipe_getopt_ptr(nng_pipe v0, char const * v1, void ** v2)
{
    return ::nng_pipe_getopt_ptr(v0, v1, v2);
}

int nng_pipe_getopt_string(nng_pipe v0, char const * v1, char ** v2)
{
    return ::nng_pipe_getopt_string(v0, v1, v2);
}

int nng_pipe_close(nng_pipe v0)
{
    return ::nng_pipe_close(v0);
}

int nng_pipe_id(nng_pipe v0)
{
    return ::nng_pipe_id(v0);
}

nng_socket nng_pipe_socket(nng_pipe v0)
{
    return ::nng_pipe_socket(v0);
}

nng_dialer nng_pipe_dialer(nng_pipe v0)
{
    return ::nng_pipe_dialer(v0);
}

nng_listener nng_pipe_listener(nng_pipe v0)
{
    return ::nng_pipe_listener(v0);
}

int nng_stats_get(nng_stat ** v0)
{
    return ::nng_stats_get(v0);
}

void nng_stats_free(nng_stat * v0)
{
    ::nng_stats_free(v0);
}

void nng_stats_dump(nng_stat * v0)
{
    ::nng_stats_dump(v0);
}

nng_stat * nng_stat_next(nng_stat * v0)
{
    return ::nng_stat_next(v0);
}

nng_stat * nng_stat_child(nng_stat * v0)
{
    return ::nng_stat_child(v0);
}

char const * nng_stat_name(nng_stat * v0)
{
    return ::nng_stat_name(v0);
}

int nng_stat_type(nng_stat * v0)
{
    return ::nng_stat_type(v0);
}

int nng_stat_unit(nng_stat * v0)
{
    return ::nng_stat_unit(v0);
}

uint64_t nng_stat_value(nng_stat * v0)
{
    return ::nng_stat_value(v0);
}

char const * nng_stat_string(nng_stat * v0)
{
    return ::nng_stat_string(v0);
}

char const * nng_stat_desc(nng_stat * v0)
{
    return ::nng_stat_desc(v0);
}

uint64_t nng_stat_timestamp(nng_stat * v0)
{
    return ::nng_stat_timestamp(v0);
}

int nng_device(nng_socket v0, nng_socket v1)
{
    return ::nng_device(v0, v1);
}

int nng_url_parse(nng_url ** v0, char const * v1)
{
    return ::nng_url_parse(v0, v1);
}

void nng_url_free(nng_url * v0)
{
    ::nng_url_free(v0);
}

int nng_url_clone(nng_url ** v0, const nng_url * v1)
{
    return ::nng_url_clone(v0, v1);
}

char const * nng_version()
{
    return ::nng_version();
}

# /************/
# /* protocol */
# /************/

int nng_bus0_open(nng_socket * v0)
{
    return ::nng_bus0_open(v0);
}

int nng_bus0_open_raw(nng_socket * v0)
{
    return ::nng_bus0_open_raw(v0);
}

int nng_pair0_open(nng_socket * v0)
{
    return ::nng_pair0_open(v0);
}

int nng_pair0_open_raw(nng_socket * v0)
{
    return ::nng_pair0_open_raw(v0);
}

int nng_pair1_open(nng_socket * v0)
{
    return ::nng_pair1_open(v0);
}

int nng_pair1_open_raw(nng_socket * v0)
{
    return ::nng_pair1_open_raw(v0);
}

int nng_pull0_open(nng_socket * v0)
{
    return ::nng_pull0_open(v0);
}

int nng_pull0_open_raw(nng_socket * v0)
{
    return ::nng_pull0_open_raw(v0);
}

int nng_push0_open(nng_socket * v0)
{
    return ::nng_push0_open(v0);
}

int nng_push0_open_raw(nng_socket * v0)
{
    return ::nng_push0_open_raw(v0);
}

int nng_pub0_open(nng_socket * v0)
{
    return ::nng_pub0_open(v0);
}

int nng_pub0_open_raw(nng_socket * v0)
{
    return ::nng_pub0_open_raw(v0);
}

int nng_sub0_open(nng_socket * v0)
{
    return ::nng_sub0_open(v0);
}

int nng_sub0_open_raw(nng_socket * v0)
{
    return ::nng_sub0_open_raw(v0);
}

int nng_rep0_open(nng_socket * v0)
{
    return ::nng_rep0_open(v0);
}

int nng_rep0_open_raw(nng_socket * v0)
{
    return ::nng_rep0_open_raw(v0);
}

int nng_req0_open(nng_socket * v0)
{
    return ::nng_req0_open(v0);
}

int nng_req0_open_raw(nng_socket * v0)
{
    return ::nng_req0_open_raw(v0);
}

int nng_respondent0_open(nng_socket * v0)
{
    return ::nng_respondent0_open(v0);
}

int nng_respondent0_open_raw(nng_socket * v0)
{
    return ::nng_respondent0_open_raw(v0);
}

int nng_surveyor0_open(nng_socket * v0)
{
    return ::nng_surveyor0_open(v0);
}

int nng_surveyor0_open_raw(nng_socket * v0)
{
    return ::nng_surveyor0_open_raw(v0);
}

# /*********************/
# /* supplemental/http */
# /*********************/

int nng_http_req_alloc(nng_http_req ** v0, nng_url const * v1)
{
    return ::nng_http_req_alloc(v0, v1);
}

void nng_http_req_free(nng_http_req * v0)
{
    ::nng_http_req_free(v0);
}

char const * nng_http_req_get_method(nng_http_req * v0)
{
    return ::nng_http_req_get_method(v0);
}

char const * nng_http_req_get_version(nng_http_req * v0)
{
    return ::nng_http_req_get_version(v0);
}

char const * nng_http_req_get_uri(nng_http_req * v0)
{
    return ::nng_http_req_get_uri(v0);
}

int nng_http_req_set_header(nng_http_req * v0, char const * v1, char const * v2)
{
    return ::nng_http_req_set_header(v0, v1, v2);
}

int nng_http_req_add_header(nng_http_req * v0, char const * v1, char const * v2)
{
    return ::nng_http_req_add_header(v0, v1, v2);
}

int nng_http_req_del_header(nng_http_req * v0, char const * v1)
{
    return ::nng_http_req_del_header(v0, v1);
}

char const * nng_http_req_get_header(nng_http_req * v0, char const * v1)
{
    return ::nng_http_req_get_header(v0, v1);
}

int nng_http_req_set_method(nng_http_req * v0, char const * v1)
{
    return ::nng_http_req_set_method(v0, v1);
}

int nng_http_req_set_version(nng_http_req * v0, char const * v1)
{
    return ::nng_http_req_set_version(v0, v1);
}

int nng_http_req_set_uri(nng_http_req * v0, char const * v1)
{
    return ::nng_http_req_set_uri(v0, v1);
}

int nng_http_req_set_data(nng_http_req * v0, void const * v1, size_t v2)
{
    return ::nng_http_req_set_data(v0, v1, v2);
}

int nng_http_req_copy_data(nng_http_req * v0, void const * v1, size_t v2)
{
    return ::nng_http_req_copy_data(v0, v1, v2);
}

void nng_http_req_get_data(nng_http_req * v0, void ** v1, size_t * v2)
{
    ::nng_http_req_get_data(v0, v1, v2);
}

int nng_http_res_alloc(nng_http_res ** v0)
{
    return ::nng_http_res_alloc(v0);
}

int nng_http_res_alloc_error(nng_http_res ** v0, uint16_t v1)
{
    return ::nng_http_res_alloc_error(v0, v1);
}

void nng_http_res_free(nng_http_res * v0)
{
    ::nng_http_res_free(v0);
}

uint16_t nng_http_res_get_status(nng_http_res * v0)
{
    return ::nng_http_res_get_status(v0);
}

int nng_http_res_set_status(nng_http_res * v0, uint16_t v1)
{
    return ::nng_http_res_set_status(v0, v1);
}

char const * nng_http_res_get_reason(nng_http_res * v0)
{
    return ::nng_http_res_get_reason(v0);
}

int nng_http_res_set_reason(nng_http_res * v0, char const * v1)
{
    return ::nng_http_res_set_reason(v0, v1);
}

int nng_http_res_set_header(nng_http_res * v0, char const * v1, char const * v2)
{
    return ::nng_http_res_set_header(v0, v1, v2);
}

int nng_http_res_add_header(nng_http_res * v0, char const * v1, char const * v2)
{
    return ::nng_http_res_add_header(v0, v1, v2);
}

int nng_http_res_del_header(nng_http_res * v0, char const * v1)
{
    return ::nng_http_res_del_header(v0, v1);
}

char const * nng_http_res_get_header(nng_http_res * v0, char const * v1)
{
    return ::nng_http_res_get_header(v0, v1);
}

int nng_http_res_set_version(nng_http_res * v0, char const * v1)
{
    return ::nng_http_res_set_version(v0, v1);
}

char const * nng_http_res_get_version(nng_http_res * v0)
{
    return ::nng_http_res_get_version(v0);
}

void nng_http_res_get_data(nng_http_res * v0, void ** v1, size_t * v2)
{
    ::nng_http_res_get_data(v0, v1, v2);
}

int nng_http_res_set_data(nng_http_res * v0, void const * v1, size_t v2)
{
    return ::nng_http_res_set_data(v0, v1, v2);
}

int nng_http_res_copy_data(nng_http_res * v0, void const * v1, size_t v2)
{
    return ::nng_http_res_copy_data(v0, v1, v2);
}

void nng_http_conn_close(nng_http_conn * v0)
{
    ::nng_http_conn_close(v0);
}

void nng_http_conn_read(nng_http_conn * v0, nng_aio * v1)
{
    ::nng_http_conn_read(v0, v1);
}

void nng_http_conn_read_all(nng_http_conn * v0, nng_aio * v1)
{
    ::nng_http_conn_read_all(v0, v1);
}

void nng_http_conn_write(nng_http_conn * v0, nng_aio * v1)
{
    ::nng_http_conn_write(v0, v1);
}

void nng_http_conn_write_all(nng_http_conn * v0, nng_aio * v1)
{
    ::nng_http_conn_write_all(v0, v1);
}

void nng_http_conn_write_req(nng_http_conn * v0, nng_http_req * v1, nng_aio * v2)
{
    ::nng_http_conn_write_req(v0, v1, v2);
}

void nng_http_conn_write_res(nng_http_conn * v0, nng_http_res * v1, nng_aio * v2)
{
    ::nng_http_conn_write_res(v0, v1, v2);
}

void nng_http_conn_read_req(nng_http_conn * v0, nng_http_req * v1, nng_aio * v2)
{
    ::nng_http_conn_read_req(v0, v1, v2);
}

void nng_http_conn_read_res(nng_http_conn * v0, nng_http_res * v1, nng_aio * v2)
{
    ::nng_http_conn_read_res(v0, v1, v2);
}

void nng_http_req_reset(nng_http_req * v0)
{
    ::nng_http_req_reset(v0);
}

void nng_http_res_reset(nng_http_res * v0)
{
    ::nng_http_res_reset(v0);
}

int nng_http_handler_alloc(nng_http_handler ** v0, char const * v1, void (*v2)(nng_aio *))
{
    return ::nng_http_handler_alloc(v0, v1, v2);
}

void nng_http_handler_free(nng_http_handler * v0)
{
    ::nng_http_handler_free(v0);
}

int nng_http_handler_alloc_file(nng_http_handler ** v0, char const * v1, char const * v2)
{
    return ::nng_http_handler_alloc_file(v0, v1, v2);
}

int nng_http_handler_alloc_static(nng_http_handler ** v0, char const * v1, void const * v2, size_t v3, char const * v4)
{
    return ::nng_http_handler_alloc_static(v0, v1, v2, v3, v4);
}

int nng_http_handler_alloc_redirect(nng_http_handler ** v0, char const * v1, uint16_t v2, char const * v3)
{
    return ::nng_http_handler_alloc_redirect(v0, v1, v2, v3);
}

int nng_http_handler_alloc_directory(nng_http_handler ** v0, char const * v1, char const * v2)
{
    return ::nng_http_handler_alloc_directory(v0, v1, v2);
}

int nng_http_handler_set_method(nng_http_handler * v0, char const * v1)
{
    return ::nng_http_handler_set_method(v0, v1);
}

int nng_http_handler_set_host(nng_http_handler * v0, char const * v1)
{
    return ::nng_http_handler_set_host(v0, v1);
}

int nng_http_handler_collect_body(nng_http_handler * v0, bool v1, size_t v2)
{
    return ::nng_http_handler_collect_body(v0, v1, v2);
}

int nng_http_handler_set_tree(nng_http_handler * v0)
{
    return ::nng_http_handler_set_tree(v0);
}

int nng_http_handler_set_data(nng_http_handler * v0, void * v1, void (*v2)(void *))
{
    return ::nng_http_handler_set_data(v0, v1, v2);
}

void * nng_http_handler_get_data(nng_http_handler * v0)
{
    return ::nng_http_handler_get_data(v0);
}

int nng_http_server_hold(nng_http_server ** v0, nng_url const * v1)
{
    return ::nng_http_server_hold(v0, v1);
}

void nng_http_server_release(nng_http_server * v0)
{
    ::nng_http_server_release(v0);
}

int nng_http_server_start(nng_http_server * v0)
{
    return ::nng_http_server_start(v0);
}

void nng_http_server_stop(nng_http_server * v0)
{
    ::nng_http_server_stop(v0);
}

int nng_http_server_add_handler(nng_http_server * v0, nng_http_handler * v1)
{
    return ::nng_http_server_add_handler(v0, v1);
}

int nng_http_server_del_handler(nng_http_server * v0, nng_http_handler * v1)
{
    return ::nng_http_server_del_handler(v0, v1);
}

int nng_http_server_set_tls(nng_http_server * v0, struct nng_tls_config * v1)
{
    return ::nng_http_server_set_tls(v0, v1);
}

int nng_http_server_get_tls(nng_http_server * v0, struct nng_tls_config ** v1)
{
    return ::nng_http_server_get_tls(v0, v1);
}

int nng_http_server_set_error_page(nng_http_server * v0, uint16_t v1, char const * v2)
{
    return ::nng_http_server_set_error_page(v0, v1, v2);
}

int nng_http_server_set_error_file(nng_http_server * v0, uint16_t v1, char const * v2)
{
    return ::nng_http_server_set_error_file(v0, v1, v2);
}

int nng_http_server_res_error(nng_http_server * v0, nng_http_res * v1)
{
    return ::nng_http_server_res_error(v0, v1);
}

int nng_http_hijack(nng_http_conn * v0)
{
    return ::nng_http_hijack(v0);
}

int nng_http_client_alloc(nng_http_client ** v0, nng_url const * v1)
{
    return ::nng_http_client_alloc(v0, v1);
}

void nng_http_client_free(nng_http_client * v0)
{
    ::nng_http_client_free(v0);
}

int nng_http_client_set_tls(nng_http_client * v0, struct nng_tls_config * v1)
{
    return ::nng_http_client_set_tls(v0, v1);
}

int nng_http_client_get_tls(nng_http_client * v0, struct nng_tls_config ** v1)
{
    return ::nng_http_client_get_tls(v0, v1);
}

void nng_http_client_connect(nng_http_client * v0, nng_aio * v1)
{
    ::nng_http_client_connect(v0, v1);
}

void nng_http_conn_transact(nng_http_conn * v0, nng_http_req * v1, nng_http_res * v2, nng_aio * v3)
{
    ::nng_http_conn_transact(v0, v1, v2, v3);
}

void nng_http_client_transact(nng_http_client * v0, nng_http_req * v1, nng_http_res * v2, nng_aio * v3)
{
    ::nng_http_client_transact(v0, v1, v2, v3);
}

# /********************/
# /* supplemental/tls */
# /********************/

#if !defined(TBAG_DISABLE_NNG_TLS)
int nng_tls_config_alloc(nng_tls_config ** v0, nng_tls_mode v1)
{
    return ::nng_tls_config_alloc(v0, v1);
}

void nng_tls_config_hold(nng_tls_config * v0)
{
    ::nng_tls_config_hold(v0);
}

void nng_tls_config_free(nng_tls_config * v0)
{
    ::nng_tls_config_free(v0);
}

int nng_tls_config_server_name(nng_tls_config * v0, char const * v1)
{
    return ::nng_tls_config_server_name(v0, v1);
}

int nng_tls_config_ca_chain(nng_tls_config * v0, char const * v1, char const * v2)
{
    return ::nng_tls_config_ca_chain(v0, v1, v2);
}

int nng_tls_config_own_cert(nng_tls_config * v0, char const * v1, char const * v2, char const * v3)
{
    return ::nng_tls_config_own_cert(v0, v1, v2, v3);
}

int nng_tls_config_key(nng_tls_config * v0, const uint8_t * v1, size_t v2)
{
    return ::nng_tls_config_key(v0, v1, v2);
}

int nng_tls_config_pass(nng_tls_config * v0, char const * v1)
{
    return ::nng_tls_config_pass(v0, v1);
}

int nng_tls_config_auth_mode(nng_tls_config * v0, nng_tls_auth_mode v1)
{
    return ::nng_tls_config_auth_mode(v0, v1);
}

int nng_tls_config_ca_file(nng_tls_config * v0, char const * v1)
{
    return ::nng_tls_config_ca_file(v0, v1);
}

int nng_tls_config_cert_key_file(nng_tls_config * v0, char const * v1, char const * v2)
{
    return ::nng_tls_config_cert_key_file(v0, v1, v2);
}
#endif // #if !defined(TBAG_DISABLE_NNG_TLS)

# /*********************/
# /* supplemental/util */
# /*********************/

int nng_opts_parse(int argc, char * const * argv,
                   nng_optspec const * opts,
                   int * val,
                   char ** optarg,
                   int * optidx)
{
    return ::nng_opts_parse(argc, argv, opts, val, optarg, optidx);
}

nng_time nng_clock()
{
    return ::nng_clock();
}

void nng_msleep(nng_duration v0)
{
    ::nng_msleep(v0);
}

int nng_thread_create(nng_thread ** v0, void (*v1)(void *), void * v2)
{
    return ::nng_thread_create(v0, v1, v2);
}

void nng_thread_destroy(nng_thread * v0)
{
    ::nng_thread_destroy(v0);
}

int nng_mtx_alloc(nng_mtx ** v0)
{
    return ::nng_mtx_alloc(v0);
}

void nng_mtx_free(nng_mtx * v0)
{
    ::nng_mtx_free(v0);
}

void nng_mtx_lock(nng_mtx * v0)
{
    ::nng_mtx_lock(v0);
}

void nng_mtx_unlock(nng_mtx * v0)
{
    ::nng_mtx_unlock(v0);
}

int nng_cv_alloc(nng_cv ** v0, nng_mtx * v1)
{
    return ::nng_cv_alloc(v0, v1);
}

void nng_cv_free(nng_cv * v0)
{
    ::nng_cv_free(v0);
}

void nng_cv_wait(nng_cv * v0)
{
    ::nng_cv_wait(v0);
}

int nng_cv_until(nng_cv * v0, nng_time v1)
{
    return ::nng_cv_until(v0, v1);
}

void nng_cv_wake(nng_cv * v0)
{
    ::nng_cv_wake(v0);
}

void nng_cv_wake1(nng_cv * v0)
{
    ::nng_cv_wake1(v0);
}

uint32_t nng_random()
{
    return ::nng_random();
}

//int nng_inproc_register()
//{
//    return ::nng_inproc_register();
//}
//
//int nng_ipc_register()
//{
//    return ::nng_ipc_register();
//}
//
//int nng_tcp_register()
//{
//    return ::nng_tcp_register();
//}
//
//int nng_ws_register()
//{
//    return ::nng_ws_register();
//}
//
//int nng_wss_register()
//{
//    return ::nng_wss_register();
//}

# /*************/
# /* extension */
# /*************/

Err nng_code_err(int code)
{
    // clang-format off
    switch (code) {
    case 0:                 return E_SUCCESS;
    case NNG_EINTR:         return E_NNG_EINTR;
    case NNG_ENOMEM:        return E_NNG_ENOMEM;
    case NNG_EINVAL:        return E_NNG_EINVAL;
    case NNG_EBUSY:         return E_NNG_EBUSY;
    case NNG_ETIMEDOUT:     return E_NNG_ETIMEDOUT;
    case NNG_ECONNREFUSED:  return E_NNG_ECONNREFUSED;
    case NNG_ECLOSED:       return E_NNG_ECLOSED;
    case NNG_EAGAIN:        return E_NNG_EAGAIN;
    case NNG_ENOTSUP:       return E_NNG_ENOTSUP;
    case NNG_EADDRINUSE:    return E_NNG_EADDRINUSE;
    case NNG_ESTATE:        return E_NNG_ESTATE;
    case NNG_ENOENT:        return E_NNG_ENOENT;
    case NNG_EPROTO:        return E_NNG_EPROTO;
    case NNG_EUNREACHABLE:  return E_NNG_EUNREACHABLE;
    case NNG_EADDRINVAL:    return E_NNG_EADDRINVAL;
    case NNG_EPERM:         return E_NNG_EPERM;
    case NNG_EMSGSIZE:      return E_NNG_EMSGSIZE;
    case NNG_ECONNABORTED:  return E_NNG_ECONNABORTED;
    case NNG_ECONNRESET:    return E_NNG_ECONNRESET;
    case NNG_ECANCELED:     return E_NNG_ECANCELED;
    case NNG_ENOFILES:      return E_NNG_ENOFILES;
    case NNG_ENOSPC:        return E_NNG_ENOSPC;
    case NNG_EEXIST:        return E_NNG_EEXIST;
    case NNG_EREADONLY:     return E_NNG_EREADONLY;
    case NNG_EWRITEONLY:    return E_NNG_EWRITEONLY;
    case NNG_ECRYPTO:       return E_NNG_ECRYPTO;
    case NNG_EPEERAUTH:     return E_NNG_EPEERAUTH;
    case NNG_ENOARG:        return E_NNG_ENOARG;
    case NNG_EAMBIGUOUS:    return E_NNG_EAMBIGUOUS;
    case NNG_EBADTYPE:      return E_NNG_EBADTYPE;
    case NNG_EINTERNAL:     return E_NNG_EINTERNAL;
    case NNG_ESYSERR:       return E_UNKNOWN; // System error flag.
    case NNG_ETRANERR:      return E_UNKNOWN; // Transport error flag.
    default:                return E_UNKNOWN;
    }
    // clang-format on
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

