/**
 * @file   Tty.hpp
 * @brief  Tty class prototype.
 * @author zer0
 * @date   2016-12-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Stream.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

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

    enum class GeneralFile : uvpp::File
    {
        FILE_STDIN  = 0,
        FILE_STDOUT = 1,
        FILE_STDERR = 2,
    };

public:
    Tty();
    Tty(Loop & loop, uvpp::File fd, bool readable);
    Tty(Loop & loop, GeneralFile fd);
    virtual ~Tty();

public:
    /** Initialize a new TTY stream with the given file descriptor. */
    bool init(Loop & loop, uvpp::File fd, bool readable);
    bool init(Loop & loop, GeneralFile fd);

    /** Set the TTY using the specified terminal mode. */
    bool setMode(TtyMode mode);

    /** To be called when the program exits. Resets TTY settings to default values for the next process to take over. */
    bool resetMode();

    /** Gets the current Window size. On success it returns 0. */
    bool getWinSize(int * width, int * height);

public:
    static uvpp::File toFile(GeneralFile fd) TBAG_NOEXCEPT;

    /** Used to detect what type of stream should be used with a given file descriptor. */
    static bool guessHandle(uvpp::File fd);
    static bool guessHandle(GeneralFile fd);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TTY_HPP__

