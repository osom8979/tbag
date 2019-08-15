/**
 * @file   TmxTemplate.hpp
 * @brief  TmxTemplate class prototype.
 * @author zer0
 * @date   2019-08-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTEMPLATE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTEMPLATE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxTileSet.hpp>
#include <libtbag/tiled/details/TmxObject.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTemplate class prototype.
 *
 * @author zer0
 * @date   2019-08-15
 *
 * @remarks
 *  The template root element contains the saved map object and
 *  a tileset element that points to an external tileset,
 *  if the object is a tile object.
 */
struct TBAG_API TmxTemplate : protected libtbag::dom::xml::XmlHelper
{
    using TileSets = std::vector<TmxTileSet>;
    using Objects = std::vector<TmxObject>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "template";

    TileSets tile_sets;
    Objects objects;

    TmxTemplate();
    ~TmxTemplate();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTEMPLATE_HPP__

