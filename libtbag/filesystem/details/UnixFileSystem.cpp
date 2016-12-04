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
#include <unistd.h> // getcwd, getuid
#include <pwd.h>    // getpwuid
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
static char const * const    HOME_ENV_NAME = "HOME";

static char const * const LAST_ANDROID_TEMP_VALUE = "/data/local/tmp";
static char const * const   LAST_POSIX_TEMP_VALUE = "/tmp";

// No temp environment variables defined.
#if defined(__PLATFORM_ANDROID__)
static char const * const LAST_TEMP_VALUE = LAST_ANDROID_TEMP_VALUE;
#else
static char const * const LAST_TEMP_VALUE = LAST_POSIX_TEMP_VALUE;
#endif

// ------------------------
// Unix specialize methods.
// ------------------------

std::string getHomeDirWithGetPwUid()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    if (passwd * pw = getpwuid(getuid())) {
        return std::string(pw->pw_dir);
    }
#endif
    return std::string();
}

// -----------------
// Common interface.
// -----------------

std::string getTempDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    // Check the TMPDIR, TMP, TEMP, and TEMPDIR environment variables in order.
    // @formatter:off
    if (char const * p1 = std::getenv( TMPDIR_ENV_NAME)) { return std::string(p1); }
    if (char const * p2 = std::getenv(    TMP_ENV_NAME)) { return std::string(p2); }
    if (char const * p3 = std::getenv(   TEMP_ENV_NAME)) { return std::string(p3); }
    if (char const * p4 = std::getenv(TEMPDIR_ENV_NAME)) { return std::string(p4); }
    return std::string(LAST_TEMP_VALUE); // No temp environment variables defined.
    // @formatter:on
#else
    return std::string();
#endif
}

std::string getWorkDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    char path[MAX_PATH_LENGTH] = {0,};
    if (getcwd(path, MAX_PATH_LENGTH) != nullptr) {
        return std::string(path);
    }
#endif
    return std::string();
}

std::string getHomeDir()
{
#if defined(__PLATFORM_UNIX_LIKE__)
    // Check if the HOME environment variable is set first.
    if (char const * path = std::getenv(HOME_ENV_NAME)) {
        return std::string(path);
    }
    // HOME is not set, so call getpwuid().
    return getHomeDirWithGetPwUid();
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

