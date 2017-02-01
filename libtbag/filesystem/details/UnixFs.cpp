/**
 * @file   UnixFs.cpp
 * @brief  Unix Filesystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 * @date   2016-12-13 (Rename: UnixFileSystem -> UnixFs)
 *
 * @remarks
 *  Use the libuv.
 */

#include <libtbag/filesystem/details/UnixFs.hpp>
#include <libtbag/filesystem/details/FsTemplate.hpp-inl>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>

#include <cstdio>

#include <fcntl.h>
#include <sys/stat.h>

#include <unicode/unistr.h>
#include <uv.h>

#ifndef _WIN_OR_UNIX
# if defined(__PLATFORM_WINDOWS__)
#  define _WIN_OR_UNIX(v) _##v
# else
#  define _WIN_OR_UNIX(v) v
# endif
#endif // _WIN_OR_UNIX

STATIC_ASSERT_CHECK_IS_SAME(int, uv_file);

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

// ---------------------
// libuv helper methods.
// ---------------------

namespace uvpp {

/**
 * @defgroup __DOXYGEN_GROUP__FILE_TYPE__ List of file type.
 * @remarks
 *  POSIX: include <sys/stat.h> or <fcntl.h>
 * @{
 */

static uint32_t const FILE_TYPE_S_IFMT   = _WIN_OR_UNIX(S_IFMT);   ///< type of file.
static uint32_t const FILE_TYPE_S_IFCHR  = _WIN_OR_UNIX(S_IFCHR);  ///< character special.
static uint32_t const FILE_TYPE_S_IFDIR  = _WIN_OR_UNIX(S_IFDIR);  ///< directory.
static uint32_t const FILE_TYPE_S_IFREG  = _WIN_OR_UNIX(S_IFREG);  ///< regular file.

#if 0
static uint32_t const FILE_TYPE_S_IFIFO  = S_IFIFO;  ///< named pipe (fifo).
static uint32_t const FILE_TYPE_S_IFBLK  = S_IFBLK;  ///< block special.
static uint32_t const FILE_TYPE_S_IFLNK  = S_IFLNK;  ///< symbolic link.
static uint32_t const FILE_TYPE_S_IFSOCK = S_IFSOCK; ///< socket.
#endif

/**
 * @}
 */

/**
 * @defgroup __DOXYGEN_GROUP__FILE_OPEN_FLAGS__ List of file open flags.
 * @remarks
 *  include <fcntl.h>
 * @{
 */

static uint32_t const FILE_OPEN_FLAG_READ_ONLY  = _WIN_OR_UNIX(O_RDONLY); ///< open for reading only.
static uint32_t const FILE_OPEN_FLAG_WRITE_ONLY = _WIN_OR_UNIX(O_WRONLY); ///< open for writing only.
static uint32_t const FILE_OPEN_FLAG_READ_WRITE = _WIN_OR_UNIX(O_RDWR);   ///< open for reading and writing.
static uint32_t const FILE_OPEN_APPEND          = _WIN_OR_UNIX(O_APPEND); ///< set append mode.

static uint32_t const FILE_OPEN_CREATE          = _WIN_OR_UNIX(O_CREAT);  ///< create if nonexistant.
static uint32_t const FILE_OPEN_TRUNCATE        = _WIN_OR_UNIX(O_TRUNC);  ///< truncate to zero length.
static uint32_t const FILE_OPEN_EXISTS_ERROR    = _WIN_OR_UNIX(O_EXCL);   ///< error if already exists.

#if 0
// !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
static uint32_t const FILE_OPEN_FLAG_ACCESS_ALL = O_ACCMODE;   ///< mask for above modes.
static uint32_t const FILE_OPEN_NON_BLOCK       = O_NONBLOCK;  ///< no delay.
static uint32_t const FILE_OPEN_SHARED_LOCK     = O_SHLOCK;    ///< open with shared file lock.
static uint32_t const FILE_OPEN_EXCLUSIVE_LOCK  = O_EXLOCK;    ///< open with exclusive file lock.
static uint32_t const FILE_OPEN_ASYNC           = O_ASYNC;     ///< signal pgrp when data ready.
static uint32_t const FILE_OPEN_SYNC            = O_FSYNC;     ///< synch I/O file integrity. (source compatibility: do not use)
static uint32_t const FILE_OPEN_NOFOLLOW        = O_NOFOLLOW;  ///< don't follow symlinks.
#endif

/**
 * @}
 */

/**
 * @defgroup __DOXYGEN_GROUP__FILE_MODE__ List of file mode flags.
 * @remarks
 *  include <fcntl.h>
 * @{
 */

// Read, write, execute/search by owner.
#if defined(__PLATFORM_WINDOWS__)
static uint32_t const FILE_MODE_OWNER_READ  = _S_IREAD;  ///< [XSI] R for owner.
static uint32_t const FILE_MODE_OWNER_WRITE = _S_IWRITE; ///< [XSI] W for owner.
#else
static uint32_t const FILE_MODE_OWNER_READ  = S_IRUSR;  ///< [XSI] R for owner.
static uint32_t const FILE_MODE_OWNER_WRITE = S_IWUSR;  ///< [XSI] W for owner.
#endif

#if 0
static uint32_t const FILE_MODE_OWNER_ALL      = S_IRWXU;  ///< [XSI] RWX mask for owner.
static uint32_t const FILE_MODE_OWNER_EXECUTE  = S_IXUSR;  ///< [XSI] X for owner.

// Read, write, execute/search by group.
static uint32_t const FILE_MODE_GROUP_ALL      = S_IRWXG;  ///< [XSI] RWX mask for group.
static uint32_t const FILE_MODE_GROUP_READ     = S_IRGRP;  ///< [XSI] R for group.
static uint32_t const FILE_MODE_GROUP_WRITE    = S_IWGRP;  ///< [XSI] W for group.
static uint32_t const FILE_MODE_GROUP_EXECUTE  = S_IXGRP;  ///< [XSI] X for group.

// Read, write, execute/search by others.
static uint32_t const FILE_MODE_OTHER_ALL      = S_IRWXO;  ///< [XSI] RWX mask for other.
static uint32_t const FILE_MODE_OTHER_READ     = S_IROTH;  ///< [XSI] R for other.
static uint32_t const FILE_MODE_OTHER_WRITE    = S_IWOTH;  ///< [XSI] W for other.
static uint32_t const FILE_MODE_OTHER_EXECUTE  = S_IXOTH;  ///< [XSI] X for other.

static uint32_t const FILE_MODE_S_ISUID        = S_ISUID;  ///< [XSI] set user id on execution.
static uint32_t const FILE_MODE_S_ISGID        = S_ISGID;  ///< [XSI] set group id on execution.
static uint32_t const FILE_MODE_S_ISVTX        = S_ISVTX;  ///< [XSI] directory restrcted delete.
#endif

/**
 * @}
 */

/**
 * List of access mode.
 *
 * @remarks
 *  POSIX: include <sys/unistd.h>
 */
enum AccessModeTable
{
    ACCESS_MODE_EXISTS  = (0   ), ///< F_OK: test for existence of file.
    ACCESS_MODE_EXECUTE = (1<<0), ///< X_OK: test for execute or search permission.
    ACCESS_MODE_WRITE   = (1<<1), ///< W_OK: test for write permission
    ACCESS_MODE_READ    = (1<<2), ///< R_OK: test for read permission.
};

using DirFunction = int (*)(char * buffer, std::size_t * size);

/** For the libuv miscellaneous function. */
static std::string getRepresentationDirectory(DirFunction func)
{
    std::size_t length = libtbag::filesystem::details::MAX_PATH_LENGTH;
    char buffer[MAX_PATH_LENGTH] = {0,};

    if (func(&buffer[0], &length) != 0) {
        return std::string();
    }
    return std::string(buffer);
}

static bool checkAccessMode(std::string const & path, int mode)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_access(nullptr, &request, path.c_str(), mode, nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

/** @ref <http://linux.die.net/man/2/stat> */
static uint64_t getStatus(std::string const & path)
{
    uint64_t result = 0;
    uv_fs_t  request;

    int const ERROR_CODE = uv_fs_stat(nullptr, &request, path.c_str(), nullptr);
    if (ERROR_CODE == 0 && request.result == 0) {
        result = request.statbuf.st_mode;
    }
    uv_fs_req_cleanup(&request);

    return result;
}

static bool checkFileType(std::string const & path, uint64_t type)
{
    return (getStatus(path) & FILE_TYPE_S_IFMT) == type;
}

static uint64_t getPermission(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return getStatus(path) & (FILE_MODE_OWNER_READ | FILE_MODE_OWNER_WRITE);
#else
    return getStatus(path) & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

static uint64_t getFixedPermission(uint64_t mode)
{
#if defined(__PLATFORM_WINDOWS__)
    return mode & (FILE_MODE_OWNER_READ | FILE_MODE_OWNER_WRITE);
#else
    return mode & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

} // namespace uvpp

// -----------------
// Common interface.
// -----------------

std::string getTempDir()
{
    return uvpp::getRepresentationDirectory(&uv_os_tmpdir);
}

std::string getWorkDir()
{
    return uvpp::getRepresentationDirectory(&uv_cwd);
}

std::string getHomeDir()
{
    return uvpp::getRepresentationDirectory(&uv_os_homedir);
}

std::string getExePath()
{
    return uvpp::getRepresentationDirectory(&uv_exepath);
}

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
        string::createRandomString(&buffer[0], buffer.size());
        cursor = prefix + buffer + suffix;

        if (isDirectory(cursor) == false && createDirectory(cursor)) {
            return cursor;
        }
    }

    return std::string();
}

std::string createDefaultTempDir()
{
    std::string temp_dir = getTempDir();
    if (isDirectory(temp_dir) == false) {
        return std::string();
    }

    if (temp_dir[temp_dir.size() - 1] != PATH_SEPARATOR_OF_POSIX) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR_OF_POSIX;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

std::string getRealPath(std::string const & path)
{
    std::string result;
    uv_fs_t request = {0,};

    if (uv_fs_realpath(nullptr, &request, path.c_str(), nullptr) == 0) {
        result = std::string(static_cast<char*>(request.ptr));
    } else {
        result = path;
    }
    uv_fs_req_cleanup(&request);
    return result;
}

bool createDirectoryEx(std::string const & path, int mode)
{
    uv_fs_t request = {0,};
    int const ERROR_CODE = uv_fs_mkdir(nullptr, &request, path.c_str(), mode, nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool createDirectory(std::string const & path)
{
    return createDirectoryEx(path, 0755);
}

bool removeDirectory(std::string const & path)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_rmdir(nullptr, &request, path.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool removeFile(std::string const & path)
{
    return ::remove(path.c_str()) == 0;
}

bool removeAll(std::string const & path)
{
    if (isDirectory(path)) {
        bool result = true;
        for (auto & cursor : scanDir(path)) {
            if (cursor != CURRENT_DIRECTORY_SHORTCUT && cursor != PARENT_DIRECTORY_SHORTCUT) {
                result &= removeAll(path + PATH_SEPARATOR_OF_POSIX + cursor);
            }
        }
        result &= removeDirectory(path);
        return result;
    }
    return removeFile(path);
}

bool rename(std::string const & from, std::string const & to)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_rename(nullptr, &request, from.c_str(), to.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool exists(std::string const & path)
{
    return uvpp::checkAccessMode(path, uvpp::ACCESS_MODE_EXISTS);
}

bool isDirectory(std::string const & path)
{
    return uvpp::checkFileType(path, uvpp::FILE_TYPE_S_IFDIR);
}

bool isRegularFile(std::string const & path)
{
    return uvpp::checkFileType(path, uvpp::FILE_TYPE_S_IFREG);
}

bool isExecutable(std::string const & path)
{
    return uvpp::checkAccessMode(path, uvpp::ACCESS_MODE_EXECUTE);
}

bool isWritable(std::string const & path)
{
    return uvpp::checkAccessMode(path, uvpp::ACCESS_MODE_WRITE);
}

bool isReadable(std::string const & path)
{
    return uvpp::checkAccessMode(path, uvpp::ACCESS_MODE_READ);
}

std::vector<std::string> scanDir(std::string const & path)
{
    std::vector<std::string> result;

    uv_fs_t request;
    uv_dirent_t dictate;

    int const ELEMENT_COUNT = uv_fs_scandir(nullptr, &request, path.c_str(), 0, nullptr);
    if (ELEMENT_COUNT > 0) {
        while (UV_EOF != uv_fs_scandir_next(&request, &dictate)) {
            result.push_back(std::string(dictate.name));
        }
    }
    uv_fs_req_cleanup(&request);

    return result;
}

// --------------------------
// Filesystem path operators.
// --------------------------

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return details::isProhibitedNameWithUtf8(utf8_path, unix::isProhibitedChar<UChar>);
}

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{
    return details::removeLastSeparatorWithUtf8(utf8_path, unix::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{
    return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_POSIX, unix::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{
    return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, unix::isPathSeparatorChar<UChar>);
}

std::string getNativeWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithUtf8(utf8_path));
}

std::string getGenericWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithGenericUtf8(utf8_path));
}

std::string getRootDirWithUtf8(std::string const & utf8_path)
{
    if (utf8_path.size() < 1 || utf8_path[0] != PATH_SEPARATOR_OF_POSIX) {
        return std::string();
    }
    return std::string() + PATH_SEPARATOR_OF_POSIX;
}

bool isAbsoluteWithUtf8(std::string const & utf8_path)
{
    return !getRootDirWithUtf8(utf8_path).empty();
}

bool isRelativeWithUtf8(std::string const & utf8_path)
{
    return !isAbsoluteWithUtf8(utf8_path);
}

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{
    return details::removeLastNodeWithUtf8(utf8_path, unix::isPathSeparatorChar<UChar>);
}

std::string appendParentWithUtf8(std::string const & utf8_path)
{
    return utf8_path + PATH_SEPARATOR_OF_POSIX + PARENT_DIRECTORY_SHORTCUT;
}

std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path)
{
    std::string const UTF8_DELIMITER = std::string() + PATH_SEPARATOR_OF_POSIX;
    std::string const ROOT = getRootDirWithUtf8(utf8_path);

    std::vector<std::string> result;
    result = string::splitUtf8Tokens(removeDuplicateSeparatorsWithUtf8(utf8_path), UTF8_DELIMITER);

    if (!ROOT.empty()) {
        // Force insert the POSIX root directory.
        result.insert(result.begin(), ROOT);
    }
    return result;
}

} // namespace unix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

