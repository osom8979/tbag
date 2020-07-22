
#ifndef NNG_COMPAT_NN_H
#define NNG_COMPAT_NN_H




#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stddef.h>
#include <stdint.h>


#ifndef NN_DECL
#if defined(_WIN32) && !defined(NNG_STATIC_LIB)
#if defined(NNG_SHARED_LIB)
#define NN_DECL		__declspec(dllexport)
#else
#define NN_DECL		__declspec(dllimport)
#endif // NNG_SHARED_LIB
#else
#if defined(NNG_SHARED_LIB) && defined(NNG_HIDDEN_VISIBILITY)
#define NN_DECL __attribute__((visibility("default")))
#else
#define NN_DECL extern
#endif
#endif // _WIN32 && !NNG_STATIC_LIB
#endif  // NN_DECL

#define AF_SP			1
#define AF_SP_RAW		2

#define NN_SOCKADDR_MAX		128
#define NN_SOL_SOCKET		0

#define NN_DONTWAIT		1

#define PROTO_SP		1
#define SP_HDR			1

#define NN_ERRBASE		(0x10000000)
#ifndef ENOTSUP
#define ENOTSUP			(NN_ERRBASE+1)
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT		(NN_ERRBASE+2)
#endif
#ifndef ENOBUFS
#define ENOBUFS			(NN_ERRBASE+3)
#endif
#ifndef ENETDOWN
#define ENETDOWN		(NN_ERRBASE+4)
#endif
#ifndef EADDRINUSE
#define EADDRINUSE		(NN_ERRBASE+5)
#endif
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL		(NN_ERRBASE+6)
#endif
#ifndef ENOTSOCK
#define ENOTSOCK		(NN_ERRBASE+7)
#endif
#ifndef EAGAIN
#define EAGAIN			(NN_ERRBASE+8)
#endif
#ifndef EBADF
#define EBADF			(NN_ERRBASE+9)
#endif
#ifndef EINVAL
#define EINVAL			(NN_ERRBASE+10)
#endif
#ifndef EMFILE
#define EMFILE			(NN_ERRBASE+11)
#endif
#ifndef EFAULT
#define EFAULT			(NN_ERRBASE+12)
#endif
#ifndef EACCES
#define EACCES			(NN_ERRBASE+13)
#endif
#ifndef ENETRESET
#define ENETRESET		(NN_ERRBASE+14)
#endif
#ifndef ENETUNREACH
#define ENETUNREACH		(NN_ERRBASE+15)
#endif
#ifndef EHOSTUNREACH
#define EHOSTUNREACH		(NN_ERRBASE+16)
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT		(NN_ERRBASE+17)
#endif
#ifndef EINPROGRESS
#define EINPROGRESS		(NN_ERRBASE+18)
#endif
#ifndef EPROTO
#define EPROTO			(NN_ERRBASE+19)
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED		(NN_ERRBASE+20)
#endif
#ifndef ENOTCONN
#define ENOTCONN		(NN_ERRBASE+21)
#endif
#ifndef EMSGSIZE
#define EMSGSIZE		(NN_ERRBASE+22)
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT		(NN_ERRBASE+23)
#endif
#ifndef ECONNABORTED
#define ECONNABORTED		(NN_ERRBASE+24)
#endif
#ifndef ECONNRESET
#define ECONNRESET		(NN_ERRBASE+25)
#endif
#ifndef ENOPROTOOPT
#define ENOPROTOOPT		(NN_ERRBASE+26)
#endif
#ifndef EISCONN
#define EISCONN			(NN_ERRBASE+27)
#endif
#ifndef ESOCKNOSUPPORT
#define ESOCKNOSPPORT		(NN_ERRBASE+28)
#endif
#ifndef ETERM
#define ETERM			(NN_ERRBASE+29)
#endif
#ifndef EFSM
#define EFSM			(NN_ERRBASE+30)
#endif
#ifndef ENOENT
#define ENOENT			(NN_ERRBASE+31)
#endif
#ifndef EIO
#define EIO			(NN_ERRBASE+32)
#endif
#ifndef EEXIST
#define EEXIST			(NN_ERRBASE+33)
#endif
#ifndef ENOSPC
#define ENOSPC			(NN_ERRBASE+34)
#endif


