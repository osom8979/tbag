/**
 * @file   Handle.hpp
 * @brief  Handle class prototype.
 * @author zer0
 * @date   2016-12-27
 *
 * @see <http://docs.libuv.org/en/v1.x/handle.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_HANDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_HANDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Native.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

/**
 * libuv handle type utility class.
 *
 * @author zer0
 * @date   2016-12-17
 * @date   2016-12-27 (Move package: libtbag/util -> libtbag/uv)
 *
 * @remarks
 *  Reference counting: @n
 *  <pre>
 *   The libuv event loop (if run in the default mode) will run until there are
 *   no active and referenced handles left. The user can force the loop to exit
 *   early by unreferencing handles which are active, for example by calling
 *   uv_unref() after calling uv_timer_start().
 *
 *   A handle can be referenced or unreferenced, the refcounting scheme doesn’t
 *   use a counter, so both operations are idempotent.
 *
 *   All handles are referenced when active by default, see uv_is_active() for
 *   a more detailed explanation on what being active involves.
 *  </pre>
 */
class TBAG_API Handle : public Native
{
public:
    using Parent = Native;

public:
    Handle(UvHandleType type);
    virtual ~Handle();

public:
    inline char const * getName() const TBAG_NOEXCEPT
    { return getUvNativeHandleName(getNative()); }

public:
    bool isInit() const TBAG_NOEXCEPT;

public:
    /**
     * @remarks
     *  This function should only be used between the
     *  initialization of the handle
     *  and the arrival of the close callback.
     */
    void close();

public:
    /** Returns true if the handle is active, false if it’s inactive. */
    bool isActive() const TBAG_NOEXCEPT;

    /** Returns true if the handle is closing or closed, false otherwise. */
    bool isClosing() const TBAG_NOEXCEPT;

    /** Reference the given handle. */
    void ref() TBAG_NOEXCEPT;

    /** Un-reference the given handle. */
    void unref() TBAG_NOEXCEPT;

    /** Returns true if the handle referenced, false otherwise. */
    bool hasRef() const TBAG_NOEXCEPT;

    /** Returns the size of the given handle type. */
    std::size_t getNativeSize() const TBAG_NOEXCEPT;

// Miscellaneous API functions:
//
// This function works for TCP, pipe and UDP handles on Unix
// and for TCP and UDP handles on Windows.
public:
    /** Get the size of the send buffer that the operating system uses for the socket. */
    int getSendBufferSize() const;
    /** Get the size of the receive buffer that the operating system uses for the socket. */
    int getRecvBufferSize() const;

    /** Set the size of the send buffer that the operating system uses for the socket. */
    void setSendBufferSize(int size);
    /** Set the size of the receive buffer that the operating system uses for the socket. */
    void setRecvBufferSize(int size);

// Event methods.
public:
    /** Close event. */
    virtual void onClose();

    /** Active from the Loop class. */
    virtual void onWalk(void * arg);
};

#ifndef _TBAG_UV_HANDLE_EX
#define _TBAG_UV_HANDLE_EX(type, name)                  \
    struct name##Handle : public Handle {               \
        name##Handle() : Handle(UvHandleType::type) { } \
        ~name##Handle() { }                             \
    }
#endif

// [WARNING] This is Stream types:
//_TBAG_UV_HANDLE_EX(TCP   , Tcp    );
//_TBAG_UV_HANDLE_EX(PIPE  , Pipe   );
//_TBAG_UV_HANDLE_EX(TTY   , Tty    );

_TBAG_UV_HANDLE_EX(UDP     , Udp    );
_TBAG_UV_HANDLE_EX(POLL    , Poll   );
_TBAG_UV_HANDLE_EX(TIMER   , Timer  );
_TBAG_UV_HANDLE_EX(PREPARE , Prepare);
_TBAG_UV_HANDLE_EX(CHECK   , Check  );
_TBAG_UV_HANDLE_EX(IDLE    , Idle   );
_TBAG_UV_HANDLE_EX(ASYNC   , Async  );
_TBAG_UV_HANDLE_EX(PROCESS , Process);
_TBAG_UV_HANDLE_EX(FS_EVENT, FsEvent);
_TBAG_UV_HANDLE_EX(FS_POLL , FsPoll );
_TBAG_UV_HANDLE_EX(SIGNAL  , Signal );

#undef _TBAG_UV_HANDLE_EX

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_HANDLE_HPP__

