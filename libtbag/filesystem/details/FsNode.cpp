/**
 * @file   FsNode.cpp
 * @brief  FsNode class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsNode.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/filesystem/details/FsCanonical.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

#include <unicode/unistr.h>
#include <unicode/uchar.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// -------------
namespace unix {
// -------------

std::string getNative(std::string const & path)
{
    return unix::removeLastSeparator(unix::removeDuplicateSeparators(path));
}

std::string getGeneric(std::string const & path)
{
    return unix::removeLastSeparator(unix::removeDuplicateSeparatorsWithGeneric(path));
}

std::string getRootDir(std::string const & path)
{
    if (path.size() < 1 || path[0] != PATH_SEPARATOR_OF_POSIX) {
        return std::string();
    }
    return std::string() + PATH_SEPARATOR_OF_POSIX;
}

std::string getParent(std::string const & path)
{
    if (unix::isAbsolute(path)) {
        return unix::removeLastNode(path);
    }
    return unix::appendParent(path);
}

bool isAbsolute(std::string const & path)
{
    return !unix::getRootDir(path).empty();
}

bool isRelative(std::string const & path)
{
    return !unix::isAbsolute(path);
}

std::string appendParent(std::string const & path)
{
    return path + PATH_SEPARATOR_OF_POSIX + PARENT_DIRECTORY_SHORTCUT;
}

std::vector<std::string> splitNodes(std::string const & path)
{
    std::string const UTF8_DELIMITER = std::string() + PATH_SEPARATOR_OF_POSIX;
    std::string const ROOT = unix::getRootDir(path);

    std::vector<std::string> result;
    result = string::splitUtf8Tokens(unix::removeDuplicateSeparators(path), UTF8_DELIMITER);

    if (!ROOT.empty()) {
        // Force insert the POSIX root directory.
        result.insert(result.begin(), ROOT);
    }
    return result;
}

// ----------------
} // namespace unix
// ----------------

// ----------------
namespace windows {
// ----------------

std::string appendParent(std::string const & path)
{
    return path + PATH_SEPARATOR_OF_WINDOWS + PARENT_DIRECTORY_SHORTCUT;
}

std::vector<std::string> splitNodes(std::string const & path)
{
    std::string const UTF8_DELIMITER = std::string() + PATH_SEPARATOR_OF_WINDOWS;
    return string::splitUtf8Tokens(removeDuplicateSeparators(path), UTF8_DELIMITER);
}

std::string getNative(std::string const & path)
{
    return windows::removeLastSeparator(windows::removeDuplicateSeparators(path));
}

std::string getGeneric(std::string const & path)
{
    return windows::removeLastSeparator(windows::removeDuplicateSeparatorsWithGeneric(path));
}

std::string getRootDir(std::string const & original_path)
{
    if (original_path.empty()) {
        return std::string();
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(original_path.c_str()));
    int32_t const PATH_LENGTH = path.length();

    if (path.length() < 2 || u_isalpha(path.charAt(0)) == false || path.charAt(1) != ':') {
        return std::string();
    }

    icu::UnicodeString buffer = path.tempSubString(0, 2);
    assert(buffer.length() == 2);

    std::string result;
    buffer.toUTF8String(result);
    return result;
}

std::string getParent(std::string const & path)
{
    if (windows::isAbsolute(path)) {
        return windows::removeLastNode(path);
    }
    return windows::appendParent(path);
}

bool isAbsolute(std::string const & path)
{
    return !windows::getRootDir(path).empty();
}

bool isRelative(std::string const & path)
{
    return !windows::isAbsolute(path);
}

// -------------------
} // namespace windows
// -------------------

std::vector<std::string> splitNodesWithCanonical(std::string const & path)
{
    std::vector<std::string> result;
    std::vector<std::string> nodes = splitNodes(path);
    using NodeItr = std::vector<std::string>::iterator;

    NodeItr itr;
    NodeItr end = nodes.end();

    if (isAbsolute(path) == true) {
        itr = nodes.begin();
    } else {
        if (nodes.size() >= 1 && nodes.at(0) == HOME_DIRECTORY_SHORTCUT) {
            result = splitNodes(getHomeDir());
            itr = nodes.begin() + 1;
        } else {
            result = splitNodes(getWorkDir());
            itr = nodes.begin();
        }
    }

    for (; itr != end; ++itr) {
        if (*itr == CURRENT_DIRECTORY_SHORTCUT) {
            // skip.
        } else if (*itr == PARENT_DIRECTORY_SHORTCUT) {
            result.pop_back();
        } else {
            result.push_back(*itr);
        }
    }

    return result;
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

