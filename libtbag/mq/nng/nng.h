
#ifndef NNG_NNG_H
#define NNG_NNG_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


# /* ==[[ MIGRATION BEGIN ]]== */
#ifndef NNG_DECL
#include <libtbag/predef.hpp>
#define NNG_DECL TBAG_API
#endif
# /* ==[[ MIGRATION END ]]== */

#define NNG_MAJOR_VERSION 1
#define NNG_MINOR_VERSION 3
#define NNG_PATCH_VERSION 0
#define NNG_RELEASE_SUFFIX "" // if non-empty, this is a pre-release

#define NNG_MAXADDRLEN (128)

#define NNG_PROTOCOL_NUMBER(maj, min) (((x) *16) + (y))



typedef struct nng_ctx_s {
	uint32_t id;
} nng_ctx;

typedef struct nng_dialer_s {
	uint32_t id;
} nng_dialer;

typedef struct nng_listener_s {
	uint32_t id;
} nng_listener;

typedef struct nng_pipe_s {
	uint32_t id;
} nng_pipe;

typedef struct nng_socket_s {
	uint32_t id;
} nng_socket;

typedef int32_t         nng_duration;
typedef struct nng_msg  nng_msg;
typedef struct nng_stat nng_stat;
typedef struct nng_aio  nng_aio;

#define NNG_PIPE_INITIALIZER { 0 }
#define NNG_SOCKET_INITIALIZER { 0 }
#define NNG_DIALER_INITIALIZER { 0 }
#define NNG_LISTENER_INITIALIZER { 0 }
#define NNG_CTX_INITIALIZER { 0 }

struct nng_sockaddr_inproc {
	uint16_t sa_family;
	char     sa_name[NNG_MAXADDRLEN];
};
typedef struct nng_sockaddr_inproc nng_sockaddr_inproc;

struct nng_sockaddr_path {
	uint16_t sa_family;
	char     sa_path[NNG_MAXADDRLEN];
};
typedef struct nng_sockaddr_path nng_sockaddr_path;
typedef struct nng_sockaddr_path nng_sockaddr_ipc;

struct nng_sockaddr_in6 {
	uint16_t sa_family;
	uint16_t sa_port;
	uint8_t  sa_addr[16];
};
typedef struct nng_sockaddr_in6 nng_sockaddr_in6;
typedef struct nng_sockaddr_in6 nng_sockaddr_udp6;
typedef struct nng_sockaddr_in6 nng_sockaddr_tcp6;

struct nng_sockaddr_in {
	uint16_t sa_family;
	uint16_t sa_port;
	uint32_t sa_addr;
};

struct nng_sockaddr_zt {
	uint16_t sa_family;
	uint64_t sa_nwid;
	uint64_t sa_nodeid;
	uint32_t sa_port;
};

typedef struct nng_sockaddr_in nng_sockaddr_in;
typedef struct nng_sockaddr_in nng_sockaddr_udp;
typedef struct nng_sockaddr_in nng_sockaddr_tcp;
typedef struct nng_sockaddr_zt nng_sockaddr_zt;

typedef union nng_sockaddr {
	uint16_t            s_family;
	nng_sockaddr_ipc    s_ipc;
	nng_sockaddr_inproc s_inproc;
	nng_sockaddr_in6    s_in6;
	nng_sockaddr_in     s_in;
	nng_sockaddr_zt     s_zt;
} nng_sockaddr;

enum nng_sockaddr_family {
	NNG_AF_UNSPEC = 0,
	NNG_AF_INPROC = 1,
	NNG_AF_IPC    = 2,
	NNG_AF_INET   = 3,
	NNG_AF_INET6  = 4,
	NNG_AF_ZT     = 5
};

typedef struct nng_iov {
	void * iov_buf;
	size_t iov_len;
} nng_iov;

#define NNG_DURATION_INFINITE (-1)
#define NNG_DURATION_DEFAULT (-2)
#define NNG_DURATION_ZERO (0)

/*NNG_DECL void nng_fini(void);*/

/*NNG_DECL int nng_close(nng_socket);*/

/*NNG_DECL int nng_socket_id(nng_socket);*/

/*NNG_DECL void nng_closeall(void);*/

/*NNG_DECL int nng_setopt(nng_socket, const char *, const void *, size_t);*/
/*NNG_DECL int nng_setopt_bool(nng_socket, const char *, bool);*/
/*NNG_DECL int nng_setopt_int(nng_socket, const char *, int);*/
/*NNG_DECL int nng_setopt_ms(nng_socket, const char *, nng_duration);*/
/*NNG_DECL int nng_setopt_size(nng_socket, const char *, size_t);*/
/*NNG_DECL int nng_setopt_uint64(nng_socket, const char *, uint64_t);*/
/*NNG_DECL int nng_setopt_string(nng_socket, const char *, const char *);*/
/*NNG_DECL int nng_setopt_ptr(nng_socket, const char *, void *);*/

/*NNG_DECL int nng_getopt(nng_socket, const char *, void *, size_t *);*/
/*NNG_DECL int nng_getopt_bool(nng_socket, const char *, bool *);*/
/*NNG_DECL int nng_getopt_int(nng_socket, const char *, int *);*/
/*NNG_DECL int nng_getopt_ms(nng_socket, const char *, nng_duration *);*/
/*NNG_DECL int nng_getopt_size(nng_socket, const char *, size_t *);*/
/*NNG_DECL int nng_getopt_uint64(nng_socket, const char *, uint64_t *);*/
/*NNG_DECL int nng_getopt_ptr(nng_socket, const char *, void **);*/

