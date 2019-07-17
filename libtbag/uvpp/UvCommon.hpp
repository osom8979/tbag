/**
 * @file   UvCommon.hpp
 * @brief  UvCommon class prototype.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-01-03 (Rename: Type -> UvCommon)
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/config-ex.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/BufferInfo.hpp>

#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
# include <cstdint>
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

#include <cstdlib>
#include <string>
#include <vector>
#include <limits>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <winsock2.h>
# include <WS2tcpip.h> // sockaddr_in6, addrinfo
# include <Windows.h>
#else
# include <netinet/in.h>
# include <netdb.h> // addrinfo
#endif

#if defined(USE_PTHREADS)
#include <pthread.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

TBAG_API void initialize();
TBAG_API void release();

// Platform-specific definitions for libuv support.
#if defined(TBAG_PLATFORM_WINDOWS)
using ufile  = int;
using usock  = SOCKET;
using ufd    = HANDLE;
using uuser  = unsigned char;
using ugroup = unsigned char;
#else
using ufile  = int;
using usock  = int;
using ufd    = int;
using uuser  = uint32_t;
using ugroup = uint32_t;
#endif

#if defined(TBAG_PLATFORM_WINDOWS)
using uthread = HANDLE;
#elif defined(USE_PTHREADS)
using uthread = pthread_t;
#else
using uthread = int;
#endif

using binf = util::binf;

#ifndef TBAG_UV_HANDLE_MAP
#define TBAG_UV_HANDLE_MAP(_TBAG_HANDLE_XX, _TBAG_REQ_XX, _TBAG_ETC_XX) \
    /* Handle types. */                      \
    _TBAG_HANDLE_XX(LOOP    , uv_loop_t)     \
    _TBAG_HANDLE_XX(HANDLE  , uv_handle_t)   \
    _TBAG_HANDLE_XX(STREAM  , uv_stream_t)   \
    _TBAG_HANDLE_XX(TCP     , uv_tcp_t)      \
    _TBAG_HANDLE_XX(UDP     , uv_udp_t)      \
    _TBAG_HANDLE_XX(PIPE    , uv_pipe_t)     \
    _TBAG_HANDLE_XX(TTY     , uv_tty_t)      \
    _TBAG_HANDLE_XX(POLL    , uv_poll_t)     \
    _TBAG_HANDLE_XX(TIMER   , uv_timer_t)    \
    _TBAG_HANDLE_XX(PREPARE , uv_prepare_t)  \
    _TBAG_HANDLE_XX(CHECK   , uv_check_t)    \
    _TBAG_HANDLE_XX(IDLE    , uv_idle_t)     \
    _TBAG_HANDLE_XX(ASYNC   , uv_async_t)    \
    _TBAG_HANDLE_XX(PROCESS , uv_process_t)  \
    _TBAG_HANDLE_XX(FS_EVENT, uv_fs_event_t) \
    _TBAG_HANDLE_XX(FS_POLL , uv_fs_poll_t)  \
    _TBAG_HANDLE_XX(SIGNAL  , uv_signal_t)   \
    /* Request types. */                        \
    _TBAG_REQ_XX(REQ        , uv_req_t)         \
    _TBAG_REQ_XX(GETADDRINFO, uv_getaddrinfo_t) \
    _TBAG_REQ_XX(GETNAMEINFO, uv_getnameinfo_t) \
    _TBAG_REQ_XX(SHUTDOWN   , uv_shutdown_t)    \
    _TBAG_REQ_XX(WRITE      , uv_write_t)       \
    _TBAG_REQ_XX(CONNECT    , uv_connect_t)     \
    _TBAG_REQ_XX(UDP_SEND   , uv_udp_send_t)    \
    _TBAG_REQ_XX(FS         , uv_fs_t)          \
    _TBAG_REQ_XX(WORK       , uv_work_t)        \
    /* None of the above. */                                \
    _TBAG_ETC_XX(CPU_INFO         , uv_cpu_info_t)          \
    _TBAG_ETC_XX(INTERFACE_ADDRESS, uv_interface_address_t) \
    _TBAG_ETC_XX(DIRENT           , uv_dirent_t)            \
    _TBAG_ETC_XX(PASSWD           , uv_passwd_t)            \
    _TBAG_ETC_XX(STDIO_CONTAINER  , uv_stdio_container_t)   \
    _TBAG_ETC_XX(PROCESS_OPTIONS  , uv_process_options_t)   \
    /* -- END -- */
#endif

#ifndef TBAG_UV_HANDLE_MAP_ALL
#define TBAG_UV_HANDLE_MAP_ALL(_TBAG_XX) \
    TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_XX, _TBAG_XX)
#endif

