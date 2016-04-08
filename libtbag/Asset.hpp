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

char const * const HOME_DIRECTORY_ASSET_NAME = "__HOME__";
char const * const  EXE_DIRECTORY_ASSET_NAME = "__EXE__";

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
    /** Default setting for the constructor. */
    struct default_setting { /* Empty Body. */ };

public:
    using PathMap = std::map<std::string, std::string>;

private:
    PathMap dirs;

public:
    Asset() {
        __EMPTY_BLOCK__;
    }

    /** Construct of Default settings. */
    explicit Asset(default_setting const & UNUSED_PARAM(empty_value)) {
        using filesystem::Common;
        this->dirs.insert(std::make_pair(getHomeDirKeyName(), Common::getHomeDir()));
        this->dirs.insert(std::make_pair(getExeDirKeyName(), Common::getExeDir()));
    }

    Asset(PathMap const & dirs) {
        this->dirs = dirs;
    }

    Asset(Asset const & obj) {
        this->copy(obj);
    }

    Asset(Asset && obj) {
        this->swap(obj);
    }

    virtual ~Asset() {
        this->dirs.clear();
    }

public:
    Asset & operator = (Asset const & obj) {
        return this->copy(obj);
    }

    Asset & operator = (Asset && obj) {
        this->swap(obj);
        return *this;
    }

public:
    Asset & copy(Asset const & obj) {
        if (this != &obj) {
            this->dirs = obj.dirs;
        }
        return *this;
    }

    void swap(Asset & obj) {
        if (this != &obj) {
            this->dirs.swap(obj.dirs);
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
    std::string getDir(std::string const & key) {
        auto itr = this->dirs.find(key);
        if (itr == this->dirs.end()) {
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
    void insertDir(std::string const & key, std::string const & value) {
        this->dirs.insert(std::make_pair(key, value));
    }

    std::size_t size() const noexcept(true) {
        return this->dirs.size();
    }

// Static methods.
public:
    /** Obtain HOME directory key name. */
    static std::string getHomeDirKeyName() {
        return std::string(HOME_DIRECTORY_ASSET_NAME);
    }

    /** Obtain executable file directory key name. */
    static std::string getExeDirKeyName() {
        return std::string(EXE_DIRECTORY_ASSET_NAME);
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

