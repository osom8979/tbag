/**
 * @file   FsRename.cpp
 * @brief  FsRename class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsRename.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

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

