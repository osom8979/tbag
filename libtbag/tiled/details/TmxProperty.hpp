/**
 * @file   TmxProperty.hpp
 * @brief  TmxProperty class prototype.
 * @author zer0
 * @date   2019-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxProperty class prototype.
 *
 * <property> element.
 *
 * DTD:
 * @code{.dtd}
 *  <!ELEMENT property EMPTY>
 * @endcode
 *
 * Example:
 * @code{.xml}
 *  <properties>
 *   <property name="enemyTint" type="color" value="#ffa33636"/>
 *  </properties>
 * @endcode
 *
 * @author zer0
 * @date   2019-06-10
 */
struct TBAG_API TmxProperty : public libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "property";

    /** The name of the property. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /**
     * The type of the property.
     * Can be string (default), int, float, bool, color or file
     * (since 0.16, with color and file added in 0.17).
     */
    TBAG_CONSTEXPR static char const * const ATT_TYPE   = "type";
    TBAG_CONSTEXPR static char const * const VAL_STRING = "string";
    TBAG_CONSTEXPR static char const * const VAL_INT    = "int";
    TBAG_CONSTEXPR static char const * const VAL_FLOAT  = "float";
    TBAG_CONSTEXPR static char const * const VAL_BOOL   = "bool";
    TBAG_CONSTEXPR static char const * const VAL_COLOR  = "color";
    TBAG_CONSTEXPR static char const * const VAL_FILE   = "file";

    /** The value of the property. */
    TBAG_CONSTEXPR static char const * const ATT_VALUE = "value";

    std::string name;
    std::string value;

    enum class Type
    {
        NONE, STRING, INT, FLOAT, BOOL, COLOR, FILE
    };

    Type type;

    inline Type getType(std::string const & text) const TBAG_NOEXCEPT
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

    inline char const * const getTypeName(Type t) const TBAG_NOEXCEPT
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

    void clear();
    Err read(Element const & elem);
    Err dump(Element & elem) const;
    Err dumpToParent(Element & elem) const;

    bool validate(std::string const & version) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTY_HPP__

