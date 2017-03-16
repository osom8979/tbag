/**
 * @file   FsRemove.cpp
 * @brief  FsRemove class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsRemove.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/filesystem/details/FsScan.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// ----------------
namespace windows {
// ----------------

bool removeDirectory(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcsWithAcp(acp_path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (RemoveDirectoryW(&WCS_PATH[0]) == FALSE) {
        __tbag_error("RemoveDirectoryW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeFile(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcsWithAcp(acp_path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (DeleteFileW(&WCS_PATH[0]) == FALSE) {
        // ERROR_FILE_NOT_FOUND: // The system cannot find the file specified.
        // ERROR_ACCESS_DENIED:  // Access is denied.
        __tbag_error("DeleteFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeAll(std::string const & acp_path)
{
    if (isDirectory(acp_path)) {
        bool result = true;
        for (auto & cursor : scanDir(acp_path)) {
            if (cursor != CURRENT_DIRECTORY_SHORTCUT && cursor != PARENT_DIRECTORY_SHORTCUT) {
                result &= removeAll(acp_path + PATH_SEPARATOR_OF_WINDOWS + cursor);
            }
        }
        result &= removeDirectory(acp_path);
        return result;
    }
    return removeFile(acp_path);
}

// -------------------
} // namespace windows
// -------------------

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

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

