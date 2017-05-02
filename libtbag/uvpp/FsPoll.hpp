/**
 * @file   FsPoll.hpp
 * @brief  FsPoll class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FSPOLL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FSPOLL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * FsPoll class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  FS Poll handles allow the user to monitor a given path for changes. @n
 *  Unlike uv_fs_event_t, fs poll handles use stat to detect when a file has changed @n
 *  so they can work on file systems where fs event handles can’t.
 */
class TBAG_API FsPoll : public Handle
{
public:
    using Parent    = Handle;
    using FileState = filesystem::details::FileState;
    using TimeSpec  = filesystem::details::TimeSpec;

public:
    FsPoll();
    FsPoll(Loop & loop);
    virtual ~FsPoll();

public:
    /** Initialize the handle. */
    uerr init(Loop & loop);

    /** Check the file at path for changes every interval milliseconds. */
    uerr start(char const * path, unsigned int interval);

    /** Stop the handle, the callback will no longer be called. */
    uerr stop();

    /** Get the path being monitored by the handle. */
    std::string getPath();

// Event methods.
public:
    virtual void onFsPoll(int status, FileState const & prev, FileState const & curr);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FSPOLL_HPP__

