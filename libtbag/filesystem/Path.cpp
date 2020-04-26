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

#include <algorithm>
#include <iterator>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

char const * getLibraryPrefix()
{
#if defined(TBAG_PLATFORM_WINDOWS) && !defined(TBAG_PLATFORM_MINGW)
    return details::WINDOWS_SHARED_LIBRARY_PREFIX;
#else
    return details::ELF_SHARED_LIBRARY_PREFIX;
#endif
}

char const * getLibrarySuffix()
{
#if defined(TBAG_PLATFORM_WINDOWS)
# if defined(TBAG_PLATFORM_MINGW)
    return details::MINGW_SHARED_LIBRARY_SUFFIX;
# else  // if defined(TBAG_PLATFORM_MINGW)
    return details::WINDOWS_SHARED_LIBRARY_SUFFIX;
# endif // if defined(TBAG_PLATFORM_MINGW)
#elif defined(TBAG_PLATFORM_MACOS)
    return details::MACHO_SHARED_LIBRARY_SUFFIX;
#else
    return details::ELF_SHARED_LIBRARY_SUFFIX;
#endif
}

char const * getModuleSuffix()
{
#if defined(TBAG_PLATFORM_WINDOWS)
# if defined(TBAG_PLATFORM_MINGW)
    return details::MINGW_SHARED_LIBRARY_SUFFIX;
# else  // if defined(TBAG_PLATFORM_MINGW)
    return details::WINDOWS_SHARED_LIBRARY_SUFFIX;
# endif // if defined(TBAG_PLATFORM_MINGW)
#else
    return details::ELF_SHARED_LIBRARY_SUFFIX;
#endif
}

std::string getLibraryName(std::string const & name)
{
    return std::string(getLibraryPrefix()) + name + getLibrarySuffix();
}

std::string getModuleName(std::string const & name)
{
    return std::string(getLibraryPrefix()) + name + getModuleSuffix();
}

std::string getExecutablePrefix()
{
    return details::ELF_EXECUTABLE_PREFIX;
}

std::string getExecutableSuffix()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return details::WINDOWS_EXECUTABLE_SUFFIX;
#else
    return details::ELF_EXECUTABLE_SUFFIX;
#endif
}

std::string getExecutableName(std::string const & name)
{
    return getExecutablePrefix() + name + getExecutableSuffix();
}

// --------------------
// Path implementation.
// --------------------

Path::Path() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<std::string>::value) : _path()
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

bool Path::operator ==(Path const & path) const
{
    return getGenericString() == path.getGenericString();
}

bool Path::operator !=(Path const & path) const
{
    return !((*this) == path);
}

bool Path::operator ==(std::string const & path) const
{
    return getGenericString() == Path(path).getGenericString();
}

