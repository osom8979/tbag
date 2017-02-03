/**
 * @file   Tty.cpp
 * @brief  Tty class implementation.
 * @author zer0
 * @date   2016-12-31
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Tty.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

Tty::Tty() : Stream(uhandle::TTY)
{
    // EMPTY.
}

Tty::Tty(Loop & loop, ufile fd, bool readable) : Tty()
{
    if (init(loop, fd, readable) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }

    if (guessHandle(fd) == false) {
        __tbag_error("Tty::Tty({}) Not UV_TTY handle warning.", static_cast<int>(fd));
    }
}

Tty::Tty(Loop & loop, GeneralFile fd) : Tty()
{
    if (init(loop, fd) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }

    if (guessHandle(fd) == false) {
        __tbag_error("Tty::Tty({}) Not UV_TTY handle warning.", static_cast<int>(fd));
    }
}

Tty::~Tty()
{
    // EMPTY.
}

uerr Tty::init(Loop & loop, ufile fd, bool readable)
{
    // readable, specifies if you plan on calling uv_read_start() with this stream.
    // stdin is readable, stdout is not.
    //
    // On Unix this function will determine the path of the fd of the terminal using ttyname_r(3),
    // open it, and use it if the passed file descriptor refers to a TTY.
    // This lets libuv put the tty in non-blocking mode without affecting other processes that share the tty.
    //
    // This function is not thread safe on systems that don’t support ioctl TIOCGPTN or TIOCPTYGNAME,
    // for instance OpenBSD and Solaris.
    //
    // Note: If reopening the TTY fails, libuv falls back to blocking writes for non-readable TTY streams.
    //
    // Changed in version 1.9.0::
    // the path of the TTY is determined by ttyname_r(3).
    // In earlier versions libuv opened /dev/tty instead.
    //
    // Changed in version 1.5.0::
    // trying to initialize a TTY stream with a file descriptor
    // that refers to a file returns UV_EINVAL on UNIX.

    int const CODE = ::uv_tty_init(loop.cast<uv_loop_t>(), Parent::cast<uv_tty_t>(), fd, readable ? 1 : 0);
    return getUerr2("Tty::init()", CODE);
}

uerr Tty::init(Loop & loop, GeneralFile fd)
{
    return init(loop, toFile(fd), fd == GeneralFile::FILE_STDIN);
}

uerr Tty::setMode(TtyMode mode)
{
    uv_tty_mode_t uv_mode = UV_TTY_MODE_NORMAL;
    // @formatter:off
    switch (mode) {
    case TtyMode::TTY_NORMAL: uv_mode = UV_TTY_MODE_NORMAL; break;
    case TtyMode::TTY_RAW:    uv_mode = UV_TTY_MODE_RAW;    break;
    case TtyMode::TTY_IO:     uv_mode = UV_TTY_MODE_IO;     break;
    }
    // @formatter:on

    // Changed in version 1.2.0::
    // the mode is specified as a uv_tty_mode_t value.
    //
    // Set the TTY using the specified terminal mode.
    int const CODE = ::uv_tty_set_mode(Parent::cast<uv_tty_t>(), uv_mode);
    return getUerr2("Tty::setMode()", CODE);
}

uerr Tty::resetMode()
{
    // To be called when the program exits.
    // Resets TTY settings to default values for the next process to take over.
    //
    // This function is async signal-safe on Unix platforms but can fail
    // with error code UV_EBUSY if you call it when execution is inside uv_tty_set_mode().
    int const CODE = ::uv_tty_reset_mode();
    return getUerr2("Tty::resetMode()", CODE);
}

uerr Tty::getWinSize(int * width, int * height)
{
    int const CODE = ::uv_tty_get_winsize(Parent::cast<uv_tty_t>(), width, height);
    return getUerr2("Tty::getWinSize()", CODE);
}

ufile Tty::toFile(GeneralFile fd) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (fd) {
    case GeneralFile::FILE_STDIN : return 0;
    case GeneralFile::FILE_STDOUT: return 1;
    case GeneralFile::FILE_STDERR: return 2;
    }
    // @formatter:on
}

bool Tty::guessHandle(ufile fd)
{
    // Used to detect what type of stream should be used with a given file descriptor.
    // Usually this will be used during initialization to guess the type of the stdio streams.
    //
    // For isatty(3) equivalent functionality use this function and test for UV_TTY.
    return ::uv_guess_handle(fd) == UV_TTY;
}

bool Tty::guessHandle(GeneralFile fd)
{
    return guessHandle(toFile(fd));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

