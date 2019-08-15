/**
 * @file   TmxMap.cpp
 * @brief  TmxMap class implementation.
 * @author zer0
 * @date   2019-08-15
 */

#include <libtbag/tiled/details/TmxMap.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxMap::TmxMap()
{
    // EMPTY.
}

TmxMap::~TmxMap()
{
    // EMPTY.
}

TmxMap::Orientation TmxMap::getOrientation(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_ORTHOGONAL) {
        return Orientation::ORTHOGONAL;
    } else if (text == VAL_ISOMETRIC) {
        return Orientation::ISOMETRIC;
    } else if (text == VAL_STAGGERED) {
        return Orientation::STAGGERED;
    } else if (text == VAL_HEXAGONAL) {
        return Orientation::HEXAGONAL;
    } else {
        return Orientation::NONE;
    }
}

char const * const TmxMap::getOrientationName(Orientation o) TBAG_NOEXCEPT
{
    // clang-format off
    switch (o) {
    case Orientation::ORTHOGONAL:  return VAL_ORTHOGONAL;
    case Orientation::ISOMETRIC:   return VAL_ISOMETRIC;
    case Orientation::STAGGERED:   return VAL_STAGGERED;
    case Orientation::HEXAGONAL:   return VAL_HEXAGONAL;
    case Orientation::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

TmxMap::RenderOrder TmxMap::getRenderOrder(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_RIGHT_DOWN) {
        return RenderOrder::RIGHT_DOWN;
    } else if (text == VAL_RIGHT_UP) {
        return RenderOrder::RIGHT_UP;
    } else if (text == VAL_LEFT_DOWN) {
        return RenderOrder::LEFT_DOWN;
    } else if (text == VAL_LEFT_UP) {
        return RenderOrder::LEFT_UP;
    } else {
        return RenderOrder::NONE;
    }
}

char const * const TmxMap::getRenderOrderName(RenderOrder o) TBAG_NOEXCEPT
{
    // clang-format off
    switch (o) {
    case RenderOrder::RIGHT_DOWN: return VAL_RIGHT_DOWN;
    case RenderOrder::RIGHT_UP:   return VAL_RIGHT_UP  ;
    case RenderOrder::LEFT_DOWN:  return VAL_LEFT_DOWN ;
    case RenderOrder::LEFT_UP:    return VAL_LEFT_UP   ;
    case RenderOrder::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

TmxMap::StaggerAxis TmxMap::getStaggerAxis(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_X) {
        return StaggerAxis::X;
    } else if (text == VAL_Y) {
        return StaggerAxis::Y;
    } else {
        return StaggerAxis::NONE;
    }
}

char const * const TmxMap::getStaggerAxisName(StaggerAxis a) TBAG_NOEXCEPT
{
    // clang-format off
    switch (a) {
    case StaggerAxis::X: return VAL_X;
    case StaggerAxis::Y: return VAL_Y;
    case StaggerAxis::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

TmxMap::StaggerIndex TmxMap::getStaggerIndex(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_ODD) {
        return StaggerIndex::ODD;
    } else if (text == VAL_EVEN) {
        return StaggerIndex::EVEN;
    } else {
        return StaggerIndex::NONE;
    }
}

char const * const TmxMap::getStaggerIndexName(StaggerIndex i) TBAG_NOEXCEPT
{
    // clang-format off
    switch (i) {
    case StaggerIndex::ODD:  return VAL_ODD;
    case StaggerIndex::EVEN: return VAL_EVEN;
    case StaggerIndex::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

Err TmxMap::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_VERSION, version);
    optAttr(elem, ATT_TILEDVERSION, tiled_version);

    std::string orientation_text;
    optAttr(elem, ATT_ORIENTATION, orientation_text);
    orientation = getOrientation(orientation_text);

    std::string render_order_text;
    optAttr(elem, ATT_RENDERORDER, render_order_text);
    render_order = getRenderOrder(render_order_text);

    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);
    optAttr(elem, ATT_TILEWIDTH, tile_width);
    optAttr(elem, ATT_TILEHEIGHT, tile_height);
    optAttr(elem, ATT_HEXSIDELENGTH, hexside_length);

    std::string stagger_axis_text;
    optAttr(elem, ATT_STAGGERAXIS, stagger_axis_text);
    stagger_axis = getStaggerAxis(stagger_axis_text);

    std::string stagger_index_text;
    optAttr(elem, ATT_STAGGERINDEX, stagger_index_text);
    stagger_index = getStaggerIndex(stagger_index_text);

    std::string background_color_text;
    optAttr(elem, ATT_BACKGROUNDCOLOR, background_color_text);
    background_color.fromArgbString(background_color_text);

    optAttr(elem, ATT_NEXTLAYERID, next_layer_id);
    optAttr(elem, ATT_NEXTOBJECTID, next_object_id);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }

    foreachElement(elem, TmxTileSet::TAG_NAME, [&](Element const & e){
        TmxTileSet tile_set;
        if (isSuccess(tile_set.read(e))) {
            tile_sets.push_back(std::move(tile_set));
        }
    });

    foreachElement(elem, TmxLayer::TAG_NAME, [&](Element const & e){
        TmxLayer layer;
        if (isSuccess(layer.read(e))) {
            layers.push_back(std::move(layer));
        }
    });

    foreachElement(elem, TmxObjectGroup::TAG_NAME, [&](Element const & e){
        TmxObjectGroup object_group;
        if (isSuccess(object_group.read(e))) {
            object_groups.push_back(std::move(object_group));
        }
    });

    foreachElement(elem, TmxImageLayer::TAG_NAME, [&](Element const & e){
        TmxImageLayer image_layer;
        if (isSuccess(image_layer.read(e))) {
            image_layers.push_back(std::move(image_layer));
        }
    });

    foreachElement(elem, TmxGroup::TAG_NAME, [&](Element const & e){
        TmxGroup group;
        if (isSuccess(group.read(e))) {
            groups.push_back(std::move(group));
        }
    });

    return E_SUCCESS;
}

Err TmxMap::read(std::string const & xml)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem);
}

