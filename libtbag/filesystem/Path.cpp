/**
 * @file   Path.cpp
 * @brief  Path class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/Common.hpp>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/locale/Convert.hpp>

#include <algorithm>
#include <utility>
#include <fstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

namespace support_utf8 {

inline static std::string getGlobalEncodingName()
{
    if (isWindowsPlatform()) {
        return std::string("windows-") + locale::getEncoding(locale::getGlobalLocaleName());
    }
    return locale::getEncoding(locale::getGlobalLocaleName());
}

inline static bool isUtf8GloablEncoding()
{
    return locale::isUtf8EncodingName(std::locale(locale::getGlobalLocaleName()));
}

template <typename Predicated>
inline static bool isFromUtf8(std::string const & path, Predicated predicated)
{
    if (isUtf8GloablEncoding() == false) {
        std::string native_path;
        if (locale::convertFromUtf8(path, getGlobalEncodingName(), native_path)) {
            return predicated(native_path);
        }
    }
    return predicated(path);
}

template <typename Predicated>
inline static std::string toUtf8(Predicated predicated)
{
    std::string native_path = predicated();
    if (isUtf8GloablEncoding() == false) {
        std::string utf8_path;
        if (locale::convertToUtf8(native_path, getGlobalEncodingName(), utf8_path)) {
            return utf8_path;
        }
    }
    return native_path;
}

inline static std::vector<std::string> scanDirFromUtf8(std::string const & path)
{
    if (isUtf8GloablEncoding() == false) {
        std::string native_path;

        if (locale::convertFromUtf8(path, getGlobalEncodingName(), native_path)) {
            std::vector<std::string> result;

            for (auto & node : libtbag::filesystem::scanDir(native_path)) {
                std::string utf8_node;
                if (locale::convertToUtf8(node, getGlobalEncodingName(), utf8_node)) {
                    result.push_back(std::move(utf8_node));
                } else {
                    result.push_back(node);
                }
            }
        }
    }

    return libtbag::filesystem::scanDir(path);
}

} // namespace support_utf8

Path::Path() TBAG_NOEXCEPT_EXPR(std::is_nothrow_default_constructible<std::string>::value) : _path()
{
    // EMPTY.
}

Path::Path(std::string const & path) : _path(path)
{
    // EMPTY.
}

Path::Path(char const * path) : Path(std::string(path))
{
    // EMPTY.
}

Path::Path(char const * path, update_generic const & UNUSED_PARAM(v)) : Path(std::string(path))
{
    updateGeneric();
}

Path::Path(std::string const & path, update_generic const & UNUSED_PARAM(v)) : Path(path)
{
    updateGeneric();
}

Path::Path(char const * path, update_canonical const & UNUSED_PARAM(v)) : Path(std::string(path))
{
    updateCanonical();
}

Path::Path(std::string const & path, update_canonical const & UNUSED_PARAM(v)) : Path(path)
{
    updateCanonical();
}

Path::Path(std::vector<std::string> const & nodes) : Path()
{
    append(nodes);
}

Path::Path(std::initializer_list<std::string> list) : Path()
{
    for (auto & cursor : list) {
        append(cursor);
    }
}

Path::Path(Path const & obj) : Path()
{
    copy(obj);
}

Path::Path(Path && obj) : Path()
{
    swap(obj);
}

Path::~Path()
{
    // EMPTY.
}

Path & Path::operator =(char const * path)
{
    _path.assign(path);
    return *this;
}

Path & Path::operator =(std::string const & path)
{
    if (&(_path) != &path) {
        _path = path;
    }
    return *this;
}

Path & Path::operator =(std::string && path)
{
    if (&(_path) != &path) {
        _path.swap(path);
    }
    return *this;
}

Path & Path::operator =(Path const & obj)
{
    return copy(obj);
}

Path & Path::operator =(Path && obj)
{
    swap(obj);
    return *this;
}

bool Path::operator ==(Path const & path)
{
    return getCanonicalString() == path.getCanonicalString();
}

bool Path::operator ==(std::string const & path)
{
    return getCanonicalString() == Path(path).getCanonicalString();
}

Path & Path::copy(Path const & obj)
{
    if (this != &obj) {
        _path = obj._path;
    }
    return *this;
}

void Path::swap(Path && obj)
{
    if (this != &obj) {
        _path.swap(obj._path);
    }
}

void Path::swap(Path & obj)
{
    if (this != &obj) {
        _path.swap(obj._path);
    }
}

std::string Path::getGenericString() const
{
    return getGenericWithUtf8(_path);
}

Path Path::getGeneric() const
{
    return Path(getGenericString());
}

std::string Path::getNativeString() const
{
    return getNativeWithUtf8(_path);
}

Path Path::getNative() const
{
    return Path(getNativeString());
}

Path Path::getCanonical() const
{
    return Path(splitNodesWithCanonical());
}

std::string Path::getCanonicalString() const
{
    return getCanonical().getString();
}

Path & Path::updateGeneric()
{
    setString(getGenericString());
    return *this;
}

Path & Path::updateNative()
{
    setString(getNativeString());
    return *this;
}

Path & Path::updateCanonical()
{
    setString(getCanonicalString());
    return *this;
}

std::string Path::getRootDirString() const
{
    return getRootDirWithUtf8(_path);
}

Path Path::getRootDir() const
{
    return Path(getRootDirString());
}

bool Path::isRootDir() const
{
    return Path(_path) == getRootDir();
}

bool Path::isAbsolute() const
{
    return isAbsoluteWithUtf8(_path);
}

bool Path::isRelative() const
{
    return isRelativeWithUtf8(_path);
}

std::string Path::append(std::string const & parent, std::string const & child)
{
    std::string result = parent;
    // 문지열이 공백일 경우, 경로 분리자를 삽입하면 루트가 되는 현상을 방지한다.
    if (!parent.empty() && parent.back() != PATH_SEPARATOR) {
        result += PATH_SEPARATOR;
    }
    result += child;
    return result;
}

Path & Path::append(std::string const & child)
{
    _path = append(this->_path, child);
    return *this;
}

Path & Path::append(std::vector<std::string> const & nodes)
{
    for (auto & cursor : nodes) {
        append(cursor);
    }
    return *this;
}

Path & Path::operator /=(std::string const & child)
{
    return append(child);
}

/* FRIEND */ Path operator /(Path const & path, std::string const & child)
{
    Path result = path;
    result.append(child);
    return result;
}

