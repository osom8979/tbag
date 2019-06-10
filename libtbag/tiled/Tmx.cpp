/**
 * @file   Tmx.cpp
 * @brief  Tmx class implementation.
 * @author zer0
 * @date   2019-05-29
 */

#include <libtbag/tiled/Tmx.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/util/Version.hpp>

#include <cassert>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

using XmlHelper = libtbag::dom::xml::XmlHelper;

Tmx::Tmx() : _map(std::make_shared<libtbag::tiled::Map>())
{
    // EMPTY.
}

Tmx::Tmx(std::nullptr_t) : _map(nullptr)
{
    // EMPTY.
}

Tmx::Tmx(Tmx const & obj) : Tmx()
{
    (*this) = obj;
}

Tmx::Tmx(Tmx && obj) TBAG_NOEXCEPT : Tmx(nullptr)
{
    (*this) = std::move(obj);
}

Tmx::~Tmx()
{
    // EMPTY.
}

Tmx & Tmx::operator =(Tmx const & obj)
{
    copy(obj);
    return *this;
}

Tmx & Tmx::operator =(Tmx && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Tmx::copy(Tmx const & obj)
{
    if (this != &obj) {
        _map = obj._map;
    }
}

void Tmx::swap(Tmx & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _map.swap(obj._map);
    }
}

void Tmx::reset()
{
    _map.reset();
}

Err Tmx::loadFromFile(std::string const & path)
{
    std::string xml;
    auto const CODE = libtbag::filesystem::readFile(path, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    return loadFromXmlString(xml);
}

Err Tmx::loadFromXmlString(std::string const & xml)
{
    Document doc;
    if (doc.Parse(xml.c_str()) != tinyxml2::XML_NO_ERROR) {
        return E_PARSING;
    }
    return loadFromXmlDocument(doc);
}

Err Tmx::loadFromXmlDocument(Document const & doc)
{
    auto const * elem = doc.FirstChildElement(Map::TAG_NAME);
    if (elem == nullptr) {
        return E_ENFOUND;
    }
    return loadFromMapElement(*elem);
}

Err Tmx::loadFromMapElement(Element const & elem)
{
    if (elem.Name() != Map::TAG_NAME) {
        return E_ILLARGS;
    }
    auto map = std::make_shared<libtbag::tiled::Map>();
    assert(static_cast<bool>(map));
    auto code = readElement(elem, *map);
    if (isSuccess(code)) {
        _map.swap(map);
    }
    return code;
}

Err Tmx::readElement(Element const & elem, Map & val)
{
    if (elem.Name() != Map::TAG_NAME) {
        return E_ILLARGS;
    }

    Err code;
    code = XmlHelper::optAttr(elem, Map::ATT_VERSION, val.version); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }

    std::string orientation;
    code = XmlHelper::optAttr(elem, Map::ATT_ORIENTATION, orientation); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }
    if (orientation == Map::VAL_ORTHOGONAL) {
        val.orientation = Map::Orientation::ORTHOGONAL;
    } else if (orientation == Map::VAL_ISOMETRIC) {
        val.orientation = Map::Orientation::ISOMETRIC;
    } else if (orientation == Map::VAL_STAGGERED) {
        val.orientation = Map::Orientation::STAGGERED;
    } else if (orientation == Map::VAL_HEXAGONAL) {
        val.orientation = Map::Orientation::HEXAGONAL;
    } else {
        val.orientation = Map::Orientation::NONE;
        tDLogW("Tmx::readElement() Unknown orientation value: {}", orientation);
    }

    code = XmlHelper::optAttr(elem, Map::ATT_WIDTH, val.width); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }
    code = XmlHelper::optAttr(elem, Map::ATT_HEIGHT, val.height); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }
    code = XmlHelper::optAttr(elem, Map::ATT_TILEWIDTH, val.tile_width); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }
    code = XmlHelper::optAttr(elem, Map::ATT_TILEHEIGHT, val.tile_height); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }

    XmlHelper::optAttr(elem, Map::ATT_TILEDVERSION, val.tiled_version);

    std::string render_order;
    XmlHelper::optAttr(elem, Map::ATT_RENDERORDER, render_order);
    if (render_order == Map::VAL_RIGHT_DOWN) {
        val.render_order = Map::RenderOrder::RIGHT_DOWN;
    } else if (render_order == Map::VAL_RIGHT_UP) {
        val.render_order = Map::RenderOrder::RIGHT_UP;
    } else if (render_order == Map::VAL_LEFT_DOWN) {
        val.render_order = Map::RenderOrder::LEFT_DOWN;
    } else if (render_order == Map::VAL_LEFT_UP) {
        val.render_order = Map::RenderOrder::LEFT_UP;
    } else {
        val.render_order = Map::RenderOrder::NONE;
        tDLogW("Tmx::readElement() Unknown RenderOrder value: {}", render_order);
    }

    XmlHelper::optAttr(elem, Map::ATT_HEXSIDELENGTH, val.hexside_length);

    std::string stagger_axis;
    XmlHelper::optAttr(elem, Map::ATT_STAGGERAXIS, stagger_axis);
    if (stagger_axis == Map::VAL_X) {
        val.stagger_axis = Map::StaggerAxis::X;
    } else if (stagger_axis == Map::VAL_Y) {
        val.stagger_axis = Map::StaggerAxis::Y;
    } else {
        val.stagger_axis = Map::StaggerAxis::NONE;
        tDLogW("Tmx::readElement() Unknown StaggerAxis value: {}", stagger_axis);
    }

    std::string stagger_index;
    XmlHelper::optAttr(elem, Map::ATT_STAGGERINDEX, stagger_index);
    if (stagger_index == Map::VAL_ODD) {
        val.stagger_index = Map::StaggerIndex::ODD;
    } else if (stagger_index == Map::VAL_EVEN) {
        val.stagger_index = Map::StaggerIndex::EVEN;
    } else {
        val.stagger_index = Map::StaggerIndex::NONE;
        tDLogW("Tmx::readElement() Unknown StaggerIndex value: {}", stagger_index);
    }

    XmlHelper::optAttr(elem, Map::ATT_BACKGROUNDCOLOR, val.background_color);
    XmlHelper::optAttr(elem, Map::ATT_NEXTLAYERID, val.next_layer_id);
    XmlHelper::optAttr(elem, Map::ATT_NEXTOBJECTID, val.next_object_id);
    XmlHelper::optAttr(elem, Map::ATT_INFINITE, val.infinite);

    code = readElement(elem, val.properties, true);
    if (isFailure(code)) {
        return code;
    }

    code = readElement(elem, val.tilesets, true);
    if (isFailure(code)) {
        return code;
    }

    return E_SUCCESS;
}

