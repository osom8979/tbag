/**
 * @file   Asset.hpp
 * @brief  Asset class prototype.
 * @author zer0
 * @date   2016-04-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/filesystem/Path.hpp>

#include <vector>
#include <map>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

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
    using ValueType = char;
    using String    = std::basic_string<ValueType>;
    using Path      = ::libtbag::filesystem::Path;
    using PathMap   = std::map<String, Path>;

    static_assert(std::is_same<ValueType, char>::value
            , "ValueType must be the same type as char");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "ValueType must be the same type as String::value_type");

public:
    /** Default setting for the constructor. */
    struct default_setting { __EMPTY_BLOCK__ };

public:
    static constexpr char const * const HOME_DIRECTORY_ASSET_NAME = "__HOME__";
    static constexpr char const * const  EXE_DIRECTORY_ASSET_NAME = "__EXE__";

private:
    PathMap _dirs;

public:
    Asset() noexcept(std::is_nothrow_default_constructible<PathMap>::value) {
        __EMPTY_BLOCK__
    }

    /** Construct of Default settings. */
    explicit Asset(default_setting const & UNUSED_PARAM(empty_value)) {
        this->insertDir(getHomeDirKeyName(), getHomeDirPath());
        this->insertDir(getExeDirKeyName(), getExeDirPath());
    }

    Asset(PathMap const & dirs) {
        this->_dirs = dirs;
    }

    Asset(Asset const & obj) {
        this->copy(obj);
    }

    Asset(Asset && obj) {
        this->swap(obj);
    }

    virtual ~Asset() {
        __EMPTY_BLOCK__
    }

public:
    Asset & operator =(Asset const & obj) {
        return this->copy(obj);
    }

    Asset & operator =(Asset && obj) {
        this->swap(obj);
        return *this;
    }

public:
    Asset & copy(Asset const & obj) {
        if (this != &obj) {
            this->_dirs = obj._dirs;
        }
        return *this;
    }

    void swap(Asset & obj) {
        if (this != &obj) {
            this->_dirs.swap(obj._dirs);
        }
    }

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
    Path getDirPath(String const & key) const {
        auto itr = this->_dirs.find(key);
        if (itr == this->_dirs.end()) {
            return Path();
        }
        return itr->second;
    }

    String getDirString(String const & key) const {
        return getDirPath(key).getString();
    }

    /**
     * Insert directory name.
     *
     * @param key   [in] Directory key name.
     * @param value [in] Directory path.
     */
    void insertDir(String const & key, Path const & value) {
        this->_dirs.insert(std::make_pair(key, value));
    }

    /**
     * Insert directory name.
     *
     * @param key   [in] Directory key name.
     * @param value [in] Directory path string.
     */
    void insertDir(String const & key, String const & value) {
        this->insertDir(key, Path(value));
    }

    std::size_t size() const noexcept(true) {
        return this->_dirs.size();
    }

    std::set<Path> scanDirs(std::string const & dir_name) const {
        std::set<Path> result;
        std::set<String> scan_result;
        Path path_cursor;

        for (auto cursor : this->_dirs) {
            path_cursor  = cursor.second;
            path_cursor /= dir_name;

            scan_result.clear();
            scan_result = filesystem::Common::scanDir(path_cursor.getNativeString());
            for (auto file : scan_result) {
                result.insert(Path(Path::append(path_cursor, file)));
            }
        }
        return result;
    }

// Static methods.
public:
    /** Obtain HOME directory key name. */
    static String getHomeDirKeyName() {
        return String(HOME_DIRECTORY_ASSET_NAME);
    }

    /** Obtain executable file directory key name. */
    static String getExeDirKeyName() {
        return String(EXE_DIRECTORY_ASSET_NAME);
    }

    /** Obtain HOME directory path. */
    static Path getHomeDirPath() {
        using filesystem::Common;
        return Path(Common::getHomeDir());
    }

    /** Obtain executable file directory path. */
    static Path getExeDirPath() {
        using filesystem::Common;
        return Path(Common::getExeDir());
    }

#ifndef CREATE_ASSET_PATH
/** Create a subdirectory accessor & mutator macro. */
#define CREATE_ASSET_PATH(name, path)                           \
    public:                                                     \
        static constexpr ValueType const * const                \
                ASSET_NAME_KEY_##name = #name;                  \
        void insert_##name() {                                  \
            insertDir(ASSET_NAME_KEY_##name, path);             \
        }                                                       \
        Path get_##name() const {                               \
            return getDirPath(ASSET_NAME_KEY_##name);           \
        }                                                       \
        bool create_##name() const {                            \
            return filesystem::Common::createDir(get_##name()); \
        }                                                       \
        bool remove_##name() const {                            \
            return filesystem::Common::removeDir(get_##name()); \
        }                                                       \
    private:
#endif
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

