/**
 * @file   FindPath.hpp
 * @brief  FindPath class prototype.
 * @author zer0
 * @date   2017-08-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FINDPATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FINDPATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

TBAG_API std::vector<std::string> splitPaths(std::string const & paths, std::string const & splitter, bool unique = true);
TBAG_API std::vector<std::string> splitPaths(std::string const & paths, bool unique = true);

TBAG_API std::string mergePaths(std::vector<std::string> const & paths, std::string const & splitter, bool unique = true);
TBAG_API std::string mergePaths(std::vector<std::string> const & paths, bool unique = true);

TBAG_API std::vector<std::string> getPathsEnv(std::string const & env);
TBAG_API Err setPathsEnv(std::string const & env, std::vector<std::string> const & paths);

TBAG_API std::vector<Path> findUtf8File(std::vector<std::string> const & paths,
                                        std::string const & regex,
                                        Path::DirentType type = Path::DIRENT_ALL);
TBAG_API std::vector<Path> findUtf8ExecuteFile(std::string const & regex);
TBAG_API Path findFirstUtf8ExecuteFile(std::string const & regex);

TBAG_API Path findEnvOrExe(std::string const & env, std::string const & regex);

TBAG_API Path findBash();
TBAG_API Path findBourneShell();
TBAG_API Path findCShell();
TBAG_API Path findZShell();
TBAG_API Path findCmd();
TBAG_API Path findShell();

TBAG_API Path findDefaultShell();

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FINDPATH_HPP__

