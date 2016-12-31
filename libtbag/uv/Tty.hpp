/**
 * @file   Tty.hpp
 * @brief  Tty class prototype.
 * @author zer0
 * @date   2016-12-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_TTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_TTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Stream.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
class Loop;

/**
 * Tty class prototype.
 *
 * @author zer0
 * @date   2016-12-31
 */
class TBAG_API Tty : public Stream
{
public:
    using Parent = Stream;

public:
    enum class TtyMode
    {
        TTY_NORMAL, // Initial/normal terminal mode
        TTY_RAW,    // Raw input mode (On Windows, ENABLE_WINDOW_INPUT is also enabled)
        TTY_IO      // Binary-safe I/O mode for IPC (Unix-only).
    };

    enum class DefaultIo : uv::File
    {
        FILE_STDIN  = 0,
        FILE_STDOUT = 1,
        FILE_STDERR = 2,
    };

public:
    Tty(Loop & loop, uv::File fd, bool readable);
    Tty(Loop & loop, DefaultIo fd);
    virtual ~Tty();

public:
    /** Set the TTY using the specified terminal mode. */
    bool setMode(TtyMode mode);

    /** To be called when the program exits. Resets TTY settings to default values for the next process to take over. */
    bool resetMode();

    /** Gets the current Window size. On success it returns 0. */
    bool getWinSize(int * width, int * height);
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_TTY_HPP__

