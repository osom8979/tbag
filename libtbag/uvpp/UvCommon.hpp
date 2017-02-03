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
#include <libtbag/predef.hpp>

#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
# include <cstdint>
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

#include <cstdlib>
#include <string>
#include <vector>

#if defined(__OS_WINDOWS__)
# include <winsock2.h>
# include <Windows.h>
#else
# include <netinet/in.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

#if defined(__OS_WINDOWS__)
using ufile = int;
using usock = SOCKET;
using ufd   = HANDLE;
#else
using ufile = int;
using usock = int;
using ufd   = int;
#endif

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

// @formatter:off
#define _TBAG_XX(name, type) name = static_cast<int>(UvType::name),
#define _TBAG_NX(name, type)
enum class UvHandleType  : int { TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) };
enum class UvRequestType : int { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) };
enum class UvEtcType     : int { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) };
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

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

#ifndef TBAG_UV_ERROR_MAP
#define TBAG_UV_ERROR_MAP(_TBAG_UVPP_XX, _TBAG_UV_XX) \
    /* uvpp error codes. */                           \
    _TBAG_UVPP_XX(_SUCCESS, "No error"         )      \
    _TBAG_UVPP_XX(_ILLARGS, "Illegal arguments")      \
    _TBAG_UVPP_XX(_NULLPTR, "Null pointer exception") \
    /* uv error codes. */                                                 \
    _TBAG_UV_XX(_E2BIG           , "Argument list too long"                ) \
    _TBAG_UV_XX(_EACCES          , "Permission denied"                     ) \
    _TBAG_UV_XX(_EADDRINUSE      , "Address already in use"                ) \
    _TBAG_UV_XX(_EADDRNOTAVAIL   , "Address not available"                 ) \
    _TBAG_UV_XX(_EAFNOSUPPORT    , "Address family not supported"          ) \
    _TBAG_UV_XX(_EAGAIN          , "Resource temporarily unavailable"      ) \
    _TBAG_UV_XX(_EAI_ADDRFAMILY  , "Address family not supported"          ) \
    _TBAG_UV_XX(_EAI_AGAIN       , "Temporary failure"                     ) \
    _TBAG_UV_XX(_EAI_BADFLAGS    , "Bad ai_flags value"                    ) \
    _TBAG_UV_XX(_EAI_BADHINTS    , "Invalid value for hints"               ) \
    _TBAG_UV_XX(_EAI_CANCELED    , "Request canceled"                      ) \
    _TBAG_UV_XX(_EAI_FAIL        , "Permanent failure"                     ) \
    _TBAG_UV_XX(_EAI_FAMILY      , "Ai_family not supported"               ) \
    _TBAG_UV_XX(_EAI_MEMORY      , "Out of memory"                         ) \
    _TBAG_UV_XX(_EAI_NODATA      , "No address"                            ) \
    _TBAG_UV_XX(_EAI_NONAME      , "Unknown node or service"               ) \
    _TBAG_UV_XX(_EAI_OVERFLOW    , "Argument buffer overflow"              ) \
    _TBAG_UV_XX(_EAI_PROTOCOL    , "Resolved protocol is unknown"          ) \
    _TBAG_UV_XX(_EAI_SERVICE     , "Service not available for socket type" ) \
    _TBAG_UV_XX(_EAI_SOCKTYPE    , "Socket type not supported"             ) \
    _TBAG_UV_XX(_EALREADY        , "Connection already in progress"        ) \
    _TBAG_UV_XX(_EBADF           , "Bad file descriptor"                   ) \
    _TBAG_UV_XX(_EBUSY           , "Resource busy or locked"               ) \
    _TBAG_UV_XX(_ECANCELED       , "Operation canceled"                    ) \
    _TBAG_UV_XX(_ECHARSET        , "Invalid Unicode character"             ) \
    _TBAG_UV_XX(_ECONNABORTED    , "Software caused connection abort"      ) \
    _TBAG_UV_XX(_ECONNREFUSED    , "Connection refused"                    ) \
    _TBAG_UV_XX(_ECONNRESET      , "Connection reset by peer"              ) \
    _TBAG_UV_XX(_EDESTADDRREQ    , "Destination address required"          ) \
    _TBAG_UV_XX(_EEXIST          , "File already exists"                   ) \
    _TBAG_UV_XX(_EFAULT          , "Bad address in system call argument"   ) \
    _TBAG_UV_XX(_EFBIG           , "File too large"                        ) \
    _TBAG_UV_XX(_EHOSTUNREACH    , "Host is unreachable"                   ) \
    _TBAG_UV_XX(_EINTR           , "Interrupted system call"               ) \
    _TBAG_UV_XX(_EINVAL          , "Invalid argument"                      ) \
    _TBAG_UV_XX(_EIO             , "I/O error"                             ) \
    _TBAG_UV_XX(_EISCONN         , "Socket is already connected"           ) \
    _TBAG_UV_XX(_EISDIR          , "Illegal operation on a directory"      ) \
    _TBAG_UV_XX(_ELOOP           , "Too many symbolic links encountered"   ) \
    _TBAG_UV_XX(_EMFILE          , "Too many open files"                   ) \
    _TBAG_UV_XX(_EMSGSIZE        , "Message too long"                      ) \
    _TBAG_UV_XX(_ENAMETOOLONG    , "Name too long"                         ) \
    _TBAG_UV_XX(_ENETDOWN        , "Network is down"                       ) \
    _TBAG_UV_XX(_ENETUNREACH     , "Network is unreachable"                ) \
    _TBAG_UV_XX(_ENFILE          , "File table overflow"                   ) \
    _TBAG_UV_XX(_ENOBUFS         , "No buffer space available"             ) \
    _TBAG_UV_XX(_ENODEV          , "No such device"                        ) \
    _TBAG_UV_XX(_ENOENT          , "No such file or directory"             ) \
    _TBAG_UV_XX(_ENOMEM          , "Not enough memory"                     ) \
    _TBAG_UV_XX(_ENONET          , "Machine is not on the network"         ) \
    _TBAG_UV_XX(_ENOPROTOOPT     , "Protocol not available"                ) \
    _TBAG_UV_XX(_ENOSPC          , "No space left on device"               ) \
    _TBAG_UV_XX(_ENOSYS          , "Function not implemented"              ) \
    _TBAG_UV_XX(_ENOTCONN        , "Socket is not connected"               ) \
    _TBAG_UV_XX(_ENOTDIR         , "Not a directory"                       ) \
    _TBAG_UV_XX(_ENOTEMPTY       , "Directory not empty"                   ) \
    _TBAG_UV_XX(_ENOTSOCK        , "Socket operation on non-socket"        ) \
    _TBAG_UV_XX(_ENOTSUP         , "Operation not supported on socket"     ) \
    _TBAG_UV_XX(_EPERM           , "Operation not permitted"               ) \
    _TBAG_UV_XX(_EPIPE           , "Broken pipe"                           ) \
    _TBAG_UV_XX(_EPROTO          , "Protocol error"                        ) \
    _TBAG_UV_XX(_EPROTONOSUPPORT , "Protocol not supported"                ) \
    _TBAG_UV_XX(_EPROTOTYPE      , "Protocol wrong type for socket"        ) \
    _TBAG_UV_XX(_ERANGE          , "Result too large"                      ) \
    _TBAG_UV_XX(_EROFS           , "Read-only file system"                 ) \
    _TBAG_UV_XX(_ESHUTDOWN       , "Cannot send after transport endpoint shutdown") \
    _TBAG_UV_XX(_ESPIPE          , "Invalid seek"                          ) \
    _TBAG_UV_XX(_ESRCH           , "No such process"                       ) \
    _TBAG_UV_XX(_ETIMEDOUT       , "Connection timed out"                  ) \
    _TBAG_UV_XX(_ETXTBSY         , "Text file is busy"                     ) \
    _TBAG_UV_XX(_EXDEV           , "Cross-device link not permitted"       ) \
    _TBAG_UV_XX(_UNKNOWN         , "Unknown error"                         ) \
    _TBAG_UV_XX(_EOF             , "End of file"                           ) \
    _TBAG_UV_XX(_ENXIO           , "No such device or address"             ) \
    _TBAG_UV_XX(_EMLINK          , "Too many links"                        ) \
    /* -- END -- */
#endif

/**
 * Table of libuv error codes.
 *
 * @author zer0
 * @date   2017-01-30
 */
enum class ErrorCode : int
{
    __START_NUMBER__ = -1,
#define _TBAG_XX(name, msg) UVPP##name,
    TBAG_UV_ERROR_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    __SIZE__
};

/** Short name of ErrorCode type. */
typedef ErrorCode uerr;

TBAG_API uerr getUerr(int uv_error_code);
TBAG_API uerr getUerr2(char const * prefix, int uv_error_code);

TBAG_API std::string getErrorName(uerr err);
TBAG_API std::string getErrorDetail(uerr err);

// ---------------------
// Native handle helper.
// ---------------------

TBAG_API char const * getUvNativeHandleName(void const * handle);
TBAG_API bool isUvNativeHandleType(void const * handle);

TBAG_API binf defaultOnAlloc(std::vector<char> & buffer, std::size_t suggested_size);

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVCOMMON_HPP__

