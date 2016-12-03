/**
 * @file   PosixFileSystem.cpp
 * @brief  PosixFileSystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 */

#include <libtbag/filesystem/details/PosixFileSystem.hpp>
#include <libtbag/log/Log.hpp>
#include <cstdlib>

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
namespace posix      {

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
#if defined(__UNIX_LIKE__)
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
    return std::string();
}

std::string getHomeDir()
{
    return std::string();
}

std::string getExePath()
{
    return std::string();
}

std::string getExeDir()
{
    return std::string();
}

bool createDirectory(std::string const & path)
{
    return false;
}

bool removeDirectory(std::string const & path)
{
    return false;
}

bool rename(std::string const & from, std::string const & to)
{
    return false;
}

bool remove(std::string const & path)
{
    return false;
}

bool exists(std::string const & path)
{
    return false;
}

bool isDirectory(std::string const & path)
{
    return false;
}

bool isRegularFile(std::string const & path)
{
    return false;
}

std::vector<std::string> scanDir(std::string const & path)
{
    return std::vector<std::string>();
}

} // namespace posix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

