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
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * 'property' element.
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
struct TBAG_API TmxProperty : protected libtbag::dom::xml::XmlHelper
{
    using Rgb24 = libtbag::graphic::Rgb24;
    using Rgb32 = libtbag::graphic::Rgb32;
    using Path  = libtbag::filesystem::Path;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "property";

    /** The name of the property. */
    TBAG_CONSTEXPR static char const * const ATT_NAME  = "name";
    TBAG_CONSTEXPR static char const * const VAL_TRUE  = "true";
    TBAG_CONSTEXPR static char const * const VAL_FALSE = "false";

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

    TmxProperty();
    TmxProperty(std::string const & n, std::string const & v, Type t);
    TmxProperty(std::string const & n, std::string const & v);
    TmxProperty(std::string const & n, int v);
    TmxProperty(std::string const & n, float v);
    TmxProperty(std::string const & n, bool v);
    TmxProperty(std::string const & n, Rgb24 const & c);
    TmxProperty(std::string const & n, Rgb32 const & c);
    TmxProperty(std::string const & n, Path const & p);
    ~TmxProperty();

    static Type getType(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getTypeName(Type t) TBAG_NOEXCEPT;

    int   toInt(int def = 0) const;
    float toFloat(float def = 0.0f) const;
    bool  toBool(bool def = false) const;
    Rgb32 toColor(Rgb32 const & def = libtbag::graphic::BLACK_COLOR) const;
    Path  toFile() const;

    void clear();

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml);

    bool validate(std::string const & version) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTY_HPP__

