/**
 * @file   FsAttribute.cpp
 * @brief  FsAttribute class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/log/Log.hpp>

#include <fcntl.h>
#include <sys/stat.h>

#include <uv.h>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif
#include <libtbag/proxy/windows/String.hpp>

#ifndef _WIN_OR_UNIX
# if defined(__PLATFORM_WINDOWS__)
#  define _WIN_OR_UNIX(v) _##v
# else
#  define _WIN_OR_UNIX(v) v
# endif
#endif // _WIN_OR_UNIX

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// -----------
namespace uv {
// -----------

// ---------------
namespace __impl {
// ---------------

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

// ------------------
} // namespace __impl
// ------------------

bool exists(std::string const & path)
{
    return __impl::checkAccessMode(path, __impl::ACCESS_MODE_EXISTS);
}

bool isDirectory(std::string const & path)
{
    return __impl::checkFileType(path, __impl::FILE_TYPE_S_IFDIR);
}

bool isRegularFile(std::string const & path)
{
    return __impl::checkFileType(path, __impl::FILE_TYPE_S_IFREG);
}

bool isExecutable(std::string const & path)
{
    return __impl::checkAccessMode(path, __impl::ACCESS_MODE_EXECUTE);
}

bool isWritable(std::string const & path)
{
    return __impl::checkAccessMode(path, __impl::ACCESS_MODE_WRITE);
}

bool isReadable(std::string const & path)
{
    return __impl::checkAccessMode(path, __impl::ACCESS_MODE_READ);
}

// --------------
} // namespace uv
// --------------

// ----------------
namespace windows {
// ----------------

// ---------------
namespace __impl {
// ---------------

static DWORD getAttribute(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(INVALID_FILE_ATTRIBUTES);
    return GetFileAttributesW(&proxy::windows::mbsToWcsWithAcp(path)[0]);
}

/**
 * PSECURITY_DESCRIPTOR utility class.
 */
struct SecurityDescriptor
{
    PSECURITY_DESCRIPTOR sd;

    SecurityDescriptor(std::size_t size) : sd(static_cast<PSECURITY_DESCRIPTOR>(::malloc(size)))
    { /* EMPTY. */ }
    ~SecurityDescriptor()
    { ::free(sd); }

    PSECURITY_DESCRIPTOR get()
    { return sd; }

    inline operator bool()
    { return sd != nullptr; }
};

static bool checkPermission(std::string const & path, DWORD permission)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    SECURITY_INFORMATION const SECURITY       = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
    DWORD                const DESIRED_ACCESS = TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE | STANDARD_RIGHTS_READ;

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);

    DWORD sd_length = 0;
    GetFileSecurityW(&WCS_PATH[0], SECURITY, nullptr, 0, &sd_length);
    if (ERROR_INSUFFICIENT_BUFFER != GetLastError()) {
        __tbag_error("Not ERROR_INSUFFICIENT_BUFFER error.");
        return false;
    }

    SecurityDescriptor sd(sd_length);
    if (static_cast<bool>(sd) == false) {
        __tbag_error("PSECURITY_DESCRIPTOR is nullptr.");
        return false;
    }
    if (GetFileSecurityW(&WCS_PATH[0], SECURITY, sd.get(), sd_length, &sd_length) == FALSE) {
        __tbag_error("GetFileSecurityW() ERROR: {}", GetLastError());
        return false;
    }

    HANDLE ptoken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), DESIRED_ACCESS, &ptoken) == FALSE) {
        __tbag_error("OpenProcessToken() ERROR: {}", GetLastError());
        return false;
    }

    HANDLE impersonated_token = NULL;
    if (DuplicateToken(ptoken, SecurityImpersonation, &impersonated_token) == FALSE) {
        __tbag_error("DuplicateToken() ERROR: {}", GetLastError());
        CloseHandle(ptoken);
        return false;
    }

    GENERIC_MAPPING mapping  = {0xFFFFFFFF};
    PRIVILEGE_SET privileges = {0,};
    DWORD granted_access     = 0;
    DWORD privileges_length  = sizeof(privileges);
    BOOL  access_status      = FALSE;
    bool  result             = false;

    mapping.GenericRead    = FILE_GENERIC_READ;
    mapping.GenericWrite   = FILE_GENERIC_WRITE;
    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
    mapping.GenericAll     = FILE_ALL_ACCESS;

    MapGenericMask(&permission, &mapping);
    if (AccessCheck(sd.get(), impersonated_token, permission, &mapping, &privileges, &privileges_length, &granted_access, &access_status)) {
        result = (access_status == TRUE ? true : false);
    } else {
        __tbag_error("AccessCheck() ERROR: {}", GetLastError());
    }

    CloseHandle(impersonated_token);
    CloseHandle(ptoken);

    return result;
}

// ------------------
} // namespace __impl
// ------------------

bool exists(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return (PathFileExistsW(&proxy::windows::mbsToWcsWithAcp(path)[0]) == TRUE);
}

bool isDirectory(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    DWORD const ATTRIBUTES = __impl::getAttribute(path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

bool isRegularFile(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    DWORD const ATTRIBUTES = __impl::getAttribute(path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
}

bool isExecutable(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(path, GENERIC_EXECUTE);
}

bool isWritable(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(path, GENERIC_WRITE);
}

bool isReadable(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(path, GENERIC_READ);
}

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

