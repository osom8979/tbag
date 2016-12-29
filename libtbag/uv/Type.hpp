/**
 * @file   Type.hpp
 * @brief  Type class prototype.
 * @author zer0
 * @date   2016-12-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_TYPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_TYPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
# include <cstdint>
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

#include <string>

#if defined(__OS_WINDOWS__)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

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
    /* -- END -- */
#endif

#ifndef TBAG_UV_HANDLE_MAP_ALL
#define TBAG_UV_HANDLE_MAP_ALL(_TBAG_XX) \
    TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_XX, _TBAG_XX)
#endif

/** POD type of UvType. */
typedef int UvPodType;

/**
 * Table of libuv types.
 *
 * @author zer0
 * @date   2016-12-07
 */
enum class UvType : UvPodType
{
#define _TBAG_XX(name, type) name,
    TBAG_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
};

// @formatter:off
#define _TBAG_XX(name, type) name = static_cast<UvPodType>(UvType::name),
#define _TBAG_NX(name, type)
enum class UvHandleType  : UvPodType { TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) };
enum class UvRequestType : UvPodType { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) };
enum class UvEtcType     : UvPodType { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) };
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

TBAG_API bool isHandle (UvType type);
TBAG_API bool isRequest(UvType type);
TBAG_API bool isEtc    (UvType type);

/**
 * @remarks
 *  - uv_stream_t is a subclass of uv_handle_t.
 *  - uv_stream is an abstract class.
 *  - uv_stream_t is the parent class of uv_tcp_t, uv_pipe_t and uv_tty_t.
 */
TBAG_API bool isStream(UvType type);

/**
 * Buffer information structure.
 *
 * @author zer0
 * @date   2016-12-27
 */
struct BufferInfo
{
    char * buffer;
    std::size_t size;

    // @formatter:off
    BufferInfo(char * b, std::size_t s) : buffer(b), size(s) { /* EMPTY. */ }
    BufferInfo() : BufferInfo(nullptr, 0) { /* EMPTY. */ }
    // @formatter:on
};

/** Short name of BufferInfo type. */
typedef BufferInfo binf;

// ---------------------
// Native handle helper.
// ---------------------

TBAG_API char const * getUvNativeHandleName(void const * handle);
TBAG_API bool isUvNativeHandleType(void const * handle);

// ------------------
// Debugging methods.
// ------------------

/**
 * @remarks
 *  Same this code:
 *  @code
 *    const char* uv_strerror(int err);
 *  @endcode
 */
TBAG_API std::string getUvErrorString(int uv_error_code);

/**
 * @remarks
 *  Same this code:
 *  @code
 *    const char* uv_err_name(int err);
 *  @endcode
 */
TBAG_API std::string getUvErrorName(int uv_error_code);

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_TYPE_HPP__

