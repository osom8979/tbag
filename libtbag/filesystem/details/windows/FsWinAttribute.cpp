/**
 * @file   FsWinAttribute.cpp
 * @brief  FsWinAttribute class implementation.
 * @author zer0
 * @date   2017-03-20
 */

#include <libtbag/filesystem/details/windows/FsWinAttribute.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/log/Log.hpp>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
# include <Shlwapi.h> // PathFileExists
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

// ---------------
namespace __impl {
// ---------------

static DWORD getAttribute(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(INVALID_FILE_ATTRIBUTES);
    return GetFileAttributesW(&locale::windows::mbsToWcs(path)[0]);
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

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(path);

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

bool exists(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return (PathFileExistsW(&locale::windows::mbsToWcs(acp_path)[0]) == TRUE);
}

bool isDirectory(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    DWORD const ATTRIBUTES = __impl::getAttribute(acp_path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

bool isRegularFile(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    DWORD const ATTRIBUTES = __impl::getAttribute(acp_path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
}

bool isExecutable(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(acp_path, GENERIC_EXECUTE);
}

bool isWritable(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(acp_path, GENERIC_WRITE);
}

bool isReadable(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(acp_path, GENERIC_READ);
}

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

