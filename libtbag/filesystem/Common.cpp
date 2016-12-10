/**
 * @file   Common.cpp
 * @brief  Common filesystem methods.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/filesystem/Common.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

static std::size_t CREATE_TEMPDIR_RETRY_COUNT = 65535u;
static char const * const CREATE_TEMPDIR_TEMPLATE = "XXXXXX";

std::string createTempDir(std::string const & prefix, std::string const & suffix)
{
    return "";
}

std::string createDefaultTempDir()
{
    return "";
}

bool removeAll(std::string const & path)
{
    if (isDirectory(path)) {
        bool result = true;
        for (auto & cursor : scanDir(path)) {
            if (cursor != "." && cursor != "..") {
                result &= removeAll(path + PATH_SEPARATOR + cursor);
            }
        }
        result &= removeDirectory(path);
        return result;
    }
    return removeFile(path);
}

void printInfos(std::ostream * stream)
{
    if (stream == nullptr) {
        stream = &std::cout;
    }

    (*stream) << "Filesystem information:\n";
    if (isWindowsPlatform()) {
        (*stream) << " * Windows Platform Filesystem.\n";
    }
    if (isUnixLikePlatform()) {
        (*stream) << " * Unix-like Platform Filesystem.\n";
    }

    (*stream) << " * Temp directory: " << getTempDir() << std::endl;
    (*stream) << " * Home directory: " << getHomeDir() << std::endl;
    (*stream) << " * Work directory: " << getWorkDir() << std::endl;
    (*stream) << " * Work directory (realpath): " << getRealPath(".") << std::endl;
    (*stream) << " * Exe path: "       << getExePath() << std::endl;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