/*NNG_DECL int nng_getopt_string(nng_socket, const char *, char **);*/

/*NNG_DECL int nng_socket_set(nng_socket, const char *, const void *, size_t);*/
/*NNG_DECL int nng_socket_set_bool(nng_socket, const char *, bool);*/
/*NNG_DECL int nng_socket_set_int(nng_socket, const char *, int);*/
/*NNG_DECL int nng_socket_set_size(nng_socket, const char *, size_t);*/
/*NNG_DECL int nng_socket_set_uint64(nng_socket, const char *, uint64_t);*/
/*NNG_DECL int nng_socket_set_string(nng_socket, const char *, const char *);*/
/*NNG_DECL int nng_socket_set_ptr(nng_socket, const char *, void *);*/
/*NNG_DECL int nng_socket_set_ms(nng_socket, const char *, nng_duration);*/
/*NNG_DECL int nng_socket_set_addr(
    nng_socket, const char *, const nng_sockaddr *);*/

/*NNG_DECL int nng_socket_get(nng_socket, const char *, void *, size_t *);*/
/*NNG_DECL int nng_socket_get_bool(nng_socket, const char *, bool *);*/
/*NNG_DECL int nng_socket_get_int(nng_socket, const char *, int *);*/
/*NNG_DECL int nng_socket_get_size(nng_socket, const char *, size_t *);*/
/*NNG_DECL int nng_socket_get_uint64(nng_socket, const char *, uint64_t *);*/
/*NNG_DECL int nng_socket_get_string(nng_socket, const char *, char **);*/
/*NNG_DECL int nng_socket_get_ptr(nng_socket, const char *, void **);*/
/*NNG_DECL int nng_socket_get_ms(nng_socket, const char *, nng_duration *);*/
/*NNG_DECL int nng_socket_get_addr(nng_socket, const char *, nng_sockaddr *);*/

typedef enum {
	NNG_PIPE_EV_ADD_PRE,
	NNG_PIPE_EV_ADD_POST,
	NNG_PIPE_EV_REM_POST,
	NNG_PIPE_EV_NUM,
} nng_pipe_ev;

typedef void (*nng_pipe_cb)(nng_pipe, nng_pipe_ev, void *);

/*NNG_DECL int nng_pipe_notify(nng_socket, nng_pipe_ev, nng_pipe_cb, void *);*/

/*NNG_DECL int nng_listen(nng_socket, const char *, nng_listener *, int);*/

/*NNG_DECL int nng_dial(nng_socket, const char *, nng_dialer *, int);*/

/*NNG_DECL int nng_dialer_create(nng_dialer *, nng_socket, const char *);*/

/*NNG_DECL int nng_listener_create(nng_listener *, nng_socket, const char *);*/

/*NNG_DECL int nng_dialer_start(nng_dialer, int);*/

/*NNG_DECL int nng_listener_start(nng_listener, int);*/

/*NNG_DECL int nng_dialer_close(nng_dialer);*/

/*NNG_DECL int nng_listener_close(nng_listener);*/

/*NNG_DECL int nng_dialer_id(nng_dialer);*/

/*NNG_DECL int nng_listener_id(nng_listener);*/

/*NNG_DECL int nng_dialer_setopt(nng_dialer, const char *, const void *, size_t);*/
/*NNG_DECL int nng_dialer_setopt_bool(nng_dialer, const char *, bool);*/
/*NNG_DECL int nng_dialer_setopt_int(nng_dialer, const char *, int);*/
/*NNG_DECL int nng_dialer_setopt_ms(nng_dialer, const char *, nng_duration);*/
/*NNG_DECL int nng_dialer_setopt_size(nng_dialer, const char *, size_t);*/
/*NNG_DECL int nng_dialer_setopt_uint64(nng_dialer, const char *, uint64_t);*/
/*NNG_DECL int nng_dialer_setopt_ptr(nng_dialer, const char *, void *);*/
/*NNG_DECL int nng_dialer_setopt_string(nng_dialer, const char *, const char *);*/

/*NNG_DECL int nng_dialer_getopt(nng_dialer, const char *, void *, size_t *);*/
/*NNG_DECL int nng_dialer_getopt_bool(nng_dialer, const char *, bool *);*/
/*NNG_DECL int nng_dialer_getopt_int(nng_dialer, const char *, int *);*/
/*NNG_DECL int nng_dialer_getopt_ms(nng_dialer, const char *, nng_duration *);*/
/*NNG_DECL int nng_dialer_getopt_size(nng_dialer, const char *, size_t *);*/
/*NNG_DECL int nng_dialer_getopt_sockaddr(
    nng_dialer, const char *, nng_sockaddr *);*/
/*NNG_DECL int nng_dialer_getopt_uint64(nng_dialer, const char *, uint64_t *);*/
/*NNG_DECL int nng_dialer_getopt_ptr(nng_dialer, const char *, void **);*/

/*NNG_DECL int nng_dialer_getopt_string(nng_dialer, const char *, char **);*/

