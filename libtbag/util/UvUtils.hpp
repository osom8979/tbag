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
#include <libtbag/debug/ErrorCode.hpp>

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
    TBAG_UTIL_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
};

// @formatter:off
#define _TBAG_XX(name, type) name = static_cast<UvPodType>(UvType::name),
#define _TBAG_NX(name, type)
enum class UvHandleType  : UvPodType { TBAG_UTIL_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) };
enum class UvRequsetType : UvPodType { TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) };
enum class UvEtcType     : UvPodType { TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) };
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

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

TBAG_API bool isUvHandle(UvType type);
TBAG_API bool isUvRequest(UvType type);

/**
 * libuv native type utility class.
 *
 * @author zer0
 * @date   2016-12-07
 */
class TBAG_API UvNative : public Noncopyable
{
public:
    using Type = UvType;

private:
    Type const TYPE;
    void * _native;

public:
    UvNative(Type type);
    ~UvNative();

public:
    inline operator bool() const TBAG_NOEXCEPT
    { return _native != nullptr; }

public:
    inline Type getType() const TBAG_NOEXCEPT
    { return TYPE; }
    inline bool isHandle() const TBAG_NOEXCEPT
    { return isUvHandle(TYPE); }
    inline bool isRequest() const TBAG_NOEXCEPT
    { return isUvRequest(TYPE); }

public:
    inline void * getNative() TBAG_NOEXCEPT
    { return _native; }
    inline void const * getNative() const TBAG_NOEXCEPT
    { return _native; }

public:
    template <typename T>
    inline T * castNative() const TBAG_NOEXCEPT
    { return static_cast<T*>(_native); }
};

/**
 * libuv handle type utility class.
 *
 * @author zer0
 * @date   2016-12-17
 */
class TBAG_API UvHandle : public UvNative
{
public:
    struct OnCloseCallback
    {
        virtual void onClose() = 0;
    };

private:
    OnCloseCallback * _on_close_cb;

public:
    UvHandle(UvHandleType type);
    ~UvHandle();

public:
    inline void setOnCloseCallback(OnCloseCallback * callback) TBAG_NOEXCEPT
    { _on_close_cb = callback; }

public:
    bool isClosing() const TBAG_NOEXCEPT;
    ErrorCode close();

public:
    void onClose(void * handle);
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__

