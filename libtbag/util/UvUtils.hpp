/**
 * @file   UvUtils.hpp
 * @brief  UvUtils class prototype.
 * @author zer0
 * @date   2016-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
# include <cstdint>
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

TBAG_API void initUv();
TBAG_API char const * getUvHandleName(void * handle);

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

#ifndef TBAG_UTIL_UV_HANDLE_MAP
#define TBAG_UTIL_UV_HANDLE_MAP(_TBAG_HANDLE_XX, _TBAG_REQ_XX, _TBAG_ETC_XX) \
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

#ifndef TBAG_UTIL_UV_HANDLE_MAP_ALL
#define TBAG_UTIL_UV_HANDLE_MAP_ALL(_TBAG_XX) \
    TBAG_UTIL_UV_HANDLE_MAP(_TBAG_XX, _TBAG_XX, _TBAG_XX)
#endif

/**
 * Table of libuv types.
 *
 * @author zer0
 * @date   2016-12-07
 */
enum class UvType : int
{
    _UV_TYPE_START_NUMBER_ = -1,
    UNKNOWN = 0,
#define _TBAG_XX(name, type) name,
    TBAG_UTIL_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    UV_TYPE_SIZE
};

/** Table of libuv handle types. */
enum class UvHandle : int
{
    _UV_HANDLE_START_NUMBER_ = -1,
#define _TBAG_XX(name, type) name,
#define _TBAG_NOT(name, type)
    TBAG_UTIL_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NOT, _TBAG_NOT)
#undef _TBAG_XX
#undef _TBAG_NOT
    UV_HANDLE_SIZE
};

/** Table of libuv request types. */
enum class UvRequset : int
{
    _UV_REQUEST_START_NUMBER_ = -1,
#define _TBAG_XX(name, type) name,
#define _TBAG_NOT(name, type)
    TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NOT, _TBAG_XX, _TBAG_NOT)
#undef _TBAG_XX
#undef _TBAG_NOT
    UV_REQUEST_SIZE
};

/** Table of libuv etc types. */
enum class UvEtc : int
{
    _UV_ETC_START_NUMBER_ = -1,
#define _TBAG_XX(name, type) name,
#define _TBAG_NOT(name, type)
    TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NOT, _TBAG_NOT, _TBAG_XX)
#undef _TBAG_XX
#undef _TBAG_NOT
    UV_ETC_SIZE
};

/**
 * libuv native type utility class.
 *
 * @author zer0
 * @date   2016-12-07
 */
class TBAG_API UvNative
{
private:
    UvType const TYPE;
    void * _native;

public:
    UvNative(UvType type);
    ~UvNative();

public:
    inline operator bool() const TBAG_NOEXCEPT
    { return _native != nullptr; }

public:
    inline UvType getType() const TBAG_NOEXCEPT
    { return TYPE; }

public:
    inline void * getNative() TBAG_NOEXCEPT
    { return _native; }
    inline void const * getNative() const TBAG_NOEXCEPT
    { return _native; }

public:
    template <typename T>
    inline T * castNative() TBAG_NOEXCEPT
    { return static_cast<T*>(_native); }
};

/**
 * libuv input stream utility class.
 *
 * @author zer0
 * @date   2016-12-16
 */
class TBAG_API UvInStream : public UvNative
{
public:
    UvInStream(UvType type);
    ~UvInStream();
};

/**
 * libuv output stream utility class.
 *
 * @author zer0
 * @date   2016-12-16
 */
class TBAG_API UvOutStream : public UvNative
{
public:
    UvOutStream(UvType type);
    ~UvOutStream();
};

}; // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__

