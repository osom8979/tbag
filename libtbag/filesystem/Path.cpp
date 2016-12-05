/**
 * @file   Path.cpp
 * @brief  Path class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/Common.hpp>

#include <fstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

Path::Path() TBAG_NOEXCEPT_EXPR(std::is_nothrow_default_constructible<std::string>::value)
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
    return NativePath::getGeneric(_path);
}

Path Path::getGeneric() const
{
    return Path(getGenericString());
}

std::string Path::getNativeString() const
{
    return NativePath::getNative(_path);
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
    return NativePath::getRootDir(_path);
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
    return NativePath::isAbsolute(_path);
}

bool Path::isRelative() const
{
    return NativePath::isRelative(_path);
}

std::string Path::append(std::string const & parent, std::string const & child)
{
    std::string result = parent;
    // 문지열이 공백일 경우, 경로 분리자를 삽입하면 루트가 되는 현상을 방지한다.
    if (!parent.empty() && parent.back() != NativePath::getPathSeparator()[0]) {
        result += NativePath::getPathSeparator();
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
    return NativePath::getParent(_path);
}

Path Path::getParent() const
{
    return Path(getParentString());
}

std::vector<std::string> Path::splitNodes() const
{
    return NativePath::splitNodes(_path);
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
        if (nodes.size() >= 1 && nodes.at(0) == "~") {
            result = Path(getHomeDir()).splitNodes();
            itr = nodes.begin() + 1;
        } else {
            result = Path(getWorkDir()).splitNodes();
            itr = nodes.begin();
        }
    }

    for (; itr != end; ++itr) {
        if (*itr == ".") {
            // skip.
        } else if (*itr == "..") {
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
    return libtbag::filesystem::exists(_path);
}

bool Path::isExecutable() const
{
    return libtbag::filesystem::isExecutable(_path);
}

bool Path::isWritable() const
{
    return libtbag::filesystem::isWritable(_path);
}

bool Path::isReadable() const
{
    return libtbag::filesystem::isReadable(_path);
}

bool Path::isRegularFile() const
{
    return libtbag::filesystem::isRegularFile(_path);
}

bool Path::isDirectory() const
{
    return libtbag::filesystem::isDirectory(_path);
}

bool Path::createDir() const
{
    auto parent = getParent();
    if (parent.isDirectory() && parent.isWritable() && exists() == false) {
        return libtbag::filesystem::createDirectory(_path);
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
        return libtbag::filesystem::removeFile(_path);
    }
    return false;
}

bool Path::removeDir() const
{
    if (isDirectory() && isWritable()) {
        return libtbag::filesystem::removeDirectory(_path);
    }
    return false;
}

bool Path::removeDirWithRecursive() const
{
    bool all_success = true;
    for (auto & path : scanDir()) {
        if (path.isDirectory()) {
            all_success &= path.removeDirWithRecursive();
        } else if (path.isRegularFile()) {
            all_success &= path.removeFile();
        } else {
            all_success = false;
        }
    }

    return all_success & remove();
}

std::vector<Path> Path::scanDir() const
{
    std::vector<Path> result;
    for (auto & path : libtbag::filesystem::scanDir(_path)) {
        result.push_back(Path(_path) / path);
    }
    return result;
}

std::size_t Path::size() const
{
    std::ifstream f(getCanonicalString(), std::ios_base::ate | std::ios_base::binary);
    if (f.eof() == false && f.fail() == false) {
        return f.tellg();
    }
    return 0;
}

Path Path::getWorkDir()
{
    return Path(libtbag::filesystem::getWorkDir());
}

Path Path::getHomeDir()
{
    return Path(libtbag::filesystem::getHomeDir());
}

Path Path::getExePath()
{
    return Path(libtbag::filesystem::getExePath());
}

Path Path::getExeDir()
{
    return Path(libtbag::filesystem::getExePath()).getParent();
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

