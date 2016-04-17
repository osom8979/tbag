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

constexpr char const * const HOME_DIRECTORY_ASSET_NAME = "__HOME__";
constexpr char const * const  EXE_DIRECTORY_ASSET_NAME = "__EXE__";

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 *
 * @remarks
 *  Resource manager class.
 */
class Asset
{
public:
    using String    = std::string;
    using ValueType = typename String::value_type;
    using PathMap   = std::map<String, String>;

    static_assert(std::is_same<ValueType, char>::value
            , "ValueType must be the same type as char");

public:
    /** Default setting for the constructor. */
    struct default_setting { __EMPTY_BLOCK__ };

private:
    PathMap _dirs;

public:
    Asset() noexcept(std::is_nothrow_default_constructible<PathMap>::value) {
        __EMPTY_BLOCK__
    }

    /** Construct of Default settings. */
    explicit Asset(default_setting const & UNUSED_PARAM(empty_value)) {
        using filesystem::Common;
        this->_dirs.insert(std::make_pair(getHomeDirKeyName(), Common::getHomeDir()));
        this->_dirs.insert(std::make_pair(getExeDirKeyName(), Common::getExeDir()));
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

    ~Asset() {
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
    String getDir(String const & key) const {
        auto itr = this->_dirs.find(key);
        if (itr == this->_dirs.end()) {
            return "";
        }
        return itr->second;
    }

    /**
     * Insert directory name.
     *
     * @param key   [in] Directory key name.
     * @param value [in] Directory path string.
     */
    void insertDir(String const & key, String const & value) {
        this->_dirs.insert(std::make_pair(key, value));
    }

    std::size_t size() const noexcept(true) {
        return this->_dirs.size();
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
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

