/**
 * @file   NngBypass.cpp
 * @brief  NngBypass class implementation.
 * @author zer0
 * @date   2019-03-21
 */

// [WARNING] Don't change the order of including.
#include <nng/nng.h>
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

int nng_pipe_notify(nng_socket v0, int v1, nng_pipe_cb v2, void * v3)
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

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