/*NNG_DECL int nng_dialer_set(nng_dialer, const char *, const void *, size_t);*/
/*NNG_DECL int nng_dialer_set_bool(nng_dialer, const char *, bool);*/
/*NNG_DECL int nng_dialer_set_int(nng_dialer, const char *, int);*/
/*NNG_DECL int nng_dialer_set_size(nng_dialer, const char *, size_t);*/
/*NNG_DECL int nng_dialer_set_uint64(nng_dialer, const char *, uint64_t);*/
/*NNG_DECL int nng_dialer_set_string(nng_dialer, const char *, const char *);*/
/*NNG_DECL int nng_dialer_set_ptr(nng_dialer, const char *, void *);*/
/*NNG_DECL int nng_dialer_set_ms(nng_dialer, const char *, nng_duration);*/
/*NNG_DECL int nng_dialer_set_addr(
    nng_dialer, const char *, const nng_sockaddr *);*/

/*NNG_DECL int nng_dialer_get(nng_dialer, const char *, void *, size_t *);*/
/*NNG_DECL int nng_dialer_get_bool(nng_dialer, const char *, bool *);*/
/*NNG_DECL int nng_dialer_get_int(nng_dialer, const char *, int *);*/
/*NNG_DECL int nng_dialer_get_size(nng_dialer, const char *, size_t *);*/
/*NNG_DECL int nng_dialer_get_uint64(nng_dialer, const char *, uint64_t *);*/
/*NNG_DECL int nng_dialer_get_string(nng_dialer, const char *, char **);*/
/*NNG_DECL int nng_dialer_get_ptr(nng_dialer, const char *, void **);*/
/*NNG_DECL int nng_dialer_get_ms(nng_dialer, const char *, nng_duration *);*/
/*NNG_DECL int nng_dialer_get_addr(nng_dialer, const char *, nng_sockaddr *);*/

/*NNG_DECL int nng_listener_setopt(
    nng_listener, const char *, const void *, size_t);*/
/*NNG_DECL int nng_listener_setopt_bool(nng_listener, const char *, bool);*/
/*NNG_DECL int nng_listener_setopt_int(nng_listener, const char *, int);*/
/*NNG_DECL int nng_listener_setopt_ms(nng_listener, const char *, nng_duration);*/
/*NNG_DECL int nng_listener_setopt_size(nng_listener, const char *, size_t);*/
/*NNG_DECL int nng_listener_setopt_uint64(nng_listener, const char *, uint64_t);*/
/*NNG_DECL int nng_listener_setopt_ptr(nng_listener, const char *, void *);*/
/*NNG_DECL int nng_listener_setopt_string(
    nng_listener, const char *, const char *);*/

/*NNG_DECL int nng_listener_getopt(nng_listener, const char *, void *, size_t *);*/
/*NNG_DECL int nng_listener_getopt_bool(nng_listener, const char *, bool *);*/
/*NNG_DECL int nng_listener_getopt_int(nng_listener, const char *, int *);*/
/*NNG_DECL int nng_listener_getopt_ms(
    nng_listener, const char *, nng_duration *);*/
/*NNG_DECL int nng_listener_getopt_size(nng_listener, const char *, size_t *);*/
/*NNG_DECL int nng_listener_getopt_sockaddr(
    nng_listener, const char *, nng_sockaddr *);*/
/*NNG_DECL int nng_listener_getopt_uint64(
    nng_listener, const char *, uint64_t *);*/
/*NNG_DECL int nng_listener_getopt_ptr(nng_listener, const char *, void **);*/

/*NNG_DECL int nng_listener_getopt_string(nng_listener, const char *, char **);*/

/*NNG_DECL int nng_listener_set(
    nng_listener, const char *, const void *, size_t);*/
/*NNG_DECL int nng_listener_set_bool(nng_listener, const char *, bool);*/
/*NNG_DECL int nng_listener_set_int(nng_listener, const char *, int);*/
/*NNG_DECL int nng_listener_set_size(nng_listener, const char *, size_t);*/
/*NNG_DECL int nng_listener_set_uint64(nng_listener, const char *, uint64_t);*/
/*NNG_DECL int nng_listener_set_string(nng_listener, const char *, const char *);*/
/*NNG_DECL int nng_listener_set_ptr(nng_listener, const char *, void *);*/
/*NNG_DECL int nng_listener_set_ms(nng_listener, const char *, nng_duration);*/
/*NNG_DECL int nng_listener_set_addr(
    nng_listener, const char *, const nng_sockaddr *);*/

/*NNG_DECL int nng_listener_get(nng_listener, const char *, void *, size_t *);*/
/*NNG_DECL int nng_listener_get_bool(nng_listener, const char *, bool *);*/
/*NNG_DECL int nng_listener_get_int(nng_listener, const char *, int *);*/
/*NNG_DECL int nng_listener_get_size(nng_listener, const char *, size_t *);*/
/*NNG_DECL int nng_listener_get_uint64(nng_listener, const char *, uint64_t *);*/
/*NNG_DECL int nng_listener_get_string(nng_listener, const char *, char **);*/
/*NNG_DECL int nng_listener_get_ptr(nng_listener, const char *, void **);*/
/*NNG_DECL int nng_listener_get_ms(nng_listener, const char *, nng_duration *);*/
/*NNG_DECL int nng_listener_get_addr(nng_listener, const char *, nng_sockaddr *);*/

/*NNG_DECL const char *nng_strerror(int);*/

/*NNG_DECL int nng_send(nng_socket, void *, size_t, int);*/

/*NNG_DECL int nng_recv(nng_socket, void *, size_t *, int);*/

/*NNG_DECL int nng_sendmsg(nng_socket, nng_msg *, int);*/

/*NNG_DECL int nng_recvmsg(nng_socket, nng_msg **, int);*/

