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
    (*stream) << " * Exe path: "       << getExePath() << std::endl;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

