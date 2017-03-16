/**
 * @file   FsRename.cpp
 * @brief  FsRename class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsRename.hpp>
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

bool rename(std::string const & acp_path_from, std::string const & acp_path_to)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_FROM = locale::windows::mbsToWcsWithAcp(acp_path_from);
    std::wstring const WCS_TO   = locale::windows::mbsToWcsWithAcp(acp_path_to);

    if (WCS_FROM.empty() || WCS_TO.empty()) {
        return false;
    }

    if (MoveFileW(&WCS_FROM[0], &WCS_TO[0]) == FALSE) {
        __tbag_error("MoveFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

// -------------------
} // namespace windows
// -------------------

bool rename(std::string const & from, std::string const & to)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_rename(nullptr, &request, from.c_str(), to.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