/*NNG_DECL void nng_send_aio(nng_socket, nng_aio *);*/

/*NNG_DECL void nng_recv_aio(nng_socket, nng_aio *);*/


/*NNG_DECL int nng_ctx_open(nng_ctx *, nng_socket);*/

/*NNG_DECL int nng_ctx_close(nng_ctx);*/

/*NNG_DECL int nng_ctx_id(nng_ctx);*/

/*NNG_DECL void nng_ctx_recv(nng_ctx, nng_aio *);*/

/*NNG_DECL void nng_ctx_send(nng_ctx, nng_aio *);*/

/*NNG_DECL int nng_ctx_getopt(nng_ctx, const char *, void *, size_t *);*/
/*NNG_DECL int nng_ctx_getopt_bool(nng_ctx, const char *, bool *);*/
/*NNG_DECL int nng_ctx_getopt_int(nng_ctx, const char *, int *);*/
/*NNG_DECL int nng_ctx_getopt_ms(nng_ctx, const char *, nng_duration *);*/
/*NNG_DECL int nng_ctx_getopt_size(nng_ctx, const char *, size_t *);*/

/*NNG_DECL int nng_ctx_setopt(nng_ctx, const char *, const void *, size_t);*/
/*NNG_DECL int nng_ctx_setopt_bool(nng_ctx, const char *, bool);*/
/*NNG_DECL int nng_ctx_setopt_int(nng_ctx, const char *, int);*/
/*NNG_DECL int nng_ctx_setopt_ms(nng_ctx, const char *, nng_duration);*/
/*NNG_DECL int nng_ctx_setopt_size(nng_ctx, const char *, size_t);*/

/*NNG_DECL int nng_ctx_get(nng_ctx, const char *, void *, size_t *);*/
/*NNG_DECL int nng_ctx_get_bool(nng_ctx, const char *, bool *);*/
/*NNG_DECL int nng_ctx_get_int(nng_ctx, const char *, int *);*/
/*NNG_DECL int nng_ctx_get_size(nng_ctx, const char *, size_t *);*/
/*NNG_DECL int nng_ctx_get_uint64(nng_ctx, const char *, uint64_t *);*/
/*NNG_DECL int nng_ctx_get_string(nng_ctx, const char *, char **);*/
/*NNG_DECL int nng_ctx_get_ptr(nng_ctx, const char *, void **);*/
/*NNG_DECL int nng_ctx_get_ms(nng_ctx, const char *, nng_duration *);*/
/*NNG_DECL int nng_ctx_get_addr(nng_ctx, const char *, nng_sockaddr *);*/

/*NNG_DECL int nng_ctx_set(nng_ctx, const char *, const void *, size_t);*/
/*NNG_DECL int nng_ctx_set_bool(nng_ctx, const char *, bool);*/
/*NNG_DECL int nng_ctx_set_int(nng_ctx, const char *, int);*/
/*NNG_DECL int nng_ctx_set_size(nng_ctx, const char *, size_t);*/
/*NNG_DECL int nng_ctx_set_uint64(nng_ctx, const char *, uint64_t);*/
/*NNG_DECL int nng_ctx_set_string(nng_ctx, const char *, const char *);*/
/*NNG_DECL int nng_ctx_set_ptr(nng_ctx, const char *, void *);*/
/*NNG_DECL int nng_ctx_set_ms(nng_ctx, const char *, nng_duration);*/
/*NNG_DECL int nng_ctx_set_addr(nng_ctx, const char *, const nng_sockaddr *);*/

/*NNG_DECL void *nng_alloc(size_t);*/

/*NNG_DECL void nng_free(void *, size_t);*/

/*NNG_DECL char *nng_strdup(const char *);*/

/*NNG_DECL void nng_strfree(char *);*/


/*NNG_DECL int nng_aio_alloc(nng_aio **, void (*)(void *), void *);*/

/*NNG_DECL void nng_aio_free(nng_aio *);*/

/*NNG_DECL void nng_aio_stop(nng_aio *);*/

/*NNG_DECL int nng_aio_result(nng_aio *);*/

/*NNG_DECL size_t nng_aio_count(nng_aio *);*/

/*NNG_DECL void nng_aio_cancel(nng_aio *);*/

/*NNG_DECL void nng_aio_abort(nng_aio *, int);*/

/*NNG_DECL void nng_aio_wait(nng_aio *);*/

/*NNG_DECL void nng_aio_set_msg(nng_aio *, nng_msg *);*/

/*NNG_DECL nng_msg *nng_aio_get_msg(nng_aio *);*/

/*NNG_DECL int nng_aio_set_input(nng_aio *, unsigned, void *);*/

/*NNG_DECL void *nng_aio_get_input(nng_aio *, unsigned);*/

/*NNG_DECL int nng_aio_set_output(nng_aio *, unsigned, void *);*/

/*NNG_DECL void *nng_aio_get_output(nng_aio *, unsigned);*/

/*NNG_DECL void nng_aio_set_timeout(nng_aio *, nng_duration);*/

/*NNG_DECL int nng_aio_set_iov(nng_aio *, unsigned, const nng_iov *);*/

/*NNG_DECL bool nng_aio_begin(nng_aio *);*/

/*NNG_DECL void nng_aio_finish(nng_aio *, int);*/

typedef void (*nng_aio_cancelfn)(nng_aio *, void *, int);
/*NNG_DECL void nng_aio_defer(nng_aio *, nng_aio_cancelfn, void *);*/

