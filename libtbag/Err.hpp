/**
 * @file   Err.hpp
 * @brief  Err class prototype.
 * @author zer0
 * @date   2016-12-14
 * @date   2017-05-18 (Rename: ErrorCode -> Err)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

#ifndef TBAG_ERROR_INFO_MAP
#define TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_UV_XX) \
    /* Common error codes. */ \
    _TBAG_XX(_SUCCESS , "No error"              ) \
    _TBAG_XX(_ILLARGS , "Illegal arguments"     ) \
    _TBAG_XX(_ISTATE  , "Illegal state"         ) \
    _TBAG_XX(_ALREADY , "Already state"         ) \
    _TBAG_XX(_BADALLOC, "Bad allocation"        ) \
    _TBAG_XX(_OORANGE , "Out of range"          ) \
    _TBAG_XX(_NULLPTR , "Null pointer exception") \
    _TBAG_XX(_EQUEUE  , "Empty queue"           ) \
    _TBAG_XX(_EBUFFER , "Empty buffer"          ) \
    _TBAG_XX(_NREADY  , "Not ready"             ) \
    _TBAG_XX(_UNSUPOP , "Unsupported operation" ) \
    _TBAG_XX(_EINIT   , "Initialize error"      ) \
    _TBAG_XX(_RDERR   , "Unknown Read error"    ) \
    _TBAG_XX(_WRERR   , "Unknown Write error"   ) \
    _TBAG_XX(_SHUTDOWN, "Shutdown error"        ) \
    _TBAG_XX(_ENFOUND , "Not found error"       ) \
    _TBAG_XX(_TIMEOUT , "Timeout error"         ) \
    _TBAG_XX(_PARING  , "Parsing error"         ) \
    _TBAG_XX(_EXPIRED , "Expired error"         ) \
    _TBAG_XX(_INSERT  , "Insert error"          ) \
    /* Locale */ \
    _TBAG_XX(_LOCALE_CONVERTER_ERROR, "UConverter error")  \
    /* Network */ \
    _TBAG_XX(_CONNECTION_RESET, "A connection was forcibly closed by a peer") \
    /* Process */ \
    _TBAG_XX(_UNKNOWN_PROCESS_ID, "Unknown process id") \
    /* libuv/uvpp */ \
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
 * List of error code.
 *
 * @author zer0
 * @date 2016-12-14
 * @date 2017-05-18 (Rename: ErrorCode -> Err)
 */
enum class Err : int
{
    __START_NUMBER__ = -1,
#define _TBAG_XX(name, msg) E##name,
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    __SIZE__
};

TBAG_API char const * getErrName(Err code) TBAG_NOEXCEPT;
TBAG_API char const * getErrDetail(Err code) TBAG_NOEXCEPT;

/** Short name of ErrorCode type. */
TBAG_API Err convertUvErrorToErr(int uv_error_code) TBAG_NOEXCEPT;
TBAG_API Err convertUvErrorToErrWithLogging(char const * prefix, int uv_error_code);

// ----------------
// libuv debugging.
// ----------------

/**
 * @remarks
 *  Same this code:
 *  @code
 *    const char* uv_strerror(int err);
 *  @endcode
 */
TBAG_API char const * getUvErrorDetail(int uv_error_code);

/**
 * @remarks
 *  Same this code:
 *  @code
 *    const char* uv_err_name(int err);
 *  @endcode
 */
TBAG_API char const * getUvErrorName(int uv_error_code);

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__

