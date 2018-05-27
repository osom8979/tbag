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

#include <exception>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

#ifndef TBAG_ERROR_INFO_MAP
#define TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_UV_XX) \
    /* Common error codes (Success Anyway). */ \
    _TBAG_XX(_SUCCESS  , "No error"                ) \
    _TBAG_XX(_WARNING  , "Success, but warning"    ) \
    _TBAG_XX(_CONTINUE , "Wait for the next"       ) \
    _TBAG_XX(_ASYNCREQ , "Async request"           ) \
    _TBAG_XX(_ENQASYNC , "Enqueue and async"       ) \
    /* Common error codes. */ \
    _TBAG_XX(_ILLARGS  , "Illegal arguments"       ) \
    _TBAG_XX(_ILLSTATE , "Illegal state"           ) \
    _TBAG_XX(_EOPEN    , "Open error"              ) \
    _TBAG_XX(_ECLOSE   , "Close error"             ) \
    _TBAG_XX(_EGET     , "Getter error"             ) \
    _TBAG_XX(_ESET     , "Setter error"             ) \
    _TBAG_XX(_ECREATE  , "Create error"            ) \
    _TBAG_XX(_EINDEX   , "Index error"             ) \
    _TBAG_XX(_ALREADY  , "Already state"           ) \
    _TBAG_XX(_SAMEOBJ  , "Same object"             ) \
    _TBAG_XX(_BADALLOC , "Bad allocation"          ) \
    _TBAG_XX(_OORANGE  , "Out of range"            ) \
    _TBAG_XX(_NULLPTR  , "Null pointer exception"  ) \
    _TBAG_XX(_EBUFFER  , "Empty buffer"            ) \
    _TBAG_XX(_EQUEUE   , "Empty queue"             ) \
    _TBAG_XX(_NREADY   , "Not ready"               ) \
    _TBAG_XX(_UNSUPOP  , "Unsupported operation"   ) \
    _TBAG_XX(_EINIT    , "Initialize error"        ) \
    _TBAG_XX(_RDERR    , "Unknown Read error"      ) \
    _TBAG_XX(_WRERR    , "Unknown Write error"     ) \
    _TBAG_XX(_SHUTDOWN , "Shutdown error"          ) \
    _TBAG_XX(_ENFOUND  , "Not found error"         ) \
    _TBAG_XX(_TIMEOUT  , "Timeout error"           ) \
    _TBAG_XX(_PARSING  , "Parsing error"           ) \
    _TBAG_XX(_VERIFIER , "Verifier error"          ) \
    _TBAG_XX(_EXPIRED  , "Expired object error"    ) \
    _TBAG_XX(_INSERT   , "Insert error"            ) \
    _TBAG_XX(_CLOSING  , "Closing object"          ) \
    _TBAG_XX(_CLOSED   , "Closed object"           ) \
    _TBAG_XX(_EQUERY   , "Query error"             ) \
    _TBAG_XX(_SMALLBUF , "Buffer size is small"    ) \
    _TBAG_XX(_KEYGEN   , "Key generator error"     ) \
    _TBAG_XX(_UNKEXCP  , "Unknown exception"       ) \
    _TBAG_XX(_UNKSIG   , "Unknown signal"          ) \
    _TBAG_XX(_ECOPY    , "Copy error"              ) \
    _TBAG_XX(_EPUSH    , "Push error"              ) \
    _TBAG_XX(_EPOP     , "Pop error"               ) \
    _TBAG_XX(_ESEND    , "Send error"              ) \
    _TBAG_XX(_ERECV    , "Recv error"              ) \
    _TBAG_XX(_INACCES  , "Inaccessible error"      ) \
    _TBAG_XX(_HTTP_1XX , "Informational (HTTP 1xx)") \
    _TBAG_XX(_HTTP_3XX , "Redirection (HTTP 3xx)"  ) \
    _TBAG_XX(_HTTP_4XX , "Client Error (HTTP 4xx)" ) \
    _TBAG_XX(_HTTP_5XX , "Server Error (HTTP 5xx)" ) \
    _TBAG_XX(_EXIT     , "Exit process"            ) \
    /* SSL */ \
    _TBAG_XX(_SSL      , "SSL error"               ) \
    _TBAG_XX(_SSLWREAD , "SSL want read"           ) \
    _TBAG_XX(_SSLWWRITE, "SSL want write"          ) \
    _TBAG_XX(_SSLWX509 , "SSL want x509 loopup"    ) \
    /* OpenCL */ \
    _TBAG_XX(_OPENCL   , "OpenCL error"            ) \
    /* CUDA */ \
    _TBAG_XX(_CUDA     , "CUDA error"              ) \
    /* LMDB */ \
    _TBAG_XX(_LMDB     , "LMDB error"              ) \
    /* Locale */ \
    _TBAG_XX(_LOCALE_CONVERTER_ERROR, "UConverter error")  \
    /* Network */ \
    _TBAG_XX(_CONNECTION_RESET, "A connection was forcibly closed by a peer") \
    _TBAG_XX(_NO_RESPONSE     , "No response"                               ) \
    _TBAG_XX(_NOT_WS_RESPONSE , "Not WebSocket response"                    ) \
    /* Process */ \
    _TBAG_XX(_UNKNOWN_PROCESS_ID, "Unknown process id") \
    /* Missing system error in libuv. */ \
    _TBAG_XX(_EBADMSG        ,  "Bad message"                                          ) \
    _TBAG_XX(_ECHILD         ,  "No child processes"                                   ) \
    _TBAG_XX(_EDEADLK        ,  "Resource deadlock would occur"                        ) \
    _TBAG_XX(_EDOM           ,  "Mathematics argument out of domain of function"       ) \
    _TBAG_XX(_EIDRM          ,  "Identifier removed"                                   ) \
    _TBAG_XX(_EILSEQ         ,  "Illegal byte sequence"                                ) \
    _TBAG_XX(_EINPROGRESS    ,  "Operation in progress"                                ) \
    _TBAG_XX(_ENETRESET      ,  "Connection aborted by network"                        ) \
    _TBAG_XX(_ENODATA        ,  "No message is available on the STREAM head read queue") \
    _TBAG_XX(_ENOEXEC        ,  "Executable file format error"                         ) \
    _TBAG_XX(_ENOLCK         ,  "No locks available"                                   ) \
    _TBAG_XX(_ENOLINK        ,  "Link has been severed"                                ) \
    _TBAG_XX(_ENOMSG         ,  "No message of the desired type"                       ) \
    _TBAG_XX(_ENOSR          ,  "No STREAM resources"                                  ) \
    _TBAG_XX(_ENOSTR         ,  "Not a STREAM"                                         ) \
    _TBAG_XX(_ENOTRECOVERABLE,  "State not recoverable"                                ) \
    _TBAG_XX(_ENOTTY         ,  "Inappropriate I/O control operation"                  ) \
    _TBAG_XX(_EOPNOTSUPP     ,  "Operation not supported on socket"                    ) \
    _TBAG_XX(_EOVERFLOW      ,  "Value too large to be stored in data type"            ) \
    _TBAG_XX(_EOWNERDEAD     ,  "Previous owner died"                                  ) \
    _TBAG_XX(_ETIME          ,  "Stream ioctl() timeout"                               ) \
    _TBAG_XX(_EWOULDBLOCK    ,  "Operation would block"                                ) \
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
};

