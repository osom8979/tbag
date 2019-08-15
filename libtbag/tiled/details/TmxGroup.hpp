/**
 * @file   TmxGroup.hpp
 * @brief  TmxGroup class prototype.
 * @author zer0
 * @date   2019-08-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXGROUP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXGROUP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/tiled/details/TmxLayer.hpp>
#include <libtbag/tiled/details/TmxObjectGroup.hpp>
#include <libtbag/tiled/details/TmxImageLayer.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxGroup class prototype.
 *
 * @author zer0
 * @date   2019-08-15
 *
 * @remarks
 *  A group layer, used to organize the layers of the map in a hierarchy.
 *  Its attributes <code>offsetx</code>, <code>offsety</code>,
 *  <code>opacity</code> and <code>visible</code> recursively affect child layers.
 */
struct TBAG_API TmxGroup : protected libtbag::dom::xml::XmlHelper
{
    using Layers = std::vector<TmxLayer>;
    using ObjectGroups = std::vector<TmxObjectGroup>;
    using ImageLayers = std::vector<TmxImageLayer>;
    using Groups = std::vector<TmxGroup>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "group";

    /**
     * Unique ID of the layer.
     * Each layer that added to a map gets a unique id.
     * Even if a layer is deleted, no layer ever gets the same ID.
     * Can not be changed in Tiled. (since Tiled 1.2)
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the group layer. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** Rendering offset of the group layer in pixels. Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";

    /** Rendering offset of the group layer in pixels. Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";

    /** The opacity of the layer as a value from 0 to 1. Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";

    /** Whether the layer is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";

    int id;
    std::string name;
    int offsetx = 0;
    int offsety = 0;
    float opacity = 1.0;
    int visible = 1;

    TmxProperties properties;
    Layers layers;
    ObjectGroups object_groups;
    ImageLayers image_layers;
    Groups groups;

    TmxGroup();
    ~TmxGroup();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXGROUP_HPP__

