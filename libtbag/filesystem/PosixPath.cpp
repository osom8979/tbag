/**
 * @file   PosixPath.cpp
 * @brief  PosixPath class implementation.
 * @author zer0
 * @date   2016-10-24
 */

#include <libtbag/filesystem/PosixPath.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/filesystem/Common.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

PosixPath::PosixPath() TBAG_NOEXCEPT
{
    // EMPTY.
}

PosixPath::~PosixPath()
{
    // EMPTY.
}

std::string PosixPath::getPathSeparator()
{
    return {details::PATH_SEPARATOR_OF_POSIX};
}

std::string PosixPath::getGenericPathSeparatorString()
{
    return {details::PATH_SEPARATOR_OF_GENERIC};
}

std::string PosixPath::getRemoveSeparatorRegex()
{
    return R"(\/\/*)";
}

bool PosixPath::isProhibitedFilename(std::string const & path) TBAG_NOEXCEPT
{
    return std::any_of(path.begin(), path.end(), ProhibitedBy());
}

std::string PosixPath::removeLastSeparator(std::string const & path)
{
    return string::removeRegex(path, getRemoveSeparatorRegex() + '$');
}

std::string PosixPath::makePreferred(std::string const & path)
{
    return path;
}

std::string PosixPath::removeDuplicateSeparators(std::string const & path)
{
    return string::replaceRegex(path, getRemoveSeparatorRegex(), getPathSeparator());
}

std::string PosixPath::getNative(std::string const & path)
{
    return removeLastSeparator(removeDuplicateSeparators(makePreferred(path)));
}

std::string PosixPath::getGeneric(std::string const & path)
{
    return getNative(path);
}

std::string PosixPath::getRootDir(std::string const & path)
{
    if (path.size() < 1 || path[0] != details::PATH_SEPARATOR_OF_POSIX) {
        return std::string();
    }
    return getPathSeparator();
}

bool PosixPath::isAbsolute(std::string const & path)
{
    return !getRootDir(path).empty();
}

bool PosixPath::isRelative(std::string const & path)
{
    return !isAbsolute(path);
}

std::string PosixPath::getParent(std::string const & path)
{
    if (path.size() == 1U && path[0] == details::PATH_SEPARATOR_OF_POSIX) {
        return std::string(); // PARENT OF ROOT.
    }

    std::string temp = removeLastSeparator(path);
    std::size_t last_separator_index = temp.rfind(getPathSeparator());

    if (last_separator_index == 0U && temp.size() > 1U && temp[0] == details::PATH_SEPARATOR_OF_POSIX) {
        return getPathSeparator(); // ROOT DIRECTORY.
    } else if (last_separator_index == std::string::npos) {
        return std::string(); // PARENT OF ROOT (Maybe relative path).
    }

    assert(last_separator_index != std::string::npos);
    return temp.substr(0, last_separator_index + 1); // PARENT DIRECTORY.
}

std::vector<std::string> PosixPath::splitNodes(std::string const & path)
{
    std::vector<std::string> result = string::splitTokens(getGeneric(path), getGenericPathSeparatorString());
    std::string root = getRootDir(path);
    if (!root.empty()) {
        // Force insert the POSIX root directory.
        result.insert(result.begin(), root);
    }
    return result;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

