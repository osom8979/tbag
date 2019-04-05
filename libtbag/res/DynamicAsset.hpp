/**
 * @file   DynamicAsset.hpp
 * @brief  DynamicAsset class prototype.
 * @author zer0
 * @date   2016-10-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <string>
#include <vector>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * DynamicAsset class prototype.
 *
 * @author zer0
 * @date   2016-10-13
 */
class TBAG_API DynamicAsset
{
public:
    using Path    = libtbag::filesystem::Path;
    using Strings = std::vector<std::string>;
    using Paths   = std::vector<Path>;
    using PathMap = std::map<std::string, Path>;

private:
    PathMap _paths; ///< Layout & Directories.

public:
    DynamicAsset();
    DynamicAsset(DynamicAsset const & obj);
    DynamicAsset(DynamicAsset && obj) TBAG_NOEXCEPT;
    ~DynamicAsset();

public:
    DynamicAsset & operator =(DynamicAsset const & obj);
    DynamicAsset & operator =(DynamicAsset && obj) TBAG_NOEXCEPT;

public:
    void copy(DynamicAsset const & obj);
    void swap(DynamicAsset & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(DynamicAsset & lh, DynamicAsset & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline PathMap       & paths()       TBAG_NOEXCEPT { return _paths; }
    inline PathMap const & paths() const TBAG_NOEXCEPT { return _paths; }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_paths.empty())
    { return _paths.empty(); }

    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_paths.size())
    { return _paths.size(); }

    inline void clear() TBAG_NOEXCEPT_SP_OP(_paths.clear())
    { _paths.clear(); }

public:
    std::size_t createLayouts();

public:
    bool add(std::string const & key, Path const & path);
    void set(std::string const & key, Path const & path);
    Path get(std::string const & key) const;

public:
    /** Obtain the list of key. */
    Strings getKeys() const;

    /** Obtain the list of path. */
    Paths getPaths() const;

public:
    bool exists(std::string const & key) const;
    bool createDir(std::string const & key) const;
    bool removeAll(std::string const & key) const;

public:
    Paths scan(std::string const & key) const;
};

// ------------------------
// Miscellaneous utilities.
// ------------------------

#define TBAG_DYNAMIC_ASSERT_LAYOUT_BIN      "bin"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_DATA     "data"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_DB       "db"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_DOM      "dom"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_CONFIG   "config"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_IMAGE    "image"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_LOG      "log"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_MAP      "map"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_PLUGIN   "plugin"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_SAVE     "save"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_SCRIPT   "script"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_SPRITE   "sprite"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_STRING   "string"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_TEMP     "temp"
#define TBAG_DYNAMIC_ASSERT_LAYOUT_VALUE    "value"

TBAG_CONSTEXPR char const * const DEFAULT_ASSET_LAYOUT[] = {
        TBAG_DYNAMIC_ASSERT_LAYOUT_BIN   ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_DATA  ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_DB    ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_DOM   ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_CONFIG,
        TBAG_DYNAMIC_ASSERT_LAYOUT_IMAGE ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_LOG   ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_MAP   ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_PLUGIN,
        TBAG_DYNAMIC_ASSERT_LAYOUT_SAVE  ,
        TBAG_DYNAMIC_ASSERT_LAYOUT_SCRIPT,
        TBAG_DYNAMIC_ASSERT_LAYOUT_SPRITE,
        TBAG_DYNAMIC_ASSERT_LAYOUT_STRING,
        TBAG_DYNAMIC_ASSERT_LAYOUT_TEMP  ,
};
TBAG_CONSTEXPR std::size_t const DEFAULT_ASSET_LAYOUT_SIZE =
        sizeof(DEFAULT_ASSET_LAYOUT) / sizeof(DEFAULT_ASSET_LAYOUT[0]);

TBAG_API std::vector<std::string> getDefaultLayout();

// clang-format off
TBAG_API DynamicAsset getDynamicAsset(filesystem::Path const & path, std::vector<std::string> const & layouts);
TBAG_API DynamicAsset getDynamicAsset(std::string      const & path, std::vector<std::string> const & layouts);
TBAG_API DynamicAsset getDynamicAsset(filesystem::Path const & path);
TBAG_API DynamicAsset getDynamicAsset(std::string      const & path);
// clang-format on

/**
 * Default dynamic asset.
 *
 * @remarks
 *  @code
 *   ${EXE_DIR}/${EXE_NAME}/${LAYOUTS...}
 *  @endcode
 */
TBAG_API DynamicAsset getDynamicAsset();

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__

