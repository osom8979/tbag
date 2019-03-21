#ifndef NNG_H
#define NNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NNG_MAJOR_VERSION 1
#define NNG_MINOR_VERSION 1
#define NNG_PATCH_VERSION 1
#define NNG_RELEASE_SUFFIX ""

#define NNG_MAXADDRLEN (128)

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

// Initializers.
// clang-format off
#define NNG_PIPE_INITIALIZER { 0 }
#define NNG_SOCKET_INITIALIZER { 0 }
#define NNG_DIALER_INITIALIZER { 0 }
#define NNG_LISTENER_INITIALIZER { 0 }
#define NNG_CTX_INITIALIZER { 0 }
// clang-format on

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

typedef enum {
    NNG_PIPE_EV_ADD_PRE,
    NNG_PIPE_EV_ADD_POST,
    NNG_PIPE_EV_REM_POST,
    NNG_PIPE_EV_NUM,
} nng_pipe_ev;

typedef void (*nng_pipe_cb)(nng_pipe, int, void *);

typedef void (*nng_aio_cancelfn)(nng_aio *, void *, int);

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

enum nng_stat_type_enum {
    NNG_STAT_SCOPE   = 0,
    NNG_STAT_LEVEL   = 1,
    NNG_STAT_COUNTER = 2,
    NNG_STAT_STRING  = 3,
    NNG_STAT_BOOLEAN = 4,
    NNG_STAT_ID      = 5,
};

enum nng_unit_enum {
    NNG_UNIT_NONE     = 0,
    NNG_UNIT_BYTES    = 1,
    NNG_UNIT_MESSAGES = 2,
    NNG_UNIT_MILLIS   = 3,
    NNG_UNIT_EVENTS   = 4
};

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

#ifdef __cplusplus
}
#endif

#endif // NNG_H