/*NNG_DECL void nng_sleep_aio(nng_duration, nng_aio *);*/

/*NNG_DECL int      nng_msg_alloc(nng_msg **, size_t);*/
/*NNG_DECL void     nng_msg_free(nng_msg *);*/
/*NNG_DECL int      nng_msg_realloc(nng_msg *, size_t);*/
/*NNG_DECL void *   nng_msg_header(nng_msg *);*/
/*NNG_DECL size_t   nng_msg_header_len(const nng_msg *);*/
/*NNG_DECL void *   nng_msg_body(nng_msg *);*/
/*NNG_DECL size_t   nng_msg_len(const nng_msg *);*/
/*NNG_DECL int      nng_msg_append(nng_msg *, const void *, size_t);*/
/*NNG_DECL int      nng_msg_insert(nng_msg *, const void *, size_t);*/
/*NNG_DECL int      nng_msg_trim(nng_msg *, size_t);*/
/*NNG_DECL int      nng_msg_chop(nng_msg *, size_t);*/
/*NNG_DECL int      nng_msg_header_append(nng_msg *, const void *, size_t);*/
/*NNG_DECL int      nng_msg_header_insert(nng_msg *, const void *, size_t);*/
/*NNG_DECL int      nng_msg_header_trim(nng_msg *, size_t);*/
/*NNG_DECL int      nng_msg_header_chop(nng_msg *, size_t);*/
/*NNG_DECL int      nng_msg_header_append_u16(nng_msg *, uint16_t);*/
/*NNG_DECL int      nng_msg_header_append_u32(nng_msg *, uint32_t);*/
/*NNG_DECL int      nng_msg_header_append_u64(nng_msg *, uint64_t);*/
/*NNG_DECL int      nng_msg_header_insert_u16(nng_msg *, uint16_t);*/
/*NNG_DECL int      nng_msg_header_insert_u32(nng_msg *, uint32_t);*/
/*NNG_DECL int      nng_msg_header_insert_u64(nng_msg *, uint64_t);*/
/*NNG_DECL int      nng_msg_header_chop_u16(nng_msg *, uint16_t *);*/
/*NNG_DECL int      nng_msg_header_chop_u32(nng_msg *, uint32_t *);*/
/*NNG_DECL int      nng_msg_header_chop_u64(nng_msg *, uint64_t *);*/
/*NNG_DECL int      nng_msg_header_trim_u16(nng_msg *, uint16_t *);*/
/*NNG_DECL int      nng_msg_header_trim_u32(nng_msg *, uint32_t *);*/
/*NNG_DECL int      nng_msg_header_trim_u64(nng_msg *, uint64_t *);*/
/*NNG_DECL int      nng_msg_append_u16(nng_msg *, uint16_t);*/
/*NNG_DECL int      nng_msg_append_u32(nng_msg *, uint32_t);*/
/*NNG_DECL int      nng_msg_append_u64(nng_msg *, uint64_t);*/
/*NNG_DECL int      nng_msg_insert_u16(nng_msg *, uint16_t);*/
/*NNG_DECL int      nng_msg_insert_u32(nng_msg *, uint32_t);*/
/*NNG_DECL int      nng_msg_insert_u64(nng_msg *, uint64_t);*/
/*NNG_DECL int      nng_msg_chop_u16(nng_msg *, uint16_t *);*/
/*NNG_DECL int      nng_msg_chop_u32(nng_msg *, uint32_t *);*/
/*NNG_DECL int      nng_msg_chop_u64(nng_msg *, uint64_t *);*/
/*NNG_DECL int      nng_msg_trim_u16(nng_msg *, uint16_t *);*/
/*NNG_DECL int      nng_msg_trim_u32(nng_msg *, uint32_t *);*/
/*NNG_DECL int      nng_msg_trim_u64(nng_msg *, uint64_t *);*/
/*NNG_DECL int      nng_msg_dup(nng_msg **, const nng_msg *);*/
/*NNG_DECL void     nng_msg_clear(nng_msg *);*/
/*NNG_DECL void     nng_msg_header_clear(nng_msg *);*/
/*NNG_DECL void     nng_msg_set_pipe(nng_msg *, nng_pipe);*/
/*NNG_DECL nng_pipe nng_msg_get_pipe(const nng_msg *);*/

/*NNG_DECL int nng_msg_getopt(nng_msg *, int, void *, size_t *);*/

/*NNG_DECL int nng_pipe_getopt(nng_pipe, const char *, void *, size_t *);*/
/*NNG_DECL int nng_pipe_getopt_bool(nng_pipe, const char *, bool *);*/
/*NNG_DECL int nng_pipe_getopt_int(nng_pipe, const char *, int *);*/
/*NNG_DECL int nng_pipe_getopt_ms(nng_pipe, const char *, nng_duration *);*/
/*NNG_DECL int nng_pipe_getopt_size(nng_pipe, const char *, size_t *);*/
/*NNG_DECL int nng_pipe_getopt_sockaddr(nng_pipe, const char *, nng_sockaddr *);*/
/*NNG_DECL int nng_pipe_getopt_uint64(nng_pipe, const char *, uint64_t *);*/
/*NNG_DECL int nng_pipe_getopt_ptr(nng_pipe, const char *, void **);*/
/*NNG_DECL int nng_pipe_getopt_string(nng_pipe, const char *, char **);*/

