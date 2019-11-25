/**
 * @file   Raw.cpp
 * @brief  Raw class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/io/Raw.hpp>

#include <cstdarg>
#include <cstdio>

#if defined(TBAG_HAVE_SYSCALL_H)
# include <syscall.h>
#elif defined(TBAG_HAVE_SYS_SYSCALL_H)
# include <sys/syscall.h>
#endif

#if defined(TBAG_HAVE_SYSCALL_H) || defined(TBAG_HAVE_SYS_SYSCALL_H)
# define safe_write(fd, s, len)  syscall(SYS_write, fd, s, len)
#else
// Not so safe, but what can you do?
# define safe_write(fd, s, len)  write(fd, s, len)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace io {

bool formatToMemory(char * buf, int size, char const * format, ...)
{
    va_list args;
    va_start(args, format);
    auto const n = vsnprintf(buf, size, format, args);
    // Not counting the terminating null character.
    va_end(args);
    return (0 <= COMPARE_AND(n) <= size);
}

int putStdout(char const * text)
{
    return ::puts(text);
}

int putStderr(char const * text)
{
    return ::fputs(text, stderr);
}

} // namespace io

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

