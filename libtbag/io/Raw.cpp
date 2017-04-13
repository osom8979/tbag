/**
 * @file   Raw.cpp
 * @brief  Raw class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/io/Raw.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdarg>
#include <cstdio>

#if defined(HAVE_SYSCALL_H)
# include <syscall.h>
#elif defined(HAVE_SYS_SYSCALL_H)
# include <sys/syscall.h>
#endif

#if defined(HAVE_SYSCALL_H) || defined(HAVE_SYS_SYSCALL_H)
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
    int n = vsnprintf(buf, size, format, args);
    // Not counting the terminating null character.
    va_end(args);

    if (0 <= COMPARE_AND(n) <= size) {
        return true;
    }
    return false;
}

} // namespace io

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