#define NN_LINGER		1
#define NN_SNDBUF		2
#define NN_RCVBUF		3
#define NN_SNDTIMEO		4
#define NN_RCVTIMEO		5
#define NN_RECONNECT_IVL	6
#define NN_RECONNECT_IVL_MAX	7
#define NN_SNDPRIO		8
#define NN_RCVPRIO		9
#define NN_SNDFD		10
#define NN_RCVFD		11
#define NN_DOMAIN		12
#define NN_PROTOCOL		13
#define NN_IPV4ONLY		14
#define NN_SOCKET_NAME		15
#define NN_RCVMAXSIZE		16
#define NN_MAXTTL		17


#define NN_POLLIN 1
#define NN_POLLOUT 2
struct nn_pollfd {
	int      fd;
	uint16_t events;
	uint16_t revents;
};

#define NN_MSG ((size_t) -1)

struct nn_iovec {
	void * iov_base;
	size_t iov_len;
};

struct nn_msghdr {
	struct nn_iovec *msg_iov;
	int              msg_iovlen;
	void *           msg_control;
	size_t           msg_controllen;
};

struct nn_cmsghdr {
	size_t cmsg_len;
	int    cmsg_level;
	int    cmsg_type;
};

#define NN_CMSG_ALIGN(len) \
	(((len) + sizeof(size_t) - 1) & (size_t) ~(sizeof(size_t) - 1))

#define NN_CMSG_FIRSTHDR(mh) nn_cmsg_next((struct nn_msghdr *) (mh), NULL)
#define NN_CMSG_NXTHDR(mh, ch) \
	nn_cmsg_next((struct nn_msghdr *) (mh), (struct nn_cmsghdr *) ch)
#define NN_CMSG_DATA(ch) ((unsigned char *) (((struct nn_cmsghdr *) (ch)) + 1))
#define NN_CMSG_SPACE(len) \
	(NN_CMSG_ALIGN(len) + NN_CMSG_ALIGN(sizeof(struct nn_cmsghdr)))
#define NN_CMSG_LEN(len) (NN_CMSG_ALIGN(sizeof(struct nn_cmsghdr)) + (len))

/*NN_DECL struct nn_cmsghdr *nn_cmsg_next(
    struct nn_msghdr *, struct nn_cmsghdr *);*/
/*NN_DECL int nn_socket(int, int);*/
/*NN_DECL int nn_setsockopt(int, int, int, const void *, size_t);*/
/*NN_DECL int nn_getsockopt(int, int, int, void *, size_t *);*/
/*NN_DECL int nn_bind(int, const char *);*/
/*NN_DECL int nn_connect(int, const char *);*/
/*NN_DECL int nn_shutdown(int, int);*/
/*NN_DECL int nn_send(int, const void *, size_t, int);*/
/*NN_DECL int nn_recv(int, void *, size_t, int);*/
/*NN_DECL int nn_sendmsg(int, const struct nn_msghdr *, int);*/
/*NN_DECL int nn_recvmsg(int, struct nn_msghdr *, int);*/
/*NN_DECL int nn_close(int);*/
/*NN_DECL int nn_poll(struct nn_pollfd *, int, int);*/
/*NN_DECL int nn_device(int, int);*/
/*NN_DECL uint64_t    nn_get_statistic(int, int);*/
/*NN_DECL void *      nn_allocmsg(size_t, int);*/
/*NN_DECL void *      nn_reallocmsg(void *, size_t);*/
/*NN_DECL int         nn_freemsg(void *);*/
/*NN_DECL int         nn_errno(void);*/
/*NN_DECL const char *nn_strerror(int);*/
/*NN_DECL void        nn_term(void);*/

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_NN_H
