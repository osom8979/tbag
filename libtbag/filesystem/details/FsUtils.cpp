/**
 * @file   FsUtils.cpp
 * @brief  FsUtils class implementation.
 * @author zer0
 * @date   2017-03-02
 */

#include <libtbag/filesystem/details/FsUtils.hpp>
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

using DirFunction = int (*)(char * buffer, std::size_t * size);

/** For the libuv miscellaneous function. */
static std::string getRepresentationDirectory(DirFunction func)
{
    std::size_t const BUFFER_SIZE = MAX_PATH_LENGTH + 1;
    std::size_t length = MAX_PATH_LENGTH;
    char buffer[BUFFER_SIZE] = {0,};

    if (func(&buffer[0], &length) != 0) {
        return std::string();
    }
    return std::string(buffer);
}

// ------------------
} // namespace __impl
// ------------------

std::string getTempDir()
{
    return __impl::getRepresentationDirectory(&uv_os_tmpdir);
}

std::string getWorkDir()
{
    return __impl::getRepresentationDirectory(&uv_cwd);
}

std::string getHomeDir()
{
    return __impl::getRepresentationDirectory(&uv_os_homedir);
}

std::string getExePath()
{
    return __impl::getRepresentationDirectory(&uv_exepath);
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

