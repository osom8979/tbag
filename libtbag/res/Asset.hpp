/**
 * @file   Asset.hpp
 * @brief  Asset class prototype.
 * @author zer0
 * @date   2016-04-03
 * @date   2016-10-13 (Change namespace: libtbag -> libtbag::res)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/filesystem/Path.hpp>

#include <string>
#include <vector>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 *
 * @remarks
 *  Resource manager class.
 *
 * @warning
 *  Not supported wchar_t string.
 */
class Asset
{
public:
    using Value   = char;
    using String  = std::basic_string<Value>;

    using Path        = ::libtbag::filesystem::Path;
    using PathMap     = std::map<String, Path>;
    using PathMapPair = typename PathMap::value_type;

    static_assert(std::is_same<Value, char>::value
            , "Value must be the same type as char");
    static_assert(std::is_same<Value, typename String::value_type>::value
            , "Value must be the same type as String::value_type");

public:
    /** Default setting for the constructor. */
    struct default_setting { /* EMPTY */ };

public:
    static constexpr char const * const HOME_DIRECTORY_ASSET_NAME = "__HOME__";
    static constexpr char const * const  EXE_DIRECTORY_ASSET_NAME = "__EXE__";

private:
    PathMap _dirs;

public:
    Asset() = default;
    virtual ~Asset() = default;

    /** Construct of Default settings. */
    explicit Asset(default_setting const & UNUSED_PARAM(empty_value)) noexcept(std::is_nothrow_default_constructible<PathMap>::value)
    {
        insertDir(getHomeDirKeyName(), getHomeDirPath());
        insertDir(getExeDirKeyName(), getExeDirPath());
    }

    Asset(PathMap const & dirs) : _dirs(dirs)
    { /* EMPTY. */ }

    Asset(Asset const & obj) = default;
    Asset(Asset && obj) = default;

public:
    Asset & operator =(Asset const & obj) = default;
    Asset & operator =(Asset && obj) = default;

public:
    /**
     * Obtain directory name.
     *
     * @param key [in] Directory key name.
     *
     * @return
     *  - Empty string: If it can't find.
     *  - Otherwise: Successful find.
     */
    Path getDirPath(String const & key) const
    {
        try {
            return _dirs.at(key);
        } catch (std::out_of_range & e) {
            return Path();
        }
    }

    String getDirString(String const & key) const
    {
        return getDirPath(key).getString();
    }

    std::vector<PathMapPair> getDirs() const
    {
        return std::vector<PathMapPair>(_dirs.begin(), _dirs.end());
    }

    /**
     * Insert directory name.
     *
     * @param key   [in] Directory key name.
     * @param value [in] Directory path.
     */
    void insertDir(String const & key, Path const & value)
    {
        _dirs.insert(std::make_pair(key, value));
    }

    /**
     * Insert directory name.
     *
     * @param key   [in] Directory key name.
     * @param value [in] Directory path string.
     */
    void insertDir(String const & key, String const & value)
    {
        insertDir(key, Path(value));
    }

    inline bool empty() const noexcept(noexcept(_dirs.empty()))
    { return _dirs.empty(); }
    inline std::size_t size() const noexcept(noexcept(_dirs.size()))
    { return _dirs.size(); }

public:
    Path find(String const & name)
    {
        Path path;
        for (auto & cursor : _dirs) {
            path = (cursor.second / name);
            if (filesystem::common::existsFile(path) == true) {
                return path;
            }
        }
        return Path();
    }

public:
    std::vector<Path> scanDirWithKey(std::string const & key) const
    {
        return scanDir(getDirPath(key));
    }

    static std::vector<Path> scanDir(Path const & path)
    {
        std::vector<Path> result;
        for (auto & cursor : filesystem::common::scanDir(path.getNativeString())) {
            result.push_back(Path(Path::append(path, cursor)));
        }
        return result;
    }

public:
    bool isDirectoryWithKey(std::string const & key)
    {
        return isDirectory(getDirPath(key));
    }

    static bool isDirectory(Path const & path)
    {
        return filesystem::common::isDirectory(path.getNativeString());
    }

// Static methods.
public:
    /** Obtain HOME directory key name. */
    static String getHomeDirKeyName()
    {
        return String(HOME_DIRECTORY_ASSET_NAME);
    }

    /** Obtain executable file directory key name. */
    static String getExeDirKeyName()
    {
        return String(EXE_DIRECTORY_ASSET_NAME);
    }

    /** Obtain HOME directory path. */
    static Path getHomeDirPath()
    {
        using namespace filesystem::common;
        return Path(getHomeDir());
    }

    /** Obtain executable file directory path. */
    static Path getExeDirPath()
    {
        using namespace filesystem::common;
        return Path(getExeDir());
    }
};

#ifndef CREATE_ASSET_PATH
/** Create a main directory accessor & mutator macro. */
#define CREATE_ASSET_PATH(name, path)                   \
public:                                                 \
    static constexpr Value const * const            \
            ASSET_NAME_KEY_##name = #name;              \
    void insert_##name() {                              \
        insertDir(ASSET_NAME_KEY_##name, path);         \
    }                                                   \
    Path get_##name() const {                           \
        return getDirPath(ASSET_NAME_KEY_##name);       \
    }                                                   \
    bool create_##name() const {                        \
        using namespace ::libtbag::filesystem::common;  \
        return createDir(get_##name());                 \
    }                                                   \
    bool remove_##name() const {                        \
        using namespace ::libtbag::filesystem::common;  \
        return removeDir(get_##name());                 \
    }                                                   \
    bool exists_##name() const {                        \
        return isDirectory(get_##name());               \
    }                                                   \
    std::vector<Path> scan_##name() const {             \
        return scanDir(get_##name());                   \
    }                                                   \
private:
#endif

#ifndef CREATE_ASSET_PATH_SUB
/** Create a subdirectory accessor & mutator macro. */
#define CREATE_ASSET_PATH_SUB(name, sub, path)           \
public:                                                  \
    Path get_##name##_##sub() const {                    \
        return getDirPath(ASSET_NAME_KEY_##name) / path; \
    }                                                    \
    bool create_##name##_##sub() const {                 \
        using namespace ::libtbag::filesystem::common;   \
        return createDir(get_##name##_##sub());          \
    }                                                    \
    bool remove_##name##_##sub() const {                 \
        using namespace ::libtbag::filesystem::common;   \
        return removeDir(get_##name##_##sub());          \
    }                                                    \
    bool exists_##name##_##sub() const {                 \
        return isDirectory(get_##name##_##sub());        \
    }                                                    \
    std::vector<Path> scan_##name##_##sub() const {      \
        return scanDir(get_##name##_##sub());            \
    }                                                    \
private:
#endif

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__

