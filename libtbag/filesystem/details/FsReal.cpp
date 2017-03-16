/**
 * @file   FsReal.cpp
 * @brief  FsReal class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsReal.hpp>
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

std::string getRealPath(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = locale::windows::mbsToWcsWithAcp(acp_path);

    if (WCS_PATH.empty()) {
        return std::string();
    }

    wchar_t ** part = {nullptr};
    DWORD const RESERVE_SIZE = GetFullPathNameW(&WCS_PATH[0], 0, nullptr, part);
    std::wstring buffer;
    buffer.resize(RESERVE_SIZE);

    DWORD const COPIED_LENGTH = GetFullPathNameW(&WCS_PATH[0], buffer.size(), &buffer[0], part);
    if (COPIED_LENGTH == 0) {
        __tbag_error("GetFullPathNameW() ERROR: {}", GetLastError());
    }

    buffer.resize(COPIED_LENGTH);
    return locale::windows::wcsToMbsWithAcp(buffer);
}

std::string getLongPathName(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = locale::windows::mbsToWcsWithAcp(acp_path);
    if (WCS_PATH.empty()) {
        return std::string();
    }

    DWORD const RESERVE_SIZE = GetLongPathNameW(&WCS_PATH[0], nullptr, 0);
    std::wstring buffer;
    buffer.resize(RESERVE_SIZE);

    DWORD const COPIED_LENGTH = GetLongPathNameW(&WCS_PATH[0], &buffer[0], RESERVE_SIZE);
    if (COPIED_LENGTH == 0) {
        __tbag_error("GetLongPathNameW() ERROR: {}", GetLastError());
    }
    buffer.resize(COPIED_LENGTH);
    return locale::windows::wcsToMbsWithAcp(buffer);
}

// -------------------
} // namespace windows
// -------------------

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

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

