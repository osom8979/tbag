/**
 * @file   Common.cpp
 * @brief  Common filesystem methods.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/filesystem/Common.hpp>
#include <libtbag/filesystem/details/TemplateFileSystem.hpp-inl>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <fstream>

#if defined(__PLATFORM_WINDOWS__)
namespace __impl = ::libtbag::filesystem::details::windows;
#else
namespace __impl = ::libtbag::filesystem::details::unix;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

std::string getTempDir()
{
    return details::toUtf8Path(__impl::getTempDir);
}

std::string getWorkDir()
{
    return details::toUtf8Path(__impl::getWorkDir);
}

std::string getHomeDir()
{
    return details::toUtf8Path(__impl::getHomeDir);
}

std::string getExePath()
{
    return details::toUtf8Path(__impl::getExePath);
}

std::string getExeDir()
{
    return removeLastNode(details::toUtf8Path(__impl::getExePath));
}

std::string createTempDir(std::string const & utf8_prefix, std::string const & utf8_suffix, std::size_t unique_size)
{
    if (locale::isUtf8GloablEncodingName() == false) {
        std::string native_prefix;
        std::string native_suffix;

        bool is_prefix = locale::convertFromUtf8(utf8_prefix, locale::getGlobalEncodingName(), native_prefix);
        bool is_suffix = locale::convertFromUtf8(utf8_suffix, locale::getGlobalEncodingName(), native_suffix);

        if (is_prefix && is_suffix) {
            return __impl::createTempDir(native_prefix, native_suffix, unique_size);
        }
    }
    return __impl::createTempDir(utf8_prefix, utf8_suffix, unique_size);
}

std::string createDefaultTempDir()
{
    std::string native_path = __impl::createDefaultTempDir();
    if (locale::isUtf8GloablEncodingName() == false) {
        std::string utf8_path;
        if (locale::convertToUtf8(native_path, locale::getGlobalEncodingName(), utf8_path)) {
            return utf8_path;
        }
    }
    return native_path;
}

std::string getRealPath(std::string const & utf8_path)
{
    if (locale::isUtf8GloablEncodingName() == false) {
        std::string native_path;
        if (locale::convertFromUtf8(utf8_path, locale::getGlobalEncodingName(), native_path)) {
            return __impl::getRealPath(native_path);
        }
    }
    return __impl::getRealPath(utf8_path);
}

bool createDirectory(std::string const & utf8_path)
{
    std::string const PARENT = getParent(utf8_path);
    if (isDirectory(PARENT) && isWritable(PARENT) && exists(utf8_path) == false) {
        return details::isFromUtf8Path(utf8_path, __impl::createDirectory);
    }
    return false;
}

bool removeDirectory(std::string const & utf8_path)
{
    if (isDirectory(utf8_path)) {
        return details::isFromUtf8Path(utf8_path, __impl::removeDirectory);
    }
    return false;
}

bool removeFile(std::string const & utf8_path)
{
    if (isRegularFile(utf8_path)) {
        return details::isFromUtf8Path(utf8_path, __impl::removeFile);
    }
    return false;
}

bool removeAll(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::removeAll);
}

bool rename(std::string const & utf8_from, std::string const & utf8_to)
{
    if (locale::isUtf8GloablEncodingName() == false) {
        std::string native_from;
        std::string native_to;

        bool is_from = locale::convertFromUtf8(utf8_from, locale::getGlobalEncodingName(), native_from);
        bool is_to   = locale::convertFromUtf8(utf8_to  , locale::getGlobalEncodingName(), native_to);

        if (is_from && is_to) {
            return __impl::rename(native_from, native_to);
        }
    }
    return __impl::rename(utf8_from, utf8_to);
}

bool exists(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::exists);
}

bool isDirectory(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::isDirectory);
}

bool isRegularFile(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::isRegularFile);
}

bool isExecutable(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::isExecutable);
}

bool isWritable(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::isWritable);
}

bool isReadable(std::string const & utf8_path)
{
    return details::isFromUtf8Path(utf8_path, __impl::isReadable);
}

std::vector<std::string> scanDir(std::string const & utf8_path)
{
    if (locale::isUtf8GloablEncodingName() == false) {
        std::string native_path;

        if (locale::convertFromUtf8(utf8_path, locale::getGlobalEncodingName(), native_path)) {
            std::vector<std::string> result;

            for (auto & node : __impl::scanDir(native_path)) {
                std::string utf8_node;
                if (locale::convertToUtf8(node, locale::getGlobalEncodingName(), utf8_node)) {
                    result.push_back(std::move(utf8_node));
                } else {
                    result.push_back(node);
                }
            }

            return result;
        }
    }

    return __impl::scanDir(utf8_path);
}

std::size_t getFileSize(std::string const & utf8_path)
{
    std::string native_path;
    if (locale::isUtf8GloablEncodingName()) {
        native_path = utf8_path;
    } else {
        locale::convertFromUtf8(utf8_path, locale::getGlobalEncodingName(), native_path);
    }

    std::ifstream f(native_path, std::ios_base::ate | std::ios_base::binary);
    if (f.eof() == false && f.fail() == false) {
        return static_cast<std::size_t>(f.tellg()/*std::streamoff*/);
    }
    return 0U;
}