/* FRIEND */ Path operator /(Path && path, std::string const & child)
{
    path.append(child);
    return path;
}

Path::operator std::string() const
{
    return _path;
}

Path::operator char const * () const
{
    return _path.c_str();
}

std::string Path::getParentString() const
{
    if (isAbsolute()) {
        return removeLastNodeWithUtf8(_path);
    }
    return appendParentWithUtf8(_path);
}

Path Path::getParent() const
{
    return Path(getParentString());
}

std::vector<std::string> Path::splitNodes() const
{
    return splitNodesWithUtf8(_path);
}

std::vector<std::string> Path::splitNodesWithCanonical() const
{
    std::vector<std::string> result;
    std::vector<std::string> nodes = splitNodes();
    using NodeItr = std::vector<std::string>::iterator;

    NodeItr itr;
    NodeItr end = nodes.end();

    if (isAbsolute() == true) {
        itr = nodes.begin();
    } else {
        if (nodes.size() >= 1 && nodes.at(0) == HOME_DIRECTORY_SHORTCUT) {
            result = Path(getHomeDir()).splitNodes();
            itr = nodes.begin() + 1;
        } else {
            result = Path(getWorkDir()).splitNodes();
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

std::string Path::getName() const
{
    auto nodes = splitNodes();
    if (nodes.rbegin() == nodes.rend()) {
        return std::string();
    }
    return *nodes.rbegin();
}

bool Path::exists() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::exists);
}

bool Path::isExecutable() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::isExecutable);
}

bool Path::isWritable() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::isWritable);
}

bool Path::isReadable() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::isReadable);
}

bool Path::isRegularFile() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::isRegularFile);
}

bool Path::isDirectory() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::isDirectory);
}

bool Path::createDir() const
{
    auto parent = getParent();
    if (parent.isDirectory() && parent.isWritable() && exists() == false) {
        return support_utf8::isFromUtf8(_path, libtbag::filesystem::createDirectory);
    }
    return false;
}

bool Path::createDirWithRecursive() const
{
    // @formatter:off
    if (createDir()) { return  true; }
    if (isRootDir()) { return false; }
    // @formatter:on

    if (getParent().createDirWithRecursive()) {
        return createDir();
    }
    return false;
}

bool Path::remove() const
{
    if (isDirectory()) {
        return removeDir();
    } else if (isRegularFile()) {
        return removeFile();
    }
    return false;
}

bool Path::removeFile() const
{
    if (isRegularFile() && isWritable()) {
        return support_utf8::isFromUtf8(_path, libtbag::filesystem::removeFile);
    }
    return false;
}

bool Path::removeDir() const
{
    if (isDirectory() && isWritable()) {
        return support_utf8::isFromUtf8(_path, libtbag::filesystem::removeDirectory);
    }
    return false;
}

bool Path::removeDirWithRecursive() const
{
    return support_utf8::isFromUtf8(_path, libtbag::filesystem::removeAll);
}

std::vector<Path> Path::scanDir() const
{
    std::vector<Path> result;
    for (auto & path : support_utf8::scanDirFromUtf8(_path)) {
        result.push_back(Path(_path) / path);
    }
    return result;
}

std::size_t Path::size() const
{
    std::string native_path;
    if (support_utf8::isUtf8GloablEncoding()) {
        native_path = _path;
    } else if (locale::convertToUtf8(native_path, support_utf8::getGlobalEncodingName(), native_path) == false) {
        native_path = _path;
    }

    std::ifstream f(native_path, std::ios_base::ate | std::ios_base::binary);
    if (f.eof() == false && f.fail() == false) {
        return static_cast<std::size_t>(f.tellg()/*std::streamoff*/);
    }
    return 0U;
}

Path Path::getWorkDir()
{
    return Path(support_utf8::toUtf8(libtbag::filesystem::getWorkDir));
}

Path Path::getHomeDir()
{
    return Path(support_utf8::toUtf8(libtbag::filesystem::getHomeDir));
}

Path Path::getExePath()
{
    return Path(support_utf8::toUtf8(libtbag::filesystem::getExePath));
}

Path Path::getExeDir()
{
    return Path(removeLastNodeWithUtf8(support_utf8::toUtf8(libtbag::filesystem::getExePath)));
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