bool Path::operator !=(std::string const & path) const
{
    return !((*this) == path);
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
    // [ko] "문지열이 공백일 경우 경로 분리자를 삽입하면 루트(Root)가 되는 현상을 방지한다." [ko]
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

std::size_t Path::findNode(std::string const & name, std::size_t start) const
{
    auto const nodes = splitNodes();
    auto const itr = std::find(std::next(nodes.begin(), start), nodes.end(), name);
    if (itr == nodes.end()) {
        return nop;
    }
    return std::distance(nodes.begin(), itr);
}

std::size_t Path::findNodeWithCanonical(std::string const & name, std::size_t start) const
{
    auto const nodes = splitNodesWithCanonical();
    auto const itr = std::find(std::next(nodes.begin(), start), nodes.end(), name);
    if (itr == nodes.end()) {
        return nop;
    }
    return std::distance(nodes.begin(), itr);
}

bool Path::existsNode(std::string const & name, std::size_t start) const
{
    auto const nodes = splitNodes();
    return std::find(std::next(nodes.begin(), start), nodes.end(), name) != nodes.end();
}

bool Path::existsNodeWithCanonical(std::string const & name, std::size_t start) const
{
    auto const nodes = splitNodesWithCanonical();
    return std::find(std::next(nodes.begin(), start), nodes.end(), name) != nodes.end();
}

std::string Path::getName() const
{
    auto nodes = splitNodes();
    if (nodes.rbegin() == nodes.rend()) {
        return std::string();
    }
    return *nodes.rbegin();
}

std::string Path::getNameWithoutExtension() const
{
    std::string name = getName();
    std::size_t pos = name.find(details::FILE_EXTENSION_DELIMITER);
    return name.substr(0, pos);
}

std::string Path::getExtensionName() const
{
    std::string name = getName();
    std::size_t pos = name.find(details::FILE_EXTENSION_DELIMITER);
    if (pos != std::string::npos) {
        return name.substr(pos);
    }
    return std::string();
}

bool Path::testSuffix(std::string const & suffix) const
{
    auto path_itr = _path.crbegin();
    auto path_end = _path.crend();
    auto itr = suffix.crbegin();
    auto end = suffix.crend();
    for (; itr != end; ++path_itr, ++itr) {
        if (path_itr == path_end) {
            return false;
        }
        if (*path_itr != *itr) {
            return false;
        }
    }
    return true;
}

Path::FileState Path::getState(bool * result) const
{
    FileState state = { 0, };
    bool const code = details::getState(_path, &state);
    if (result != nullptr) {
        *result = code;
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

std::vector<std::string> Path::scanNameOnly(DirentType type) const
{
    return details::scanDir(_path, type);
}

std::vector<std::string> Path::scanRecurrentNameOnly(DirentType type) const
{
    std::vector<std::string> result;
    scanRecurrentNameOnly(type, Path(_path), Path(), result);
    return result;
}

std::vector<Path> Path::scanDir(DirentType type) const
{
    std::vector<Path> result;
    for (auto & cursor : scanNameOnly(type)) {
        result.push_back(Path(_path) / cursor);
    }
    return result;
}

std::vector<Path> Path::scanRecurrentDir(DirentType type) const
{
    auto const PARENT_DIR = Path(_path);
    std::vector<Path> result;
    for (auto & cursor : scanRecurrentNameOnly(type)) {
        result.push_back(PARENT_DIR / cursor);
    }
    return result;
}

std::vector<std::string> Path::scanRecurrentFiles(
    std::vector<std::string> const & suffixes,
    bool ignore_hidden) const
{
    using namespace libtbag::filesystem;
    if (!isDirectory()) {
        return {};
    }

    std::vector<std::string> dir_names;
    for (auto const & dir : scanRecurrentNameOnly(Path::DIRENT_DIR)) {
        if (dir.empty()) {
            continue;
        }

        bool exists_ignore_node = false;
        for (auto const & node : Path(dir).splitNodes()) {
            if (node.empty()) {
                continue;
            }
            if (node[0] == '.') {
                exists_ignore_node = true;
                break;
            }
        }

        if (ignore_hidden && exists_ignore_node) {
            continue;
        }

        dir_names.emplace_back(dir);
    }

    std::vector<std::string> result;
    for (auto const & dir_name : dir_names) {
        for (auto const & file_name : (*this / dir_name).scanNameOnly(Path::DIRENT_FILE)) {
            if (file_name.empty()) {
                continue;
            }
            if (ignore_hidden && file_name[0] == '.') {
                continue;
            }

            auto const name_path = Path(file_name);
            bool match_extension = false;

            if (suffixes.empty()) {
                match_extension = true;
            } else {
                for (auto const & suffix : suffixes) {
                    if (name_path.testSuffix(suffix)) {
                        match_extension = true;
                        break;
                    }
                }
            }

            if (match_extension) {
                result.emplace_back((Path(dir_name) / file_name).toString());
            }
        }
    }

    return result;
}

void Path::scanRecurrentNameOnly(DirentType type,
                                 Path const & parent_dir,
                                 Path const & node,
                                 std::vector<std::string> & result)
{
    for (auto & dir_name : details::scanDir(parent_dir, Path::DIRENT_DIR)) {
        scanRecurrentNameOnly(type, parent_dir / dir_name, node / dir_name, result);
    }
    for (auto & cursor : details::scanDir(parent_dir, type)) {
        result.push_back(node / cursor);
    }
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

// ----------
// Utilities.
// ----------

Err checkPrepareFiles(std::string const & source_path, std::string const & destination_path, bool check_dir)
{
    return checkPrepareFiles(Path(source_path), Path(destination_path));
}

Err checkPrepareFiles(Path const & source_path, Path const & destination_path, bool check_dir)
{
    if (!source_path.exists()) {
        // Not exists source file.
        return E_ENOENT;
    }

    if (!source_path.isReadable()) {
        // You do not have read permission.
        return E_EACCES;
    }

    if (destination_path.exists()) {
        // File already exists.
        return E_EEXIST;
    }

    Path const DESTINATION_DIR = destination_path.getParent();
    if (check_dir) {
        if (!DESTINATION_DIR.exists()) {
            // Not exists destination directory.
            return E_ENOENT;
        }

        if (!DESTINATION_DIR.isDirectory()) {
            // Not a directory.
            return E_ENOENT;
        }

        if (!DESTINATION_DIR.isWritable()) {
            // You do not have write permission.
            return E_EACCES;
        }
    }

    return E_SUCCESS;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

