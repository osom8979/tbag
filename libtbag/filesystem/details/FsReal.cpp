/**
 * @file   FsReal.cpp
 * @brief  FsReal class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsReal.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

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

