/**
 * @file   Err.cpp
 * @brief  Err class implementation.
 * @author zer0
 * @date   2016-12-14
 * @date   2017-05-18 (Rename: ErrorCode -> Err)
 */

#ifndef TBAG_SYSTEM_ERROR_INFO_MAP
#define TBAG_SYSTEM_ERROR_INFO_MAP(_TBAG_XX, _TBAG_DUP_XX) \
    _TBAG_XX(E2BIG          , "Argument list too long"                               ) \
    _TBAG_XX(EACCES         , "Permission denied"                                    ) \
    _TBAG_XX(EADDRINUSE     , "Address in use"                                       ) \
    _TBAG_XX(EADDRNOTAVAIL  , "Address not available"                                ) \
    _TBAG_XX(EAFNOSUPPORT   , "Address family not supported"                         ) \
    _TBAG_XX(EAGAIN         , "Resource unavailable, try again"                      ) \
    _TBAG_XX(EALREADY       , "Connection already in progress"                       ) \
    _TBAG_XX(EBADF          , "Bad file descriptor"                                  ) \
    _TBAG_XX(EBADMSG        , "Bad message"                                          ) \
    _TBAG_XX(EBUSY          , "Device or resource busy"                              ) \
    _TBAG_XX(ECANCELED      , "Operation canceled"                                   ) \
    _TBAG_XX(ECHILD         , "No child processes"                                   ) \
    _TBAG_XX(ECONNABORTED   , "Connection aborted"                                   ) \
    _TBAG_XX(ECONNREFUSED   , "Connection refused"                                   ) \
    _TBAG_XX(ECONNRESET     , "Connection reset"                                     ) \
    _TBAG_XX(EDEADLK        , "Resource deadlock would occur"                        ) \
    _TBAG_XX(EDESTADDRREQ   , "Destination address required"                         ) \
    _TBAG_XX(EDOM           , "Mathematics argument out of domain of function"       ) \
    _TBAG_XX(EEXIST         , "File exists"                                          ) \
    _TBAG_XX(EFAULT         , "Bad address"                                          ) \
    _TBAG_XX(EFBIG          , "File too large"                                       ) \
    _TBAG_XX(EHOSTUNREACH   , "Host is unreachable"                                  ) \
    _TBAG_XX(EIDRM          , "Identifier removed"                                   ) \
    _TBAG_XX(EILSEQ         , "Illegal byte sequence"                                ) \
    _TBAG_XX(EINPROGRESS    , "Operation in progress"                                ) \
    _TBAG_XX(EINTR          , "Interrupted function"                                 ) \
    _TBAG_XX(EINVAL         , "Invalid argument"                                     ) \
    _TBAG_XX(EIO            , "I/O error"                                            ) \
    _TBAG_XX(EISCONN        , "Socket is connected"                                  ) \
    _TBAG_XX(EISDIR         , "Is a directory"                                       ) \
    _TBAG_XX(ELOOP          , "Too many levels of symbolic links"                    ) \
    _TBAG_XX(EMFILE         , "File descriptor value too large"                      ) \
    _TBAG_XX(EMLINK         , "Too many links"                                       ) \
    _TBAG_XX(EMSGSIZE       , "Message too large"                                    ) \
    _TBAG_XX(ENAMETOOLONG   , "Filename too long"                                    ) \
    _TBAG_XX(ENETDOWN       , "Network is down"                                      ) \
    _TBAG_XX(ENETRESET      , "Connection aborted by network"                        ) \
    _TBAG_XX(ENETUNREACH    , "Network unreachable"                                  ) \
    _TBAG_XX(ENFILE         , "Too many files open in system"                        ) \
    _TBAG_XX(ENOBUFS        , "No buffer space available"                            ) \
    _TBAG_XX(ENODATA        , "No message is available on the STREAM head read queue") \
    _TBAG_XX(ENODEV         , "No such device"                                       ) \
    _TBAG_XX(ENOENT         , "No such file or directory"                            ) \
    _TBAG_XX(ENOEXEC        , "Executable file format error"                         ) \
    _TBAG_XX(ENOLCK         , "No locks available"                                   ) \
    _TBAG_XX(ENOLINK        , "Link has been severed"                                ) \
    _TBAG_XX(ENOMEM         , "Not enough space"                                     ) \
    _TBAG_XX(ENOMSG         , "No message of the desired type"                       ) \
    _TBAG_XX(ENOPROTOOPT    , "Protocol not available"                               ) \
    _TBAG_XX(ENOSPC         , "No space left on device"                              ) \
    _TBAG_XX(ENOSR          , "No STREAM resources"                                  ) \
    _TBAG_XX(ENOSTR         , "Not a STREAM"                                         ) \
    _TBAG_XX(ENOSYS         , "Function not supported"                               ) \
    _TBAG_XX(ENOTCONN       , "The socket is not connected"                          ) \
    _TBAG_XX(ENOTDIR        , "Not a directory"                                      ) \
    _TBAG_XX(ENOTEMPTY      , "Directory not empty"                                  ) \
    _TBAG_XX(ENOTRECOVERABLE, "State not recoverable"                                ) \
    _TBAG_XX(ENOTSOCK       , "Not a socket"                                         ) \
    _TBAG_XX(ENOTSUP        , "Not supported"                                        ) \
    _TBAG_XX(ENOTTY         , "Inappropriate I/O control operation"                  ) \
    _TBAG_XX(ENXIO          , "No such device or address"                            ) \
    _TBAG_XX(EOVERFLOW      , "Value too large to be stored in data type"            ) \
    _TBAG_XX(EOWNERDEAD     , "Previous owner died"                                  ) \
    _TBAG_XX(EPERM          , "Operation not permitted"                              ) \
    _TBAG_XX(EPIPE          , "Broken pipe"                                          ) \
    _TBAG_XX(EPROTO         , "Protocol error"                                       ) \
    _TBAG_XX(EPROTONOSUPPORT, "Protocol not supported"                               ) \
    _TBAG_XX(EPROTOTYPE     , "Protocol wrong type for socket"                       ) \
    _TBAG_XX(ERANGE         , "Result too large"                                     ) \
    _TBAG_XX(EROFS          , "Read-only file system"                                ) \
    _TBAG_XX(ESPIPE         , "Invalid seek"                                         ) \
    _TBAG_XX(ESRCH          , "No such process"                                      ) \
    _TBAG_XX(ETIME          , "Stream ioctl() timeout"                               ) \
    _TBAG_XX(ETIMEDOUT      , "Connection timed out"                                 ) \
    _TBAG_XX(ETXTBSY        , "Text file busy"                                       ) \
    _TBAG_XX(EXDEV          , "Cross-device link"                                    ) \
    _TBAG_DUP_XX(EOPNOTSUPP , "Operation not supported on socket."                   ) \
    _TBAG_DUP_XX(EWOULDBLOCK, "Operation would block."                               ) \
    /* -- END -- */
