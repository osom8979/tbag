/**
 * @file   FsWinRemove.cpp
 * @brief  FsWinRemove class implementation.
 * @author zer0
 * @date   2017-03-21
 */

#include <libtbag/filesystem/details/windows/FsWinRemove.hpp>
#include <libtbag/filesystem/details/windows/FsWinAttribute.hpp>
#include <libtbag/filesystem/details/windows/FsWinScan.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/log/Log.hpp>

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
namespace windows    {

bool removeDirectory(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);
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

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);
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

bool removeFileWithUtf8(std::string const & utf8_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(utf8_path, locale::windows::CODE_PAGE_UTF8);
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

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

