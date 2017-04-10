/**
 * @file   Path.cpp
 * @brief  Path class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/details/FsCanonical.hpp>
#include <libtbag/filesystem/details/FsNode.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

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
    return details::getGeneric(_path);
}

Path Path::getGeneric() const
{
    return Path(getGenericString());
}

std::string Path::getNativeString() const
{
    return details::getNative(_path);
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
    return details::getRootDir(_path);
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
    return details::isAbsolute(_path);
}

bool Path::isRelative() const
{
    return details::isRelative(_path);
}

std::string Path::append(std::string const & parent, std::string const & child)
{
    std::string result = parent;
    // "문지열이 공백일 경우 경로 분리자를 삽입하면 루트(Root)가 되는 현상을 방지한다."
    if (!parent.empty() && parent.back() != details::PATH_SEPARATOR) {
        result += details::PATH_SEPARATOR;
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
    return details::getParent(_path);
}

Path Path::getParent() const
{
    return Path(getParentString());
}

std::vector<std::string> Path::splitNodes() const
{
    return details::splitNodes(_path);
}

std::vector<std::string> Path::splitNodesWithCanonical() const
{
    return details::splitNodesWithCanonical(_path);
}

std::string Path::getName() const
{
    auto nodes = splitNodes();
    if (nodes.rbegin() == nodes.rend()) {
        return std::string();
    }
    return *nodes.rbegin();
}

std::string Path::getExtensionName() const
{
    std::string name = getName();
    std::size_t pos = name.find(".");
    if (pos != std::string::npos) {
        return name.substr(pos);
    }
    return std::string();
}

Path::FileState Path::getState() const
{
    FileState state = {0};
    if (details::getState(_path, &state) == false) {
        __tbag_error("Path::getState() result error.");
    }
    return state;
}

bool Path::exists() const
{
    return details::exists(_path);
}

bool Path::isExecutable() const
{
    return details::isExecutable(_path);
}

bool Path::isWritable() const
{
    return details::isWritable(_path);
}

bool Path::isReadable() const
{
    return details::isReadable(_path);
}

bool Path::isRegularFile() const
{
    return details::isRegularFile(_path);
}

bool Path::isDirectory() const
{
    return details::isDirectory(_path);
}

bool Path::createDir(int mode) const
{
    if (details::createDirectoryEx(_path, mode)) {
        return true;
    }

    if (isRootDir()) {
        return false;
    }

    if (getParent().createDir(mode)) {
        return details::createDirectoryEx(_path, mode);
    }
    return false;
}

bool Path::remove() const
{
    if (isDirectory()) {
        return details::removeDirectory(_path);
    } else if (isRegularFile()) {
        return details::removeFile(_path);
    }
    return false;
}

bool Path::removeAll() const
{
    return details::removeAll(_path);
}

std::vector<Path> Path::scanDir(DirentType type) const
{
    std::vector<Path> result;
    for (auto & path : details::scanDir(_path, type)) {
        result.push_back(Path(_path) / path);
    }
    return result;
}

Path Path::getWorkDir()
{
    return Path(details::getWorkDir());
}

Path Path::getHomeDir()
{
    return Path(details::getHomeDir());
}

Path Path::getExePath()
{
    return Path(details::getExePath());
}

Path Path::getExeDir()
{
    return Path(details::removeLastNode(details::getExePath()));
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