/*NNG_DECL int nng_pipe_get(nng_pipe, const char *, void *, size_t *);*/
/*NNG_DECL int nng_pipe_get_bool(nng_pipe, const char *, bool *);*/
/*NNG_DECL int nng_pipe_get_int(nng_pipe, const char *, int *);*/
/*NNG_DECL int nng_pipe_get_ms(nng_pipe, const char *, nng_duration *);*/
/*NNG_DECL int nng_pipe_get_size(nng_pipe, const char *, size_t *);*/
/*NNG_DECL int nng_pipe_get_uint64(nng_pipe, const char *, uint64_t *);*/
/*NNG_DECL int nng_pipe_get_string(nng_pipe, const char *, char **);*/
/*NNG_DECL int nng_pipe_get_ptr(nng_pipe, const char *, void **);*/
/*NNG_DECL int nng_pipe_get_addr(nng_pipe, const char *, nng_sockaddr *);*/

/*NNG_DECL int          nng_pipe_close(nng_pipe);*/
/*NNG_DECL int          nng_pipe_id(nng_pipe);*/
/*NNG_DECL nng_socket   nng_pipe_socket(nng_pipe);*/
/*NNG_DECL nng_dialer   nng_pipe_dialer(nng_pipe);*/
/*NNG_DECL nng_listener nng_pipe_listener(nng_pipe);*/

enum nng_flag_enum {
	NNG_FLAG_ALLOC    = 1,
	NNG_FLAG_NONBLOCK = 2
};

#define NNG_OPT_SOCKNAME "socket-name"
#define NNG_OPT_RAW "raw"
#define NNG_OPT_PROTO "protocol"
#define NNG_OPT_PROTONAME "protocol-name"
#define NNG_OPT_PEER "peer"
#define NNG_OPT_PEERNAME "peer-name"
#define NNG_OPT_RECVBUF "recv-buffer"
#define NNG_OPT_SENDBUF "send-buffer"
#define NNG_OPT_RECVFD "recv-fd"
#define NNG_OPT_SENDFD "send-fd"
#define NNG_OPT_RECVTIMEO "recv-timeout"
#define NNG_OPT_SENDTIMEO "send-timeout"
#define NNG_OPT_LOCADDR "local-address"
#define NNG_OPT_REMADDR "remote-address"
#define NNG_OPT_URL "url"
#define NNG_OPT_MAXTTL "ttl-max"
#define NNG_OPT_RECVMAXSZ "recv-size-max"
#define NNG_OPT_RECONNMINT "reconnect-time-min"
#define NNG_OPT_RECONNMAXT "reconnect-time-max"


#define NNG_OPT_TLS_CONFIG "tls-config"

#define NNG_OPT_TLS_AUTH_MODE "tls-authmode"

#define NNG_OPT_TLS_CERT_KEY_FILE "tls-cert-key-file"

#define NNG_OPT_TLS_CA_FILE "tls-ca-file"

#define NNG_OPT_TLS_SERVER_NAME "tls-server-name"

#define NNG_OPT_TLS_VERIFIED "tls-verified"


#define NNG_OPT_TCP_NODELAY "tcp-nodelay"

#define NNG_OPT_TCP_KEEPALIVE "tcp-keepalive"

#define NNG_OPT_TCP_BOUND_PORT "tcp-bound-port"


#define NNG_OPT_IPC_SECURITY_DESCRIPTOR "ipc:security-descriptor"

#define NNG_OPT_IPC_PERMISSIONS "ipc:permissions"

#define NNG_OPT_IPC_PEER_UID "ipc:peer-uid"

#define NNG_OPT_IPC_PEER_GID "ipc:peer-gid"

#define NNG_OPT_IPC_PEER_PID "ipc:peer-pid"

#define NNG_OPT_IPC_PEER_ZONEID "ipc:peer-zoneid"


#define NNG_OPT_WS_REQUEST_HEADERS "ws:request-headers"

#define NNG_OPT_WS_RESPONSE_HEADERS "ws:response-headers"

#define NNG_OPT_WS_RESPONSE_HEADER "ws:response-header:"

#define NNG_OPT_WS_REQUEST_HEADER "ws:request-header:"

#define NNG_OPT_WS_REQUEST_URI "ws:request-uri"

#define NNG_OPT_WS_SENDMAXFRAME "ws:txframe-max"

#define NNG_OPT_WS_RECVMAXFRAME "ws:rxframe-max"

#define NNG_OPT_WS_PROTOCOL "ws:protocol"



/*NNG_DECL int nng_stats_get(nng_stat **);*/

/*NNG_DECL void nng_stats_free(nng_stat *);*/

/*NNG_DECL void nng_stats_dump(nng_stat *);*/

/*NNG_DECL nng_stat *nng_stat_next(nng_stat *);*/

/*NNG_DECL nng_stat *nng_stat_child(nng_stat *);*/

/*NNG_DECL const char *nng_stat_name(nng_stat *);*/

/*NNG_DECL int nng_stat_type(nng_stat *);*/

/*NNG_DECL nng_stat *nng_stat_find(nng_stat *, const char *);*/

/*NNG_DECL nng_stat *nng_stat_find_socket(nng_stat *, nng_socket);*/

/*NNG_DECL nng_stat *nng_stat_find_dialer(nng_stat *, nng_dialer);*/

/*NNG_DECL nng_stat *nng_stat_find_listener(nng_stat *, nng_listener);*/

enum nng_stat_type_enum {
	NNG_STAT_SCOPE   = 0,
	NNG_STAT_LEVEL   = 1,
	NNG_STAT_COUNTER = 2,
	NNG_STAT_STRING  = 3,
	NNG_STAT_BOOLEAN = 4,
	NNG_STAT_ID      = 5,
};

