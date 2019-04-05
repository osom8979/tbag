/**
 * @file   FsCommon.cpp
 * @brief  FsCommon class implementation.
 * @author zer0
 * @date   2017-03-21
 */

#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/filesystem/details/windows/FsWinCommon.hpp>
#include <libtbag/filesystem/details/UvFs-inl.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#if !defined(TBAG_PLATFORM_WINDOWS)
# include <sys/types.h>
# include <sys/stat.h>
#endif

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// ---------------
namespace __impl {
// ---------------

using DirFunction = int (*)(char * buffer, std::size_t * size);

/** For the libuv miscellaneous function. */
static std::string getRepresentationDirectory(DirFunction func)
{
    std::size_t const BUFFER_SIZE = MAX_PATH_LENGTH + 1;
    std::size_t length = MAX_PATH_LENGTH;
    char buffer[BUFFER_SIZE] = {0,};

    if (func(&buffer[0], &length) != 0) {
        return std::string();
    }
    return std::string(buffer);
}

inline static bool isDirentType(DirentType type, uv_dirent_type_t uv_type)
{
    // clang-format off
    if (type & DIRENT_UNKNOWN && uv_type == UV_DIRENT_UNKNOWN) { return true; }
    if (type & DIRENT_FILE    && uv_type == UV_DIRENT_FILE   ) { return true; }
    if (type & DIRENT_DIR     && uv_type == UV_DIRENT_DIR    ) { return true; }
    if (type & DIRENT_LINK    && uv_type == UV_DIRENT_LINK   ) { return true; }
    if (type & DIRENT_FIFO    && uv_type == UV_DIRENT_FIFO   ) { return true; }
    if (type & DIRENT_SOCKET  && uv_type == UV_DIRENT_SOCKET ) { return true; }
    if (type & DIRENT_CHAR    && uv_type == UV_DIRENT_CHAR   ) { return true; }
    if (type & DIRENT_BLOCK   && uv_type == UV_DIRENT_BLOCK  ) { return true; }
    // clang-format on

    return false;
}

template <typename Predicated, typename Param>
static bool getState(Predicated predicated, Param param, FileState * state)
{
    uv_fs_t request;
    int const ERROR_CODE = predicated(nullptr, &request, param, nullptr);
    if (state != nullptr && ERROR_CODE == 0 && request.result == 0) {
        *state = toFileState(request.statbuf);
    }
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0 && request.result == 0;
}

template <typename Predicated, typename Param>
static bool setMode(Predicated predicated, Param param, int mode)
{
    uv_fs_t request;
    int const ERROR_CODE = predicated(nullptr, &request, param, mode, nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

// ------------------
} // namespace __impl
// ------------------

std::string getTempDir()
{
    // On Windows, uv_os_tmpdir() uses GetTempPathW().
    //
    // On all other operating systems, uv_os_tmpdir() uses the first environment variable found
    // in the ordered list TMPDIR, TMP, TEMP, and TEMPDIR.
    // If none of these are found, the path "/tmp" is used, or, on Android, "/data/local/tmp" is used.
    // The temp directory is stored in buffer.
    // When uv_os_tmpdir() is called, size indicates the maximum size of buffer.
    // On success size is set to the string length of buffer (which does not include the terminating null).
    // On UV_ENOBUFS failure size is set to the required length for buffer, including the null byte.
    return __impl::getRepresentationDirectory(&uv_os_tmpdir);
}

std::string getWorkDir()
{
    // If the current working directory is too large to fit in buffer,
    // this function returns UV_ENOBUFS, and sets size to the required length, including the null terminator.
    //
    // Changed in version 1.1.0:
    //  - On Unix the path no longer ends in a slash.
    //
    // Changed in version 1.9.0:
    //  - the returned length includes the terminating null byte on UV_ENOBUFS,
    //     and the buffer is null terminated on success.
    return __impl::getRepresentationDirectory(&uv_cwd);
}

std::string getHomeDir()
{
    // On Windows, uv_os_homedir() first checks the USERPROFILE environment variable using GetEnvironmentVariableW().
    // If USERPROFILE is not set, GetUserProfileDirectoryW() is called.
    //
    // On all other operating systems, uv_os_homedir() first checks the HOME environment variable using getenv(3).
    // If HOME is not set, getpwuid_r(3) is called. The user's home directory is stored in buffer.
    // When uv_os_homedir() is called, size indicates the maximum size of buffer.
    // On success size is set to the string length of buffer.
    // On UV_ENOBUFS failure size is set to the required length for buffer, including the null byte.
    return __impl::getRepresentationDirectory(&uv_os_homedir);
}

std::string getExePath()
{
    return __impl::getRepresentationDirectory(&uv_exepath);
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

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; retry < CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
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

bool removeDirectory(std::string const & path)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_rmdir(nullptr, &request, path.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool removeFile(std::string const & path)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return windows::removeFileWithUtf8(path);
#else
    return ::remove(path.c_str()) == 0;
#endif
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

int setUserMask(int mode)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return 0;
#else
    return static_cast<int>(::umask(static_cast<mode_t>(mode)));
#endif
}

bool checkAccessMode(std::string const & path, int mode)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_access(nullptr, &request, path.c_str(), mode, nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool exists(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_EXISTS);
}

bool isExecutable(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_EXECUTE);
}

bool isWritable(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_WRITE);
}

bool isReadable(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_READ);
}

