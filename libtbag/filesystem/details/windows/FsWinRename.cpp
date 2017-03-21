/**
 * @file   FsWinRename.cpp
 * @brief  FsWinRename class implementation.
 * @author zer0
 * @date   2017-03-21
 */

#include <libtbag/filesystem/details/windows/FsWinRename.hpp>
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

bool rename(std::string const & acp_path_from, std::string const & acp_path_to)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_FROM = locale::windows::mbsToWcs(acp_path_from);
    std::wstring const WCS_TO   = locale::windows::mbsToWcs(acp_path_to);

    if (WCS_FROM.empty() || WCS_TO.empty()) {
        return false;
    }

    if (MoveFileW(&WCS_FROM[0], &WCS_TO[0]) == FALSE) {
        __tbag_error("MoveFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