/*NNG_DECL int nng_stat_unit(nng_stat *);*/

enum nng_unit_enum {
	NNG_UNIT_NONE     = 0,
	NNG_UNIT_BYTES    = 1,
	NNG_UNIT_MESSAGES = 2,
	NNG_UNIT_MILLIS   = 3,
	NNG_UNIT_EVENTS   = 4
};

/*NNG_DECL uint64_t nng_stat_value(nng_stat *);*/

/*NNG_DECL const char *nng_stat_string(nng_stat *);*/

/*NNG_DECL const char *nng_stat_desc(nng_stat *);*/

/*NNG_DECL uint64_t nng_stat_timestamp(nng_stat *);*/

/*NNG_DECL int nng_device(nng_socket, nng_socket);*/



enum nng_errno_enum {
	NNG_EINTR        = 1,
	NNG_ENOMEM       = 2,
	NNG_EINVAL       = 3,
	NNG_EBUSY        = 4,
	NNG_ETIMEDOUT    = 5,
	NNG_ECONNREFUSED = 6,
	NNG_ECLOSED      = 7,
	NNG_EAGAIN       = 8,
	NNG_ENOTSUP      = 9,
	NNG_EADDRINUSE   = 10,
	NNG_ESTATE       = 11,
	NNG_ENOENT       = 12,
	NNG_EPROTO       = 13,
	NNG_EUNREACHABLE = 14,
	NNG_EADDRINVAL   = 15,
	NNG_EPERM        = 16,
	NNG_EMSGSIZE     = 17,
	NNG_ECONNABORTED = 18,
	NNG_ECONNRESET   = 19,
	NNG_ECANCELED    = 20,
	NNG_ENOFILES     = 21,
	NNG_ENOSPC       = 22,
	NNG_EEXIST       = 23,
	NNG_EREADONLY    = 24,
	NNG_EWRITEONLY   = 25,
	NNG_ECRYPTO      = 26,
	NNG_EPEERAUTH    = 27,
	NNG_ENOARG       = 28,
	NNG_EAMBIGUOUS   = 29,
	NNG_EBADTYPE     = 30,
	NNG_ECONNSHUT    = 31,
	NNG_EINTERNAL    = 1000,
	NNG_ESYSERR      = 0x10000000,
	NNG_ETRANERR     = 0x20000000
};


typedef struct nng_url {
	char *u_rawurl;
	char *u_scheme;
	char *u_userinfo;
	char *u_host;
	char *u_hostname;
	char *u_port;
	char *u_path;
	char *u_query;
	char *u_fragment;
	char *u_requri;
} nng_url;

/*NNG_DECL int nng_url_parse(nng_url **, const char *);*/

/*NNG_DECL void nng_url_free(nng_url *);*/

/*NNG_DECL int nng_url_clone(nng_url **, const nng_url *);*/

/*NNG_DECL const char *nng_version(void);*/


typedef struct nng_stream          nng_stream;
typedef struct nng_stream_dialer   nng_stream_dialer;
typedef struct nng_stream_listener nng_stream_listener;

/*NNG_DECL void nng_stream_free(nng_stream *);*/
/*NNG_DECL void nng_stream_close(nng_stream *);*/
/*NNG_DECL void nng_stream_send(nng_stream *, nng_aio *);*/
/*NNG_DECL void nng_stream_recv(nng_stream *, nng_aio *);*/
/*NNG_DECL int  nng_stream_get(nng_stream *, const char *, void *, size_t *);*/
/*NNG_DECL int  nng_stream_get_bool(nng_stream *, const char *, bool *);*/
/*NNG_DECL int  nng_stream_get_int(nng_stream *, const char *, int *);*/
/*NNG_DECL int  nng_stream_get_ms(nng_stream *, const char *, nng_duration *);*/
/*NNG_DECL int  nng_stream_get_size(nng_stream *, const char *, size_t *);*/
/*NNG_DECL int  nng_stream_get_uint64(nng_stream *, const char *, uint64_t *);*/
/*NNG_DECL int  nng_stream_get_string(nng_stream *, const char *, char **);*/
/*NNG_DECL int  nng_stream_get_ptr(nng_stream *, const char *, void **);*/
/*NNG_DECL int  nng_stream_get_addr(nng_stream *, const char *, nng_sockaddr *);*/
/*NNG_DECL int  nng_stream_set(nng_stream *, const char *, const void *, size_t);*/
/*NNG_DECL int  nng_stream_set_bool(nng_stream *, const char *, bool);*/
/*NNG_DECL int  nng_stream_set_int(nng_stream *, const char *, int);*/
/*NNG_DECL int  nng_stream_set_ms(nng_stream *, const char *, nng_duration);*/
/*NNG_DECL int  nng_stream_set_size(nng_stream *, const char *, size_t);*/
/*NNG_DECL int  nng_stream_set_uint64(nng_stream *, const char *, uint64_t);*/
/*NNG_DECL int  nng_stream_set_string(nng_stream *, const char *, const char *);*/
/*NNG_DECL int  nng_stream_set_ptr(nng_stream *, const char *, void *);*/
/*NNG_DECL int  nng_stream_set_addr(
     nng_stream *, const char *, const nng_sockaddr *);*/

/*NNG_DECL int nng_stream_dialer_alloc(nng_stream_dialer **, const char *);*/
/*NNG_DECL int nng_stream_dialer_alloc_url(
    nng_stream_dialer **, const nng_url *);*/