bool getState(std::string const & path, FileState * state)
{
    return __impl::getState(::uv_fs_stat, path.c_str(), state);
}

bool getStateWithFile(ufile file, FileState * state)
{
    return __impl::getState(::uv_fs_fstat, file, state);
}

bool getStateWithLink(std::string const & path, FileState * state)
{
    return __impl::getState(::uv_fs_lstat, path.c_str(), state);
}

uint64_t getMode(std::string const & path)
{
    FileState state = {0,};
    if (getState(path, &state)) {
        return state.mode;
    }
    return 0;
}

std::size_t getSize(std::string const & path)
{
    FileState state = {0,};
    if (getState(path, &state)) {
        return state.size;
    }
    return 0;
}

uint64_t getPermission(std::string const & path)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return getMode(path) & (FILE_MODE_OWNER_READ | FILE_MODE_OWNER_WRITE);
#else
    return getMode(path) & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

uint64_t getFixedPermission(uint64_t mode)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return mode & (FILE_MODE_OWNER_READ | FILE_MODE_OWNER_WRITE);
#else
    return mode & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

bool setMode(std::string const & path, int mode)
{
    return __impl::setMode(::uv_fs_chmod, path.c_str(), mode);
}

bool setModeWithFile(ufile file, int mode)
{
    return __impl::setMode(::uv_fs_fchmod, file, mode);
}

bool checkFileType(std::string const & path, uint64_t type)
{
    return (getMode(path) & FILE_TYPE_S_IFMT) == type;
}

bool isDirectory(std::string const & path)
{
    return checkFileType(path, FILE_TYPE_S_IFDIR);
}

bool isRegularFile(std::string const & path)
{
    return checkFileType(path, FILE_TYPE_S_IFREG);
}

