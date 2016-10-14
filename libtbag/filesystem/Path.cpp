/**
 * @file   Path.cpp
 * @brief  Path class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

Path::Path() noexcept(std::is_nothrow_default_constructible<String>::value)
{
    // EMPTY.
}

Path::Path(String const & path) : _path(path)
{
    // EMPTY.
}

Path::Path(ValueType const * path) : Path(String(path))
{
    // EMPTY.
}

Path::Path(ValueType const * path, update_generic const & UNUSED_PARAM(v)) : Path(String(path))
{
    updateGeneric();
}

Path::Path(String const & path, update_generic const & UNUSED_PARAM(v)) : Path(path)
{
    updateGeneric();
}

Path::Path(ValueType const * path, update_canonical const & UNUSED_PARAM(v)) : Path(String(path))
{
    updateCanonical();
}

Path::Path(String const & path, update_canonical const & UNUSED_PARAM(v)) : Path(path)
{
    updateCanonical();
}

Path::Path(std::vector<String> const & nodes) : Path()
{
    append(nodes);
}

Path::Path(std::initializer_list<String> list) : Path()
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

Path & Path::operator =(ValueType const * path)
{
    _path.assign(path);
    return *this;
}

Path & Path::operator =(String const & path)
{
    if (&(_path) != &path) {
        _path = path;
    }
    return *this;
}

Path & Path::operator =(String && path)
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

bool Path::operator ==(String const & path)
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

Path::String Path::getGenericString() const
{
    return NativePath::getGeneric(_path);
}

Path Path::getGeneric() const
{
    return Path(getGenericString());
}

Path::String Path::getNativeString() const
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

Path::String Path::getCanonicalString() const
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

Path::String Path::getRootDirString() const
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

Path::String Path::append(String const & parent, String const & child)
{
    String result = parent;
    // 문지열이 공백일 경우, 경로 분리자를 삽입하면 루트가 되는 현상을 방지한다.
    if (!parent.empty() && parent.back() != NativePath::PATH_SEPARATOR) {
        result += NativePath::getPathSeparator();
    }
    result += child;
    return result;
}

Path & Path::append(String const & child)
{
    _path = append(this->_path, child);
    return *this;
}

Path & Path::append(std::vector<String> const & nodes)
{
    for (auto & cursor : nodes) {
        append(cursor);
    }
    return *this;
}

Path & Path::operator /=(String const & child)
{
    return append(child);
}

/* FRIEND */ Path operator /(Path const & path, Path::String const & child)
{
    Path result = path;
    result.append(child);
    return result;
}

/* FRIEND */ Path operator /(Path && path, Path::String const & child)
{
    path.append(child);
    return path;
}

Path::operator String() const
{
    return _path;
}

Path::operator ValueType const * () const
{
    return _path.c_str();
}

Path::String Path::getParentString() const
{
    return NativePath::getParent(_path);
}

Path Path::getParent() const
{
    return Path(getParentString());
}

std::vector<Path::String> Path::splitNodes() const
{
    return NativePath::splitNodes(_path);
}

std::vector<Path::String> Path::splitNodesWithCanonical() const
{
    std::vector<String> result;
    std::vector<String> nodes = splitNodes();
    using NodeItr = typename std::vector<String>::iterator;

    NodeItr itr;
    NodeItr end = nodes.end();

    if (isAbsolute() == true) {
        itr = nodes.begin();
    } else {
        if (nodes.size() >= 1 && nodes.at(0) == "~") {
            result = Path(common::getHomeDir()).splitNodes();
            itr = nodes.begin() + 1;
        } else {
            result = Path(common::getWorkDir()).splitNodes();
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

Path::String Path::getName() const
{
    auto nodes = splitNodes();
    if (nodes.rbegin() == nodes.rend()) {
        return String();
    }
    return *nodes.rbegin();
}

bool Path::exists() const
{
    return libtbag::filesystem::common::isExistsMode(_path);
}

bool Path::isExecutable() const
{
    return libtbag::filesystem::common::isExecutableMode(_path);
}

bool Path::isWritable() const
{
    return libtbag::filesystem::common::isWritableMode(_path);
}

bool Path::isReadable() const
{
    return libtbag::filesystem::common::isReadableMode(_path);
}

bool Path::isRegularFile() const
{
    return libtbag::filesystem::common::isRegularFile(_path);
}

bool Path::isDirectory() const
{
    return libtbag::filesystem::common::isDirectory(_path);
}

bool Path::createDir() const
{
    auto parent = getParent();
    if (parent.isDirectory() && parent.isWritable() && exists() == false) {
        return libtbag::filesystem::common::createDir(_path);
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
        return libtbag::filesystem::common::remove(_path);
    }
    return false;
}

bool Path::removeDir() const
{
    if (isDirectory() && isWritable()) {
        return libtbag::filesystem::common::removeDir(_path);
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
    for (auto & path : libtbag::filesystem::common::scanDir(_path)) {
        result.push_back(Path(_path) / path);
    }
    return result;
}

Path Path::getWorkDir()
{
    return Path(libtbag::filesystem::common::getWorkDir());
}

Path Path::getHomeDir()
{
    return Path(libtbag::filesystem::common::getHomeDir());
}

Path Path::getExePath()
{
    return Path(libtbag::filesystem::common::getExePath());
}

Path Path::getExeDir()
{
    return Path(libtbag::filesystem::common::getExeDir());
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