/*NNG_DECL void nng_stream_dialer_free(nng_stream_dialer *);*/
/*NNG_DECL void nng_stream_dialer_close(nng_stream_dialer *);*/
/*NNG_DECL void nng_stream_dialer_dial(nng_stream_dialer *, nng_aio *);*/
/*NNG_DECL int  nng_stream_dialer_set(
     nng_stream_dialer *, const char *, const void *, size_t);*/
/*NNG_DECL int nng_stream_dialer_get(
    nng_stream_dialer *, const char *, void *, size_t *);*/
/*NNG_DECL int nng_stream_dialer_get_bool(
    nng_stream_dialer *, const char *, bool *);*/
/*NNG_DECL int nng_stream_dialer_get_int(
    nng_stream_dialer *, const char *, int *);*/
/*NNG_DECL int nng_stream_dialer_get_ms(
    nng_stream_dialer *, const char *, nng_duration *);*/
/*NNG_DECL int nng_stream_dialer_get_size(
    nng_stream_dialer *, const char *, size_t *);*/
/*NNG_DECL int nng_stream_dialer_get_uint64(
    nng_stream_dialer *, const char *, uint64_t *);*/
/*NNG_DECL int nng_stream_dialer_get_string(
    nng_stream_dialer *, const char *, char **);*/
/*NNG_DECL int nng_stream_dialer_get_ptr(
    nng_stream_dialer *, const char *, void **);*/
/*NNG_DECL int nng_stream_dialer_get_addr(
    nng_stream_dialer *, const char *, nng_sockaddr *);*/
/*NNG_DECL int nng_stream_dialer_set_bool(
    nng_stream_dialer *, const char *, bool);*/
/*NNG_DECL int nng_stream_dialer_set_int(nng_stream_dialer *, const char *, int);*/
/*NNG_DECL int nng_stream_dialer_set_ms(
    nng_stream_dialer *, const char *, nng_duration);*/
/*NNG_DECL int nng_stream_dialer_set_size(
    nng_stream_dialer *, const char *, size_t);*/
/*NNG_DECL int nng_stream_dialer_set_uint64(
    nng_stream_dialer *, const char *, uint64_t);*/
/*NNG_DECL int nng_stream_dialer_set_string(
    nng_stream_dialer *, const char *, const char *);*/
/*NNG_DECL int nng_stream_dialer_set_ptr(
    nng_stream_dialer *, const char *, void *);*/
/*NNG_DECL int nng_stream_dialer_set_addr(
    nng_stream_dialer *, const char *, const nng_sockaddr *);*/

/*NNG_DECL int nng_stream_listener_alloc(nng_stream_listener **, const char *);*/
/*NNG_DECL int nng_stream_listener_alloc_url(
    nng_stream_listener **, const nng_url *);*/
/*NNG_DECL void nng_stream_listener_free(nng_stream_listener *);*/
/*NNG_DECL void nng_stream_listener_close(nng_stream_listener *);*/
/*NNG_DECL int  nng_stream_listener_listen(nng_stream_listener *);*/
/*NNG_DECL void nng_stream_listener_accept(nng_stream_listener *, nng_aio *);*/
/*NNG_DECL int  nng_stream_listener_set(
     nng_stream_listener *, const char *, const void *, size_t);*/
/*NNG_DECL int nng_stream_listener_get(
    nng_stream_listener *, const char *, void *, size_t *);*/
/*NNG_DECL int nng_stream_listener_get_bool(
    nng_stream_listener *, const char *, bool *);*/
/*NNG_DECL int nng_stream_listener_get_int(
    nng_stream_listener *, const char *, int *);*/
/*NNG_DECL int nng_stream_listener_get_ms(
    nng_stream_listener *, const char *, nng_duration *);*/
/*NNG_DECL int nng_stream_listener_get_size(
    nng_stream_listener *, const char *, size_t *);*/
/*NNG_DECL int nng_stream_listener_get_uint64(
    nng_stream_listener *, const char *, uint64_t *);*/
/*NNG_DECL int nng_stream_listener_get_string(
    nng_stream_listener *, const char *, char **);*/
/*NNG_DECL int nng_stream_listener_get_ptr(
    nng_stream_listener *, const char *, void **);*/
/*NNG_DECL int nng_stream_listener_get_addr(
    nng_stream_listener *, const char *, nng_sockaddr *);*/
/*NNG_DECL int nng_stream_listener_set_bool(
    nng_stream_listener *, const char *, bool);*/
/*NNG_DECL int nng_stream_listener_set_int(
    nng_stream_listener *, const char *, int);*/
/*NNG_DECL int nng_stream_listener_set_ms(
    nng_stream_listener *, const char *, nng_duration);*/
/*NNG_DECL int nng_stream_listener_set_size(
    nng_stream_listener *, const char *, size_t);*/
/*NNG_DECL int nng_stream_listener_set_uint64(
    nng_stream_listener *, const char *, uint64_t);*/
/*NNG_DECL int nng_stream_listener_set_string(
    nng_stream_listener *, const char *, const char *);*/
/*NNG_DECL int nng_stream_listener_set_ptr(
    nng_stream_listener *, const char *, void *);*/
/*NNG_DECL int nng_stream_listener_set_addr(
    nng_stream_listener *, const char *, const nng_sockaddr *);*/

#ifdef __cplusplus
}
#endif

#endif // NNG_NNG_H