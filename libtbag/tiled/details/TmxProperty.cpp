/**
 * @file   TmxProperty.cpp
 * @brief  TmxProperty class implementation.
 * @author zer0
 * @date   2019-06-10
 */

#include <libtbag/tiled/details/TmxProperty.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

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

TmxProperty::~TmxProperty()
{
    // EMPTY.
}

void TmxProperty::clear()
{
    name.clear();
    value.clear();
    type = Type::STRING; // (default)
}

Err TmxProperty::read(Element const & elem)
{
    if (elem.Name() != TAG_NAME) {
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
        tDLogW("TmxProperty::read() Unknown Type value: {}", type_text);
    }

    return E_SUCCESS;
}

Err TmxProperty::dump(Element & elem) const
{
    if (elem.Name() != TAG_NAME) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_VALUE, value);
    setAttr(elem, ATT_TYPE, getTypeName(type));
    return E_SUCCESS;
}

Err TmxProperty::dumpToParent(Element & elem) const
{
    auto * new_elem = newElement(elem, TAG_NAME);
    assert(new_elem != nullptr);
    insertElement(elem, new_elem);
    return E_SUCCESS;
}

bool TmxProperty::validate(std::string const & version) const
{
    if (type == Type::NONE) {
        return false;
    }

    using Version = libtbag::util::Version;
    Version const V016(0, 1, 6);
    Version const V017(0, 1, 7);
    Version ver(version);

    if (ver >= V017) {
        return true;
    }
    assert(ver < V017);
    assert(type != Type::NONE);
    if (type == Type::COLOR || type == Type::FILE) {
        return false;
    }

    if (ver >= V016) {
        return true;
    }
    assert(ver < V016);
    assert(type != Type::NONE);
    assert(type != Type::COLOR);
    assert(type != Type::FILE);
    return (type == Type::STRING);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

