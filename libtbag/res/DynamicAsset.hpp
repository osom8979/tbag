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
    using Value  = char;
    using String = std::basic_string<Value>;
    using Path   = libtbag::filesystem::Path;

    using StringVector = std::vector<String>;
    using PathVector   = std::vector<Path>;
    using PathMap      = std::map<String, Path>;
    using PathMapPair  = PathMap::value_type;

    static_assert(std::is_same<Value, char>::value
            , "Value must be the same type as char");
    static_assert(std::is_same<Value, typename String::value_type>::value
            , "Value must be the same type as String::value_type");

private:
    PathMap _paths; ///< Parent directories.

public:
    DynamicAsset();
    DynamicAsset(DynamicAsset const & obj);
    DynamicAsset(DynamicAsset && obj);
    ~DynamicAsset();

public:
    DynamicAsset & operator =(DynamicAsset const & obj);
    DynamicAsset & operator =(DynamicAsset && obj);

public:
    // @formatter:off
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_paths.empty())
    { return _paths.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_paths.size())
    { return _paths.size(); }
    inline void clear() TBAG_NOEXCEPT_SP_OP(_paths.clear())
    { _paths.clear(); }
    // @formatter:on

public:
    bool init();

public:
    bool add(String const & key, Path const & path);
    Path get(String const & key) const;

public:
    /** Obtain the list of key. */
    StringVector getKeys() const;

    /** Obtain the list of path. */
    PathVector getPaths() const;

public:
    bool exists(String const & key) const;
    bool createDir(String const & key) const;
    bool removeAll(String const & key) const;

public:
    PathVector scan(String const & key) const;
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

inline std::vector<std::string> getDefaultLayout()
{
    std::vector<std::string> result;
    for (std::size_t i = 0; i < DEFAULT_ASSET_LAYOUT_SIZE; ++i) {
        result.emplace_back(DEFAULT_ASSET_LAYOUT[i]);
    }
    return result;
}

TBAG_API DynamicAsset getDynamicAsset(filesystem::Path const & path, std::vector<std::string> const & layouts);
TBAG_API DynamicAsset getDynamicAsset(std::string      const & path, std::vector<std::string> const & layouts);
TBAG_API DynamicAsset getDynamicAsset(filesystem::Path const & path);
TBAG_API DynamicAsset getDynamicAsset(std::string      const & path);

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__

