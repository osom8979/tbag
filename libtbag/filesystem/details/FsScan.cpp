/**
 * @file   FsScan.cpp
 * @brief  FsScan class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsScan.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// ---------------
namespace __impl {
// ---------------

inline static bool isDirentType(DirentType type, uv_dirent_type_t uv_type)
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

