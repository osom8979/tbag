/**
 * @file   MultiAsset.hpp
 * @brief  MultiAsset class prototype.
 * @author zer0
 * @date   2016-10-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_MULTIASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_MULTIASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/res/DynamicAsset.hpp>

#include <string>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * MultiAsset class prototype.
 *
 * @author zer0
 * @date   2016-10-14
 *
 * @remarks
 *  Map of @c DynamicAsset classes.
 */
class TBAG_API MultiAsset
{
public:
    using DynamicAsset = libtbag::res::DynamicAsset;

    using Value  = DynamicAsset::Value;
    using String = DynamicAsset::String;
    using Path   = DynamicAsset::Path;

    using StringVector = DynamicAsset::StringVector;
    using PathVector   = DynamicAsset::PathVector;
    using PathMap      = DynamicAsset::PathMap;
    using PathMapPair  = DynamicAsset::PathMapPair;

    using AssetMap     = std::map<String, DynamicAsset>;
    using AssetMapPair = AssetMap::value_type;

    static_assert(std::is_same<Value, char>::value
            , "Value must be the same type as char");
    static_assert(std::is_same<Value, typename String::value_type>::value
            , "Value must be the same type as String::value_type");

private:
    AssetMap _assets;

public:
    MultiAsset();
    MultiAsset(MultiAsset const & obj);
    MultiAsset(MultiAsset && obj);
    ~MultiAsset();

public:
    MultiAsset & operator =(MultiAsset const & obj);
    MultiAsset & operator =(MultiAsset && obj);

public:
    // @formatter:off
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_assets.empty()))
    { return _assets.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_assets.size()))
    { return _assets.size(); }
    inline void clear() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_assets.clear()))
    { _assets.clear(); }
    // @formatter:on

public:
    bool add(String const & key, DynamicAsset const & asset);
    DynamicAsset at(String const & key);

public:
    /** Obtain the list of key. */
    StringVector getKeys() const;

public:
    /** Initialize all assets. */
    bool init();

public:
    static MultiAsset create(PathVector const & paths, StringVector const & layouts);
};

TBAG_CONSTEXPR char const * const DEFAULT_ASSET_LAYOUT[] = {
        "assets",
        "db"    ,
        "dom"   ,
        "config",
        "image" ,
        "log"   ,
        "map"   ,
        "plugin",
        "save"  ,
        "script",
        "sprite",
        "string",
        "temp"  ,
};
TBAG_CONSTEXPR std::size_t const DEFAULT_ASSET_LAYOUT_SIZE =
        sizeof(DEFAULT_ASSET_LAYOUT) / sizeof(DEFAULT_ASSET_LAYOUT[0]);

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_MULTIASSET_HPP__