#endif

// [WARNING] Don't change include order.
#include <libtbag/Err.hpp>
#include <cerrno>

// ---------------
namespace __impl {
// ---------------

/**
 * Convert system error to libtbag::Err.
 *
 * @param[in] system_error
 *      system error code defined in errno.h (c++: cerrno)
 *
 * @return Converted libtbag::Err code.
 *
 * @remarks
 *  Each of the macros defined in <cerrno> expands to integer constant expressions with type int,
 *  each with a positive value, matching most of the POSIX error codes. The following constants
 *  are defined (the implementation may define more, as long as they begin with 'E' followed by
 *  digits or uppercase letters)
 *
 * @warning
 *  All values are required to be unique except that the values
 *  of EOPNOTSUPP and ENOTSUP may be identical and the values
 *  of EAGAIN and EWOULDBLOCK may be identical.
 *
 * @see <http://en.cppreference.com/w/cpp/error/errno_macros>
 * @see <http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html>
 */
static libtbag::Err convertSystemErrorToErr(int system_error)
{
    switch (system_error) {
#define _TABG_XX(name, msg) case name: return libtbag::Err::E_##name;
#define _TABG_DUP_XX(name, msg)
    TBAG_SYSTEM_ERROR_INFO_MAP(_TABG_XX, _TABG_DUP_XX)
#undef _TABG_DUP_XX
#undef _TABG_XX

#if defined(EOPNOTSUPP) && defined(ENOTSUP) && (EOPNOTSUPP != ENOTSUP)
    case EOPNOTSUPP: return libtbag::Err::E_EOPNOTSUPP; // Operation not supported on socket.
#endif

#if defined(EWOULDBLOCK) && defined(EAGAIN) && (EWOULDBLOCK != EAGAIN)
    case EWOULDBLOCK: return libtbag::Err::E_EWOULDBLOCK; // Operation would block.
#endif
    default: return libtbag::Err::E_UNKNOWN;
    }
}

// ------------------
} // namespace __impl
// ------------------

#include <libtbag/log/Log.hpp>
#include <cstring>
#include <uv.h>

// Symbol check.
#define _XX(name, _) static_assert(UV_##name && static_cast<int>(libtbag::Err::E_##name), "");
UV_ERRNO_MAP(_XX)
#undef _XX

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

Err getErr(char const * upper_text) TBAG_NOEXCEPT
{
    assert(upper_text != nullptr);
#define _TBAG_XX(name, num, message) if (::strcmp("E"#name, upper_text) == 0) { return Err::E##name; }
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    // &(#name[1]) : Remove underscore.
#define _TBAG_XX(name, num, message) if (::strcmp(&(#name[1]), upper_text) == 0) { return Err::E##name; }
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    return E_UNKNOWN;
}

char const * getErrName(Err code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(name, num, message) case Err::E##name: return "E"#name;
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default:
        return "E_UNKNOWN";
    }
}

char const * getErrDetail(Err code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(name, num, message) case Err::E##name: return message;
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default:
        return "E_UNKNOWN";
    }
}

bool existsErr(int code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(name, num, message) case static_cast<int>(Err::E##name): return true;
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default:
        return false;
    }
}

Err convertUvErrorToErr(int uv_error_code) TBAG_NOEXCEPT
{
    // clang-format off
    switch (uv_error_code) {
    case 0: return Err::E_SUCCESS;
#define _TBAG_ERASE_XX(name, num, msg)
#define _TBAG_XX(name, num, msg) case UV##name: return Err::E##name;
    TBAG_ERROR_INFO_MAP(_TBAG_ERASE_XX, _TBAG_XX)
#undef _TBAG_ERASE_XX
#undef _TBAG_XX
    default:
        return E_UNKNOWN;
    }
    // clang-format on
}

Err convertUvErrorToErrWithLogging(char const * prefix, int uv_error_code)
{
    if (uv_error_code != 0) {
        tDLogE("UV~{} Error({}): {}", prefix, uv_error_code, getUvErrorName(uv_error_code));
        return convertUvErrorToErr(uv_error_code);
    }
    return Err::E_SUCCESS;
}

Err convertSystemErrorToErr(int system_error)
{
    return __impl::convertSystemErrorToErr(system_error);
}

Err getGlobalSystemError()
{
    return convertSystemErrorToErr(errno);
}

// ----------------
// libuv debugging.
// ----------------

char const * getUvErrorDetail(int uv_error_code)
{
    return ::uv_strerror(uv_error_code);
}

char const * getUvErrorName(int uv_error_code)
{
    return ::uv_err_name(uv_error_code);
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