inline int toInt(Err code) TBAG_NOEXCEPT { return static_cast<int>(code); }
inline Err toErr(int code) TBAG_NOEXCEPT { return static_cast<Err>(code); }

TBAG_API char const * getErrName(Err code) TBAG_NOEXCEPT;
TBAG_API char const * getErrDetail(Err code) TBAG_NOEXCEPT;

TBAG_API Err convertUvErrorToErr(int uv_error_code) TBAG_NOEXCEPT;
TBAG_API Err convertUvErrorToErrWithLogging(char const * prefix, int uv_error_code);

TBAG_API Err convertSystemErrorToErr(int system_error);
TBAG_API Err getGlobalSystemError();

/**
 * Err exception.
 *
 * @author zer0
 * @date   2018-04-07
 */
struct ErrException : public std::exception
{
    Err const CODE;

    ErrException(Err c) : CODE(c) { /* EMPTY. */ }
    virtual ~ErrException() { /* EMPTY. */ }

    virtual const char * what() const TBAG_NOEXCEPT
    { return getErrName(CODE); }
};

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

// --------------
// Output Stream.
// --------------

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, Err const & err)
{
    return os << libtbag::getErrName(err);
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

inline bool isSuccess(libtbag::Err err) TBAG_NOEXCEPT
{
    return err == libtbag::Err::E_SUCCESS;
}

inline bool isFailure(libtbag::Err err) TBAG_NOEXCEPT
{
    return !isSuccess(err);
}

inline bool isSuccessAnyway(libtbag::Err err) TBAG_NOEXCEPT
{
    using namespace libtbag;
    // @formatter:off
    return err == Err::E_SUCCESS  ||
           err == Err::E_WARNING  ||
           err == Err::E_CONTINUE ||
           err == Err::E_ASYNCREQ ||
           err == Err::E_ENQASYNC;
    // @formatter:on
}

inline bool isFailureAnyway(libtbag::Err err) TBAG_NOEXCEPT
{
    return !isSuccessAnyway(err);
}

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__

