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

#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

std::vector<std::string> splitPaths(std::string const & paths, std::string const & splitter, bool unique)
{
    if (unique) {
        auto const tokens = libtbag::string::splitTokens(paths, splitter);
        auto const temp = std::set<std::string>(tokens.begin(), tokens.end());
        return std::vector<std::string>(temp.begin(), temp.end());
    } else {
        return libtbag::string::splitTokens(paths, splitter);
    }
}

std::vector<std::string> splitPaths(std::string const & paths, bool unique)
{
    return splitPaths(paths, std::string(1, libtbag::filesystem::details::PATH_SPLITTER), unique);
}

std::string mergePaths(std::vector<std::string> const & paths, std::string const & splitter, bool unique)
{
    if (unique) {
        auto const temp = std::set<std::string>(paths.begin(), paths.end());
        return libtbag::string::mergeTokens(temp.begin(), temp.end(), splitter);
    } else {
        return libtbag::string::mergeTokens(paths, splitter);
    }
}

std::string mergePaths(std::vector<std::string> const & paths, bool unique)
{
    return mergePaths(paths, std::string(1, libtbag::filesystem::details::PATH_SPLITTER), unique);
}

std::vector<std::string> getPathsEnv(std::string const & env)
{
    std::string path_env;
    if (isFailure(libtbag::uvpp::getEnv(env, path_env))) {
        return {};
    }
    return splitPaths(path_env);
}

Err setPathsEnv(std::string const & env, std::vector<std::string> const & paths)
{
    return libtbag::uvpp::setEnv(env, mergePaths(paths));
}

std::vector<Path> findUtf8File(std::vector<std::string> const & paths,
                               std::string const & regex,
                               Path::DirentType type)
{
    std::vector<Path> result;
    for (auto & path : paths) {
        for (auto & child : details::scanDir(path, type)) {
            if (string::isUtf8Match(child, regex)) {
                result.push_back(Path(path) / child);
            }
        }
    }
    return result;
}

std::vector<Path> findUtf8ExecuteFile(std::string const & regex)
{
    return findUtf8File(getPathsEnv("PATH"), regex, Path::DIRENT_FILE|Path::DIRENT_LINK); // NOLINT
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

