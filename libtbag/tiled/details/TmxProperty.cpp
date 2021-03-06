/**
 * @file   TmxProperty.cpp
 * @brief  TmxProperty class implementation.
 * @author zer0
 * @date   2019-06-10
 */

#include <libtbag/tiled/details/TmxProperty.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxProperty::TmxProperty() : name(), value(), type(Type::NONE)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, std::string const & v, Type t)
        : name(n), value(v), type(t)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, std::string const & v)
        : name(n), value(v), type(Type::STRING)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, int v)
        : name(n), value(std::to_string(v)), type(Type::INT)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, float v)
        : name(n), value(std::to_string(v)), type(Type::FLOAT)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, bool v)
        : name(n), value(v ? VAL_TRUE : VAL_FALSE), type(Type::BOOL)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, Rgb24 const & c)
        : TmxProperty(n, libtbag::graphic::convertRgb24ToRgb32(c))
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, Rgb32 const & c)
        : name(n), value(c.toArgbString()), type(Type::COLOR)
{
    // EMPTY.
}

TmxProperty::TmxProperty(std::string const & n, Path const & p)
        : name(n), value(p.toString()), type(Type::FILE)
{
    // EMPTY.
}

TmxProperty::~TmxProperty()
{
    // EMPTY.
}

TmxProperty::Type TmxProperty::getType(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_STRING) {
        return Type::STRING;
    } else if (text == VAL_INT) {
        return Type::INT;
    } else if (text == VAL_FLOAT) {
        return Type::FLOAT;
    } else if (text == VAL_BOOL) {
        return Type::BOOL;
    } else if (text == VAL_COLOR) {
        return Type::COLOR;
    } else if (text == VAL_FILE) {
        return Type::FILE;
    } else {
        return Type::NONE;
    }
}

char const * const TmxProperty::getTypeName(Type t) TBAG_NOEXCEPT
{
    // clang-format off
    switch (t) {
    case Type::STRING:  return VAL_STRING;
    case Type::INT:     return VAL_INT;
    case Type::FLOAT:   return VAL_FLOAT;
    case Type::BOOL:    return VAL_BOOL;
    case Type::COLOR:   return VAL_COLOR;
    case Type::FILE:    return VAL_FILE;
    case Type::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

int TmxProperty::toInt(int def) const
{
    return libtbag::string::toValue<int>(value, def);
}

float TmxProperty::toFloat(float def) const
{
    return libtbag::string::toValue<float>(value, def);
}

bool TmxProperty::toBool(bool def) const
{
    return libtbag::string::toValue<bool>(value, def);
}

TmxProperty::Rgb32 TmxProperty::toColor(Rgb32 const & def) const
{
    Rgb32 result;
    if (isSuccess(libtbag::graphic::convertArgbHexStringToRgb32(value, result))) {
        return result;
    }
    return def;
}

TmxProperty::Path TmxProperty::toFile() const
{
    return Path(value);
}

void TmxProperty::clear()
{
    name.clear();
    value.clear();
    type = Type::STRING; // (default)
}

Err TmxProperty::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    Err code = optAttr(elem, ATT_NAME, name); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }

    code = optAttr(elem, ATT_VALUE, value); // #REQUIRED
    if (isFailure(code)) {
        return code;
    }

    std::string type_text;
    optAttr(elem, ATT_TYPE, type_text, VAL_STRING);
    type = getType(type_text);
    if (type == Type::NONE) {
        // Unknown Type value.
    }

    return E_SUCCESS;
}

Err TmxProperty::read(std::string const & xml)
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

Err TmxProperty::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_VALUE, value);
    setAttr(elem, ATT_TYPE, getTypeName(type));
    return E_SUCCESS;
}

Err TmxProperty::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeDocumentToXmlText(doc, xml);
}

bool TmxProperty::validate(Version const & version) const
{
    if (type == Type::NONE) {
        return false;
    }

    Version const V017(0, 17);
    if (version >= V017) {
        return true;
    }
    assert(version < V017);
    assert(type != Type::NONE);
    if (type == Type::COLOR || type == Type::FILE) {
        return false;
    }

    Version const V016(0, 16);
    if (version >= V016) {
        return true;
    }
    assert(version < V016);
    assert(type != Type::NONE);
    assert(type != Type::COLOR);
    assert(type != Type::FILE);
    return (type == Type::STRING);
}

bool TmxProperty::validate(std::string const & version) const
{
    return validate(Version(version));
}

bool TmxProperty::validate(int major_version, int minor_version) const
{
    return validate(Version(major_version, minor_version));
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

