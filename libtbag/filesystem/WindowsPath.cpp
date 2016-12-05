/**
 * @file   WindowsPath.cpp
 * @brief  WindowsPath class implementation.
 * @author zer0
 * @date   2016-10-24
 */

#include <libtbag/filesystem/WindowsPath.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/filesystem/Common.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <iterator> // std::distance
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

WindowsPath::WindowsPath() TBAG_NOEXCEPT
{
    // EMPTY.
}

WindowsPath::~WindowsPath()
{
    // EMPTY.
}

std::string WindowsPath::getPathSeparator()
{
    return {details::PATH_SEPARATOR_OF_WINDOWS};
}

std::string WindowsPath::getGenericPathSeparatorString()
{
    return {details::PATH_SEPARATOR_OF_GENERIC};
}

std::string WindowsPath::getRemoveSeparatorRegex()
{
    return R"([\\\/][\\\/]*)";
}

bool WindowsPath::isProhibitedFilename(std::string const & path)
{
    return std::any_of(path.begin(), path.end(), ProhibitedBy());
}

std::string WindowsPath::removeLastSeparator(std::string const & path)
{
    return string::removeRegex(path, getRemoveSeparatorRegex() + '$');
}

std::string WindowsPath::getReplaceStringOfPosix()
{
    return {details::PATH_SEPARATOR_OF_POSIX};
}

std::string WindowsPath::makePreferred(std::string const & path)
{
    return string::replaceRegex(path, getReplaceStringOfPosix(), getPathSeparator());
}

std::string WindowsPath::removeDuplicateSeparators(std::string const & path)
{
    return string::replaceRegex(path, getRemoveSeparatorRegex(), getPathSeparator());
}

std::string WindowsPath::getNative(std::string const & path)
{
    return removeLastSeparator(removeDuplicateSeparators(makePreferred(path)));
}

std::string WindowsPath::getGeneric(std::string const & path)
{
    return string::replaceRegex(getNative(path), getRemoveSeparatorRegex(), getGenericPathSeparatorString());
}

std::string WindowsPath::getRootDir(std::string const & path)
{
    if (path.size() < 2 || path[1] != ':') {
        return std::string();
    }
    if (/* * */('a' <= COMPARE_AND(path[0]) <= 'z')
            || ('A' <= COMPARE_AND(path[0]) <= 'Z')) {
        return path.substr(0, 2);
    }
    return std::string();
}

bool WindowsPath::isAbsolute(std::string const & path)
{
    return !getRootDir(path).empty();
}

bool WindowsPath::isRelative(std::string const & path)
{
    return !isAbsolute(path);
}

std::string WindowsPath::getParent(std::string const & path)
{
    std::string temp = removeLastSeparator(path);
    for (auto itr = temp.rbegin(); itr != temp.rend(); ++itr) {
        if (/* * */*itr == details::PATH_SEPARATOR_OF_WINDOWS
                || *itr == details::PATH_SEPARATOR_OF_POSIX) {
            return temp.substr(0, temp.size() - std::distance(temp.rbegin(), itr));
        }
    }
    return std::string();
}

std::vector<std::string> WindowsPath::splitNodes(std::string const & path)
{
    return string::splitTokens(getGeneric(path), getGenericPathSeparatorString());
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