std::string getRealPath(std::string const & path)
{
    // Equivalent to realpath(3) on Unix.
    // Windows uses GetFinalPathNameByHandle.
    //
    // Warning:
    //  This function has certain platform specific caveats that were discovered when used in Node.
    //   - macOS and other BSDs:
    //      this function will fail with UV_ELOOP if more than 32 symlinks are found while resolving the given path.
    //      This limit is hardcoded and cannot be sidestepped.
    //   - Windows:
    //      while this function works in the common case, there are a number of corner cases where it doesn't:
    //       - Paths in ramdisk volumes created by tools which sidestep the Volume Manager (such as ImDisk) cannot be resolved.
    //       - Inconsistent casing when using drive letters.
    //       - Resolved path bypasses subst’d drives.
    //
    // While this function can still be used, it’s not recommended if scenarios such as the above need to be supported.
    //
    // Note:
    //  This function is not implemented on Windows XP and Windows Server 2003. On these systems, UV_ENOSYS is returned.
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

bool link(std::string const & path, std::string const & new_path)
{
    uv_fs_t request = {0,};
    int const ERROR_CODE = uv_fs_link(nullptr, &request, path.c_str(), new_path.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool symlink(std::string const & path, std::string const & new_path, int flags)
{
    uv_fs_t request = {0,};
    int const ERROR_CODE = uv_fs_symlink(nullptr, &request, path.c_str(), new_path.c_str(), flags, nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool unlink(std::string const & path)
{
    uv_fs_t request = {0,};
    int const ERROR_CODE = uv_fs_unlink(nullptr, &request, path.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

std::vector<std::string> scanDir(std::string const & path, DirentType type)
{
    // Equivalent to scandir(3), with a slightly different API.
    //
    // Once the callback for the request is called, the user can use uv_fs_scandir_next()
    // to get ent populated with the next directory entry data.
    // When there are no more entries UV_EOF will be returned.
    //
    // Note:
    //  On Linux, getting the type of an entry is only supported by some filesystems
    //  (btrfs, ext2, ext3 and ext4 at the time of this writing), check the getdents(2) man page.

    std::vector<std::string> result;

    uv_fs_t request;
    uv_dirent_t dictate;

    int const ELEMENT_COUNT = uv_fs_scandir(nullptr, &request, path.c_str(), 0, nullptr);
    if (ELEMENT_COUNT > 0) {
        while (UV_EOF != uv_fs_scandir_next(&request, &dictate)) {
            if (__impl::isDirentType(type, dictate.type)) {
                result.push_back(std::string(dictate.name));
            }
        }
    }
    uv_fs_req_cleanup(&request);

    return result;
}

ufile open(std::string const & path, int flags, int mode)
{
    uv_fs_t req = {0,};
    ufile f = uv_fs_open(nullptr, &req, path.c_str(), flags, mode, nullptr);
    uv_fs_req_cleanup(&req);
    return f;
}

bool close(ufile file)
{
    uv_fs_t req = {0,};
    int const ERROR_CODE = uv_fs_close(nullptr, &req, file, nullptr);
    uv_fs_req_cleanup(&req);
    return ERROR_CODE == 0;
}

int read(ufile file, binf const * infos, std::size_t infos_size, int64_t offset)
{
    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    uv_fs_t req = {0,};
    int const READ_SIZE = uv_fs_read(nullptr, &req, file,
                                     &uv_infos[0], static_cast<unsigned int>(uv_infos.size()),
                                     offset, nullptr);
    uv_fs_req_cleanup(&req);
    return READ_SIZE;
}

int read(ufile file, char * buffer, std::size_t size, int64_t offset)
{
    uv_buf_t buf;
    buf.base = (char*)buffer;
    buf.len  = size;

    uv_fs_t req = {0,};
    int const READ_SIZE = uv_fs_read(nullptr, &req, file, &buf, 1U, offset, nullptr);
    uv_fs_req_cleanup(&req);
    return READ_SIZE;
}

int write(ufile file, binf const * infos, std::size_t infos_size, int64_t offset)
{
    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    uv_fs_t req = {0,};
    int const WRITE_SIZE = uv_fs_write(nullptr, &req, file,
                                       &uv_infos[0], static_cast<unsigned int>(uv_infos.size()),
                                       offset, nullptr);
    uv_fs_req_cleanup(&req);
    return WRITE_SIZE;
}

int write(ufile file, char const * buffer, std::size_t size, int64_t offset)
{
    uv_buf_t buf;
    buf.base = (char*)buffer;
    buf.len  = size;

    uv_fs_t req = {0,};
    int const WRITE_SIZE = uv_fs_write(nullptr, &req, file, &buf, 1U, offset, nullptr);
    uv_fs_req_cleanup(&req);
    return WRITE_SIZE;
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

