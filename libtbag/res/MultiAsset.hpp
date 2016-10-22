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
 *  - Directory structure:
 *  @code
 *   + path1
 *     - layout1
 *     - layout2
 *     - layout3
 *   + path2
 *     - layout1
 *     - layout2
 *     - layout3
 *   + path3
 *     - layout1
 *     - layout2
 *     - layout3
 *  @endcode
 *  - Data structure:
 *  @code
 *  MultiAsset{
 *     layout1: DynamicAsset{0:path1/layout1, 1:path2/layout1, 2:path3/layout1}
 *    ,layout2: DynamicAsset{0:path1/layout2, 1:path2/layout2, 2:path3/layout2}
 *    ,layout3: DynamicAsset{0:path1/layout3, 1:path2/layout3, 2:path3/layout3}
 *  }
 *  @endcode
 */
class TBAG_EXPORTS MultiAsset
{
public:
    using DynamicAsset = libtbag::res::DynamicAsset;

    using Value  = typename DynamicAsset::Value;
    using String = typename DynamicAsset::String;
    using Path   = typename DynamicAsset::Path;

    using StringVector = typename DynamicAsset::StringVector;
    using PathVector   = typename DynamicAsset::PathVector;
    using PathMap      = typename DynamicAsset::PathMap;
    using PathMapPair  = typename DynamicAsset::PathMapPair;

    using AssetMap     = std::map<String, DynamicAsset>;
    using AssetMapPair = typename AssetMap::value_type;

    static_assert(std::is_same<Value, char>::value
            , "Value must be the same type as char");
    static_assert(std::is_same<Value, typename String::value_type>::value
            , "Value must be the same type as String::value_type");

    static Value const * const LAYOUT_NAMES[];
    static std::size_t const LAYOUT_NAMES_SIZE;

private:
    AssetMap _assets;

public:
    MultiAsset() = default;
    ~MultiAsset() = default;

    MultiAsset(MultiAsset const & obj) = default;
    MultiAsset(MultiAsset && obj) = default;

    MultiAsset & operator =(MultiAsset const & obj) = default;
    MultiAsset & operator =(MultiAsset && obj) = default;

public:
    inline bool empty() const noexcept(noexcept(_assets.empty()))
    { return _assets.empty(); }
    inline std::size_t size() const noexcept(noexcept(_assets.size()))
    { return _assets.size(); }
    inline void clear() noexcept(noexcept(_assets.clear()))
    { _assets.clear(); }

public:
    bool add(String const & name, DynamicAsset const & asset);
    bool add(String && name, DynamicAsset && asset);
    bool add(String const & name, PathVector const & paths);

public:
    DynamicAsset getAsset(String const & name) const;
    StringVector getNames() const;

public:
    Path findWriteableDir(String const & name) const;
    Path findFile(String const & name, String const & filename) const;

public:
    static MultiAsset create(PathVector const & paths, StringVector const & layouts);
    static MultiAsset createDefault();
};


} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_MULTIASSET_HPP__

