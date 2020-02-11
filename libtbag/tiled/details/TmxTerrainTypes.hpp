/**
 * @file   TmxTerrainTypes.hpp
 * @brief  TmxTerrainTypes class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAINTYPES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAINTYPES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxTerrain.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTerrainTypes class prototype.
 *
 * @author zer0
 * @date   2019-07-23
 */
struct TBAG_API TmxTerrainTypes : protected libtbag::dom::xml::XmlHelper
{
    using Terrains = std::vector<TmxTerrain>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "terraintypes";

    Terrains terrains;

    TmxTerrainTypes();
    TmxTerrainTypes(Terrains const & t);
    ~TmxTerrainTypes();

    using       iterator = typename Terrains::iterator;
    using const_iterator = typename Terrains::const_iterator;
    using       reverse_iterator = typename Terrains::reverse_iterator;
    using const_reverse_iterator = typename Terrains::const_reverse_iterator;

    iterator begin() TBAG_NOEXCEPT_SP_OP(terrains.begin())
    { return terrains.begin(); }
    iterator end() TBAG_NOEXCEPT_SP_OP(terrains.end())
    { return terrains.end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(terrains.begin())
    { return terrains.begin(); }
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(terrains.end())
    { return terrains.end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(terrains.cbegin())
    { return terrains.cbegin(); }
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(terrains.cend())
    { return terrains.cend(); }

    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(terrains.rbegin())
    { return terrains.rbegin(); }
    reverse_iterator rend() TBAG_NOEXCEPT_SP_OP(terrains.rend())
    { return terrains.rend(); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(terrains.rbegin())
    { return terrains.rbegin(); }
    const_reverse_iterator rend() const TBAG_NOEXCEPT_SP_OP(terrains.rend())
    { return terrains.rend(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(terrains.crbegin())
    { return terrains.crbegin(); }
    const_reverse_iterator crend() const TBAG_NOEXCEPT_SP_OP(terrains.crend())
    { return terrains.crend(); }

    bool empty() const;
    std::size_t size() const;
    void clear();

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAINTYPES_HPP__