Err TmxMap::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_VERSION        , version);
    setAttr(elem, ATT_TILEDVERSION   , tiled_version);
    setAttr(elem, ATT_ORIENTATION    , getOrientationName(orientation));
    setAttr(elem, ATT_RENDERORDER    , getRenderOrderName(render_order));
    setAttr(elem, ATT_WIDTH          , width);
    setAttr(elem, ATT_HEIGHT         , height);
    setAttr(elem, ATT_TILEWIDTH      , tile_width);
    setAttr(elem, ATT_TILEHEIGHT     , tile_height);
    setAttr(elem, ATT_HEXSIDELENGTH  , hexside_length);
    setAttr(elem, ATT_STAGGERAXIS    , getStaggerAxisName(stagger_axis));
    setAttr(elem, ATT_STAGGERINDEX   , getStaggerIndexName(stagger_index));
    setAttr(elem, ATT_BACKGROUNDCOLOR, background_color.toArgbString());
    setAttr(elem, ATT_NEXTLAYERID    , next_layer_id);
    setAttr(elem, ATT_NEXTOBJECTID   , next_object_id);

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }

    for (auto & tile_set : tile_sets) {
        newElement(elem, TmxTileSet::TAG_NAME, [&](Element & p){
            tile_set.write(p);
        });
    }

    for (auto & layer : layers) {
        newElement(elem, TmxLayer::TAG_NAME, [&](Element & p){
            layer.write(p);
        });
    }

    for (auto & object_group : object_groups) {
        newElement(elem, TmxObjectGroup::TAG_NAME, [&](Element & p){
            object_group.write(p);
        });
    }

    for (auto & image_layer : image_layers) {
        newElement(elem, TmxImageLayer::TAG_NAME, [&](Element & p){
            image_layer.write(p);
        });
    }

    for (auto & group : groups) {
        newElement(elem, TmxGroup::TAG_NAME, [&](Element & p){
            group.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxMap::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

