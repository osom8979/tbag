/**
 * @file   FsAttribute.cpp
 * @brief  FsAttribute class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/log/Log.hpp>

#if !defined(__PLATFORM_WINDOWS__)
#include <sys/types.h>
#include <sys/stat.h>
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

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

