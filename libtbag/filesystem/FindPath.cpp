/**
 * @file   FindPath.cpp
 * @brief  FindPath class implementation.
 * @author zer0
 * @date   2017-08-02
 */

#include <libtbag/filesystem/FindPath.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

std::vector<Path> findUtf8File(std::vector<std::string> const & paths,
                               std::string const & regex,
                               Path::DirentType type)
{
    tDLogD("TEST04: {}", regex);
    std::vector<Path> result;
    for (auto & path : paths) {
        for (auto & child : details::scanDir(path, type)) {
            tDLogD("TEST07 {}", child);
            if (string::isUtf8Match(child, regex)) {
                tDLogD("TEST08");
                result.push_back(Path(path) / child);
            }
        }
    }
    tDLogD("TEST05");
    return result;
}

std::vector<Path> findUtf8ExecuteFile(std::string const & regex)
{
    tDLogD("TEST01");
    std::string path_env;
    if (isFailure(uvpp::getEnv("PATH", path_env))) {
        return std::vector<Path>();
    }
    tDLogD("TEST02: {}", path_env);
    auto const PATHS = string::splitTokens(path_env, std::string(1, details::PATH_SPLITTER));
    tDLogD("TEST03: {}", PATHS.size());
    return findUtf8File(PATHS, regex, Path::DIRENT_FILE | Path::DIRENT_LINK);
}

Path findFirstUtf8ExecuteFile(std::string const & regex)
{
    auto files = findUtf8ExecuteFile(regex);
    if (files.empty()) {
        return Path();
    }
    return files[0];
}

Path findEnvOrExe(std::string const & env, std::string const & regex)
{
    std::string value;
    if (isSuccess(uvpp::getEnv(env, value))) {
        Path com_path(value);
        if (com_path.exists()) {
            return com_path;
        }
    }
    return findFirstUtf8ExecuteFile(regex);
}

// clang-format off
Path findBash       () { return findFirstUtf8ExecuteFile("^bash$"); }
Path findBourneShell() { return findFirstUtf8ExecuteFile("^sh$");   }
Path findCShell     () { return findFirstUtf8ExecuteFile("^(csh|tcsh)$");  }
Path findZShell     () { return findFirstUtf8ExecuteFile("^zsh$");  }
Path findCmd        () { return findEnvOrExe("ComSpec", "^cmd\\.exe$"); }
Path findShell      () { return findEnvOrExe("SHELL", "^(bash|sh|csh|tcsh|zsh)$"); }
// clang-format on

Path findDefaultShell()
{
    if (isWindowsPlatform()) {
        return findCmd();
    }
    return findShell();
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

