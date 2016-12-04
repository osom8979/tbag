/**
 * @file   UnixFileSystem.cpp
 * @brief  UnixFileSystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 */

#include <libtbag/filesystem/details/UnixFileSystem.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>

#if defined(__PLATFORM_UNIX_LIKE__)
#include <unistd.h> // getcwd
#endif

#ifndef __CHECK_TEMP_ENV_VAR
#define __CHECK_TEMP_ENV_VAR(name, return_label)  \
    do {                            \
        path = std::getenv(name);   \
        if (path != nullptr) {      \
            goto return_label;      \
        }                           \
    } while (0)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace unix       {

static char const * const  TMPDIR_ENV_NAME = "TMPDIR";
static char const * const     TMP_ENV_NAME = "TMP";
static char const * const    TEMP_ENV_NAME = "TEMP";
static char const * const TEMPDIR_ENV_NAME = "TEMPDIR";

static char const * const LAST_ANDROID_TEMP_VALUE = "/data/local/tmp";
static char const * const   LAST_POSIX_TEMP_VALUE = "/tmp";

// No temp environment variables defined.
#if defined(__PLATFORM_ANDROID__)
static char const * const LAST_TEMP_VALUE = LAST_ANDROID_TEMP_VALUE;
#else
static char const * const LAST_TEMP_VALUE = LAST_POSIX_TEMP_VALUE;
#endif

std::string getTempDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    const char * path = nullptr;

    // Check the TMPDIR, TMP, TEMP, and TEMPDIR environment variables in order.
    __CHECK_TEMP_ENV_VAR( TMPDIR_ENV_NAME, __return_path);
    __CHECK_TEMP_ENV_VAR(    TMP_ENV_NAME, __return_path);
    __CHECK_TEMP_ENV_VAR(   TEMP_ENV_NAME, __return_path);
    __CHECK_TEMP_ENV_VAR(TEMPDIR_ENV_NAME, __return_path);

    // No temp environment variables defined.
    path = LAST_TEMP_VALUE;

__return_path:
    return std::string(path);
#else
    return std::string();
#endif
}

std::string getWorkDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    char path[MAX_PATH_LENGTH] = {0,};
    if (getcwd(path, MAX_PATH_LENGTH) == nullptr) {
        return std::string();
    }
    return std::string(path);
#else
    return std::string();
#endif
}

std::string getHomeDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return std::string();
#else
    return std::string();
#endif
}

std::string getExePath()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return std::string();
#else
    return std::string();
#endif
}

std::string getExeDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return std::string();
#else
    return std::string();
#endif
}

bool createDirectory(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

bool removeDirectory(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

bool rename(std::string const & from, std::string const & to)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

bool remove(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

bool exists(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

bool isDirectory(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

bool isRegularFile(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return false;
#else
    return false;
#endif
}

std::vector<std::string> scanDir(std::string const & path)
{
#if defined(__PLATFORM_UNIX_LIKE__)
    return std::vector<std::string>();
#else
    return std::vector<std::string>();
#endif
}

} // namespace unix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