Err Tmx::readElement(Element const & elem, Properties & val, bool clear)
{
    if (elem.Name() != Properties::TAG_NAME) {
        return E_ILLARGS;
    }

    if (clear) {
        val.properties.clear();
    }

    Err code;
    auto const * child_elem = elem.FirstChildElement(Property::TAG_NAME);
    while (child_elem != nullptr) {
        Property property;
        code = readElement(*child_elem, property);
        if (isSuccess(code)) {
            val.properties.push_back(std::move(property));
        } else {
            return code;
        }
        child_elem = child_elem->NextSiblingElement(Property::TAG_NAME);
    }
    return E_SUCCESS;
}

Err Tmx::readElement(Element const & elem, Property & val)
{
    if (elem.Name() != Property::TAG_NAME) {
        return E_ILLARGS;
    }

    Err code;
    code = XmlHelper::optAttr(elem, Property::ATT_NAME, val.name); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }
    code = XmlHelper::optAttr(elem, Property::ATT_VALUE, val.value); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }

    std::string type;
    XmlHelper::optAttr(elem, Property::ATT_TYPE, type);
    if (type == Property::VAL_STRING) {
        val.type = Property::Type::STRING;
    } else if (type == Property::VAL_INT) {
        val.type = Property::Type::INT;
    } else if (type == Property::VAL_FLOAT) {
        val.type = Property::Type::FLOAT;
    } else if (type == Property::VAL_BOOL) {
        val.type = Property::Type::BOOL;
    } else if (type == Property::VAL_COLOR) {
        val.type = Property::Type::COLOR;
    } else if (type == Property::VAL_FILE) {
        val.type = Property::Type::FILE;
    } else {
        val.type = Property::Type::NONE;
        tDLogW("Tmx::readElement() Unknown Type value: {}", type);
    }
    return E_SUCCESS;
}

Err Tmx::readElement(Element const & elem, Tilesets & val, bool clear)
{
    if (elem.Name() != Map::TAG_NAME) {
        return E_ILLARGS;
    }

    if (clear) {
        val.tilesets.clear();
    }

    auto const * child_elem = elem.FirstChildElement(Tileset::TAG_NAME);
    while (child_elem != nullptr) {
        Tileset tileset;
        if (isSuccess(readElement(*child_elem, tileset))) {
            val.tilesets.push_back(std::move(tileset));
        }
        child_elem = child_elem->NextSiblingElement(Tileset::TAG_NAME);
    }
    return E_SUCCESS;
}

Err Tmx::readElement(Element const & elem, Tileset & val)
{
    if (elem.Name() != Tileset::TAG_NAME) {
        return E_ILLARGS;
    }

    Err code;
    code = XmlHelper::optAttr(elem, Tileset::ATT_FIRSTGID, val.first_gid); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }

    code = XmlHelper::optAttr(elem, Tileset::ATT_SOURCE, val.source);
    if (isFailure(code)) {
        // "name" REQUIRED only if "source" tsx not present.
        code = XmlHelper::optAttr(elem, Tileset::ATT_NAME, val.name);
        if (isFailure(code)) {
            return code;
        }
    } else {
        XmlHelper::optAttr(elem, Tileset::ATT_NAME, val.name);
    }

    XmlHelper::optAttr(elem, Tileset::ATT_TILEWIDTH, val.tile_width);
    XmlHelper::optAttr(elem, Tileset::ATT_TILEHEIGHT, val.tile_height);
    XmlHelper::optAttr(elem, Tileset::ATT_SPACING, val.spacing);
    XmlHelper::optAttr(elem, Tileset::ATT_MARGIN, val.margin);
    XmlHelper::optAttr(elem, Tileset::ATT_TILECOUNT, val.tile_count);
    XmlHelper::optAttr(elem, Tileset::ATT_COLUMNS, val.columns);
    return E_SUCCESS;
}

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

