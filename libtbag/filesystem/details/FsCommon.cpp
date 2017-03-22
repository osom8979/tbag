/**
 * @file   FsCommon.cpp
 * @brief  FsCommon class implementation.
 * @author zer0
 * @date   2017-03-21
 */

#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/filesystem/details/windows/FsWinCommon.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#if !defined(__PLATFORM_WINDOWS__)
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
    // @formatter:off
    if (type & DIRENT_UNKNOWN && uv_type == UV_DIRENT_UNKNOWN) { return true; }
    if (type & DIRENT_FILE    && uv_type == UV_DIRENT_FILE   ) { return true; }
    if (type & DIRENT_DIR     && uv_type == UV_DIRENT_DIR    ) { return true; }
    if (type & DIRENT_LINK    && uv_type == UV_DIRENT_LINK   ) { return true; }
    if (type & DIRENT_FIFO    && uv_type == UV_DIRENT_FIFO   ) { return true; }
    if (type & DIRENT_SOCKET  && uv_type == UV_DIRENT_SOCKET ) { return true; }
    if (type & DIRENT_CHAR    && uv_type == UV_DIRENT_CHAR   ) { return true; }
    if (type & DIRENT_BLOCK   && uv_type == UV_DIRENT_BLOCK  ) { return true; }
    // @formatter:on

    return false;
}

static TimeSpec toTimeSpec(uv_timespec_t uv_time)
{
    return TimeSpec{uv_time.tv_sec, uv_time.tv_nsec};
}

static FileState toFileState(uv_stat_t uv_stat)
{
    FileState fs;
    fs.dev      = uv_stat.st_dev;
    fs.mode     = uv_stat.st_mode;
    fs.nlink    = uv_stat.st_nlink;
    fs.uid      = uv_stat.st_uid;
    fs.gid      = uv_stat.st_gid;
    fs.rdev     = uv_stat.st_rdev;
    fs.ino      = uv_stat.st_ino;
    fs.size     = uv_stat.st_size;
    fs.blksize  = uv_stat.st_blksize;
    fs.blocks   = uv_stat.st_blocks;
    fs.flags    = uv_stat.st_flags;
    fs.gen      = uv_stat.st_gen;
    fs.atim     = toTimeSpec(uv_stat.st_atim);
    fs.mtim     = toTimeSpec(uv_stat.st_mtim);
    fs.ctim     = toTimeSpec(uv_stat.st_ctim);
    fs.birthtim = toTimeSpec(uv_stat.st_birthtim);
    return fs;
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
    return __impl::getRepresentationDirectory(&uv_os_tmpdir);
}

std::string getWorkDir()
{
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
#if defined(__PLATFORM_WINDOWS__)
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
#if defined(__PLATFORM_WINDOWS__)
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

bool setMode(std::string const & path, int mode)
{
    return __impl::setMode(::uv_fs_chmod, path.c_str(), mode);
}

bool setModeWithFile(ufile file, int mode)
{
    return __impl::setMode(::uv_fs_fchmod, file, mode);
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
#if defined(__PLATFORM_WINDOWS__)
    return getMode(path) & (FILE_MODE_OWNER_READ | FILE_MODE_OWNER_WRITE);
#else
    return getMode(path) & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

uint64_t getFixedPermission(uint64_t mode)
{
#if defined(__PLATFORM_WINDOWS__)
    return mode & (FILE_MODE_OWNER_READ | FILE_MODE_OWNER_WRITE);
#else
    return mode & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

bool checkFileType(std::string const & path, uint64_t type)
{
    return (getMode(path) & FILE_TYPE_S_IFMT) == type;
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

int read2(ufile file, char const * buffer, std::size_t size, int64_t offset)
{
    uv_buf_t buf;
    buf.base = (char*)buffer;
    buf.len  = size;

    uv_fs_t req = {0,};
    int const READ_SIZE = uv_fs_read(nullptr, &req, file, &buf, 1U, offset, nullptr);
    uv_fs_req_cleanup(&req);
    return READ_SIZE;
}

int write2(ufile file, char const * buffer, std::size_t size, int64_t offset)
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