std::size_t createSimpleTextFile(std::string const & utf8_path, char const * buffer, std::size_t size)
{
    std::string native_path;
    if (locale::isUtf8GloablEncodingName()) {
        native_path = utf8_path;
    } else {
        locale::convertFromUtf8(utf8_path, locale::getGlobalEncodingName(), native_path);
    }

    std::ofstream f(native_path, std::ios_base::binary);
    if (f.is_open()) {
        return static_cast<std::size_t>(f.write(buffer, size).tellp());
    }
    return 0U;
}

// --------------------------
// Filesystem path operators.
// --------------------------

bool isProhibitedName(std::string const & utf8_path)
{
    return __impl::isProhibitedNameWithUtf8(utf8_path);
}

std::string removeLastSeparator(std::string const & utf8_path)
{
    return __impl::removeLastSeparatorWithUtf8(utf8_path);
}

std::string removeDuplicateSeparators(std::string const & utf8_path)
{
    return __impl::removeDuplicateSeparatorsWithUtf8(utf8_path);
}

std::string removeDuplicateSeparatorsWithGeneric(std::string const & utf8_path)
{
    return __impl::removeDuplicateSeparatorsWithGenericUtf8(utf8_path);
}

std::string getNative(std::string const & utf8_path)
{
    return __impl::getNativeWithUtf8(utf8_path);
}

std::string getGeneric(std::string const & utf8_path)
{
    return __impl::getGenericWithUtf8(utf8_path);
}

std::string getRootDir(std::string const & utf8_path)
{
    return __impl::getRootDirWithUtf8(utf8_path);
}

std::string getParent(std::string const & utf8_path)
{
    if (isAbsolute(utf8_path)) {
        return removeLastNode(utf8_path);
    }
    return appendParent(utf8_path);
}

bool isAbsolute(std::string const & utf8_path)
{
    return __impl::isAbsoluteWithUtf8(utf8_path);
}

bool isRelative(std::string const & utf8_path)
{
    return __impl::isRelativeWithUtf8(utf8_path);
}

std::string removeLastNode(std::string const & utf8_path)
{
    return __impl::removeLastNodeWithUtf8(utf8_path);
}

std::string appendParent(std::string const & utf8_path)
{
    return __impl::appendParentWithUtf8(utf8_path);
}

std::vector<std::string> splitNodes(std::string const & utf8_path)
{
    return __impl::splitNodesWithUtf8(utf8_path);
}

std::vector<std::string> splitNodesWithCanonical(std::string const & utf8_path)
{
    std::vector<std::string> result;
    std::vector<std::string> nodes = splitNodes(utf8_path);
    using NodeItr = std::vector<std::string>::iterator;

    NodeItr itr;
    NodeItr end = nodes.end();

    if (__impl::isAbsoluteWithUtf8(utf8_path) == true) {
        itr = nodes.begin();
    } else {
        if (nodes.size() >= 1 && nodes.at(0) == details::HOME_DIRECTORY_SHORTCUT) {
            result = splitNodes(getHomeDir());
            itr = nodes.begin() + 1;
        } else {
            result = splitNodes(getWorkDir());
            itr = nodes.begin();
        }
    }

    for (; itr != end; ++itr) {
        if (*itr == details::CURRENT_DIRECTORY_SHORTCUT) {
            // skip.
        } else if (*itr == details::PARENT_DIRECTORY_SHORTCUT) {
            result.pop_back();
        } else {
            result.push_back(*itr);
        }
    }

    return result;
}

// ---------------------------------
// Filesystem information operators.
// ---------------------------------

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

    (*stream) << " * Temp directory: " << __impl::getTempDir() << std::endl;
    (*stream) << " * Home directory: " << __impl::getHomeDir() << std::endl;
    (*stream) << " * Work directory: " << __impl::getWorkDir() << std::endl;
    (*stream) << " * Work directory (realpath): " << __impl::getRealPath(".") << std::endl;
    (*stream) << " * Exe path: "       << __impl::getExePath() << std::endl;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

