/**
 * @file   FsScan.cpp
 * @brief  FsScan class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsScan.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
# include <Shlwapi.h> // StrCmp
# include <Strsafe.h> // StringCchLength, etc ...
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

std::vector<std::string> scanDir(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::vector<std::string>());

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(path);

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.
    std::size_t length = 0;
    if (FAILED(StringCchLengthW(&WCS_PATH[0], MAX_PATH, &length))) {
        // The value in psz is NULL, cchMax is larger than STRSAFE_MAX_CCH, or psz is longer than cchMax.
        __tbag_error("StringCchLengthW() ERROR.");
        return std::vector<std::string>();
    }

    if (length > (MAX_PATH - 3)) {
        __tbag_error("Directory path is too long.");
        return std::vector<std::string>();
    }

    wchar_t scan_directory[MAX_PATH] = { 0, };
    if (FAILED(StringCchCopyW(scan_directory, MAX_PATH, &WCS_PATH[0]))) {
        __tbag_error("StringCchCopyW ERROR.");
        return std::vector<std::string>();
    }
    if (FAILED(StringCchCatW(scan_directory, MAX_PATH, L"\\*"))) {
        __tbag_error("StringCchCatW ERROR.");
        return std::vector<std::string>();
    }

    WIN32_FIND_DATAW find_data = {0,};
    HANDLE find_handle = FindFirstFileW(scan_directory, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        __tbag_error("FindFirstFileW ERROR: {}", GetLastError());
        return std::vector<std::string>();
    }

    std::vector<std::string> result;

    // List all the files in the directory with some info about them.
    do {
        // DIRECTORY:
        //  @code
        //    find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
        //  @endcode
        // SIZE:
        //  @code
        //    LARGE_INTEGER file_size
        //    file_size.LowPart = find_data.nFileSizeLow;
        //    file_size.HighPart = find_data.nFileSizeHigh;
        //    static_cast<int64_t>(file_size.QuadPart);
        //  @endcode
        if (StrCmpW(L".", find_data.cFileName) != 0 && StrCmpW(L"..", find_data.cFileName) != 0) {
            result.push_back(locale::windows::wcsToMbs(find_data.cFileName));
        }
    } while (FindNextFileW(find_handle, &find_data) == TRUE);

    FindClose(find_handle);
    return result;
}

// -------------------
} // namespace windows
// -------------------

// ---------------
namespace __impl {
// ---------------

static inline bool isDirentType(DirentType type, uv_dirent_type_t uv_type)
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

// ------------------
} // namespace __impl
// ------------------

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

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