/**
 * Table of libuv types.
 *
 * @author zer0
 * @date   2016-12-07
 */
enum class UvType : int
{
    __START_NUMBER__ = -1,
#define _TBAG_XX(name, type) name,
    TBAG_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    __SIZE__
};

// clang-format off
#define _TBAG_XX(name, type) name = static_cast<int>(UvType::name),
#define _TBAG_NX(name, type)
enum class UvHandleType  : int { TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) };
enum class UvRequestType : int { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) };
enum class UvEtcType     : int { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) };
#undef _TBAG_XX
#undef _TBAG_NX
// clang-format on

/** Short name of uv types. */
typedef UvType        utype;
typedef UvHandleType  uhandle;
typedef UvRequestType ureq;
typedef UvEtcType     uetc;

#if defined(_UINTPTR_T)
using AddressType = std::uintptr_t;
#else
using AddressType = std::size_t;
#endif
AddressType const DEBUG_UV_DELETED_ADDRESS_VALUE = static_cast<AddressType>(-1);

TBAG_CONSTEXPR static unsigned int getBufferInfoSizeMax() TBAG_NOEXCEPT
{
#if defined(max)
    TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif
    return std::numeric_limits<unsigned int>::max();
#if defined(__RESTORE_MAX__)
    TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif
}

template <typename T>
inline bool isDeletedAddress(T const * value) TBAG_NOEXCEPT
{ return DEBUG_UV_DELETED_ADDRESS_VALUE == reinterpret_cast<AddressType>(value); }

TBAG_API bool isHandle (utype type);
TBAG_API bool isRequest(utype type);
TBAG_API bool isEtc    (utype type);

/**
 * @remarks
 *  - uv_stream_t is a subclass of uv_handle_t.
 *  - uv_stream is an abstract class.
 *  - uv_stream_t is the parent class of uv_tcp_t, uv_pipe_t and uv_tty_t.
 */
TBAG_API bool isStream(utype type);

// ---------------------
// Native handle helper.
// ---------------------

TBAG_API char const * getUvNativeHandleName(void const * handle);
TBAG_API bool isUvNativeHandleType(void const * handle);

TBAG_API binf defaultOnAlloc(std::vector<char> & buffer, std::size_t suggested_size);

// --------
// Network.
// --------

/**
 * @var SOCKADDR_MAX_BYTE_SIZE
 *
 * Longest possible addresses.
 */
#if defined(SOCK_MAXADDRLEN)
TBAG_CONSTEXPR const int SOCKADDR_MAX_BYTE_SIZE = SOCK_MAXADDRLEN;
#else
TBAG_CONSTEXPR const int SOCKADDR_MAX_BYTE_SIZE = 255;
#endif

/**
 * @var INET_ADDR_MAX_BYTE_SIZE
 *
 * Longest possible ipv4 address string.
 *
 * @remarks
 *  e.g. 255.255.255.255
 */
#if defined(INET_ADDRSTRLEN)
TBAG_CONSTEXPR const int INET_ADDR_MAX_BYTE_SIZE = INET_ADDRSTRLEN;
#else
TBAG_CONSTEXPR const int INET_ADDR_MAX_BYTE_SIZE = 16;
#endif

/**
 * @var INET6_ADDR_MAX_BYTE_SIZE
 *
 * Longest possible ipv6 address string.
 *
 * @remarks
 *  e.g. 2001:0db8:85a3:08d3:1319:8a2e:0370:7334
 */
#if defined(INET6_ADDRSTRLEN)
TBAG_CONSTEXPR const int INET6_ADDR_MAX_BYTE_SIZE = INET6_ADDRSTRLEN;
#else
TBAG_CONSTEXPR const int INET6_ADDR_MAX_BYTE_SIZE = 46;
#endif

TBAG_CONSTEXPR const int UNKNOWN_PORT_NUMBER = (-1);

TBAG_API std::string getIpName(sockaddr     const * address);
TBAG_API std::string getIpName(sockaddr_in  const * address);
TBAG_API std::string getIpName(sockaddr_in6 const * address);

TBAG_API int getPortNumber(sockaddr const * address);
TBAG_API int getPortNumber(sockaddr_in const * address);
TBAG_API int getPortNumber(sockaddr_in6 const * address);

TBAG_API Err initAddress(std::string const & ip, int port, sockaddr_in * addr);
TBAG_API Err initAddress(std::string const & ip, int port, sockaddr_in6 * addr);

/** Cross-platform IPv6-capable implementation of inet_ntop(3). */
TBAG_API Err convertInetNtop(int family, void const * address, std::string & text);

/** Cross-platform IPv6-capable implementation of inet_pton(3). */
TBAG_API Err convertInetPton(int family, std::string const & text, void * address);

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVCOMMON_HPP__

