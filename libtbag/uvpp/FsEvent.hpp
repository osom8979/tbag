/**
 * @file   FsEvent.hpp
 * @brief  FsEvent class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FSEVENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FSEVENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * FsEvent class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  FS Event handles allow the user to monitor a given path for changes, @n
 *  for example, if the file was renamed or there was a generic change in it. @n
 *  This handle uses the best backend for the job on each platform.
 *
 * @warning
 *  For AIX, the non default IBM bos.ahafs package has to be installed. @n
 *  The AIX Event Infrastructure file system (ahafs) has some limitations:
 *  - ahafs tracks monitoring per process and is not thread safe.
 *    A separate process must be spawned for each monitor for the same event.
 *  - Events for file modification (writing to a file) are not received
 *    if only the containing folder is watched.
 */
class TBAG_API FsEvent : public Handle
{
public:
    friend class Loop;

public:
    using Parent = Handle;

public:
    /** Event types that uv_fs_event_t handles monitor. */
    using Event = int;

    TBAG_CONSTEXPR static Event const EVENT_RENAME = 1;
    TBAG_CONSTEXPR static Event const EVENT_CHANGE = 2;

public:
    /** Flags that can be passed to uv_fs_event_start() to control its behavior. */
    using EventFlag = unsigned int;

    /**
     * By default, if the fs event watcher is given a directory name, we will
     * watch for all events in that directory. This flags overrides this behavior
     * and makes fs_event report only changes to the directory entry itself. This
     * flag does not affect individual files watched.
     * This flag is currently not implemented yet on any backend.
     */
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_WATCH_ENTRY = 1;

    /**
     * By default uv_fs_event will try to use a kernel interface such as inotify
     * or kqueue to detect events. This may not work on remote file systems such
     * as NFS mounts. This flag makes fs_event fall back to calling stat() on a
     * regular interval.
     * This flag is currently not implemented yet on any backend.
     */
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_STAT = 2;

    /**
     * By default, event watcher, when watching directory, is not registering
     * (is ignoring) changes in its subdirectories.
     * This flag will override this behaviour on platforms that support it.
     */
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_RECURSIVE = 4;

    /** Default event flags. */
    TBAG_CONSTEXPR static EventFlag const EVENT_FLAG_DEFAULT = 0;

protected:
    FsEvent(Loop & loop);

public:
    virtual ~FsEvent();

public:
    /** Initialize the handle. */
    uerr init(Loop & loop);

    /**
     * Start the handle with the given callback, which will watch the specified path for changes.
     * flags can be an ORed mask of uv_fs_event_flags.
     *
     * @warning
     *  Currently the only supported flag is UV_FS_EVENT_RECURSIVE and only on OSX and Windows.
     */
    uerr start(char const * path, EventFlag flags = EVENT_FLAG_DEFAULT);

    /** Stop the handle, the callback will no longer be called. */
    uerr stop();

    /** Get the path being monitored by the handle. */
    std::string getPath();

public:
    static Event getEvent(int native_events) TBAG_NOEXCEPT;

// Event methods.
public:
    virtual void onFsEvent(const char * filename, Event events, uerr status);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FSEVENT_HPP__

