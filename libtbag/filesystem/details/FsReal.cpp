/**
 * @file   FsReal.cpp
 * @brief  FsReal class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsReal.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif
#include <libtbag/proxy/windows/String.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// -----------
namespace uv {
// -----------

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

// --------------
} // namespace uv
// --------------

// ----------------
namespace windows {
// ----------------

std::string getRealPath(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);

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
    return proxy::windows::wcsToMbsWithAcp(buffer);
}

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

