/**
 * @file   PrimitiveType.cpp
 * @brief  PrimitiveType class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/PrimitiveType.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
STATIC_ASSERT_INTEGER_EQUAL(sf::Points       , libtbag::gui::PrimitiveType::PT_POINTS);
STATIC_ASSERT_INTEGER_EQUAL(sf::Lines        , libtbag::gui::PrimitiveType::PT_LINES);
STATIC_ASSERT_INTEGER_EQUAL(sf::LineStrip    , libtbag::gui::PrimitiveType::PT_LINE_STRIP);
STATIC_ASSERT_INTEGER_EQUAL(sf::Triangles    , libtbag::gui::PrimitiveType::PT_TRIANGLES);
STATIC_ASSERT_INTEGER_EQUAL(sf::TriangleStrip, libtbag::gui::PrimitiveType::PT_TRIANGLE_STRIP);
STATIC_ASSERT_INTEGER_EQUAL(sf::TriangleFan  , libtbag::gui::PrimitiveType::PT_TRIANGLE_FAN);
STATIC_ASSERT_INTEGER_EQUAL(sf::Quads        , libtbag::gui::PrimitiveType::PT_QUADS);
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

char const * const getPrimitiveTypeName(PrimitiveType type) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (type) {
    case PrimitiveType::PT_POINTS:          return "POINTS";
    case PrimitiveType::PT_LINES:           return "LINES";
    case PrimitiveType::PT_LINE_STRIP:      return "LINE_STRIP";
    case PrimitiveType::PT_TRIANGLES:       return "TRIANGLES";
    case PrimitiveType::PT_TRIANGLE_STRIP:  return "TRIANGLE_STRIP";
    case PrimitiveType::PT_TRIANGLE_FAN:    return "TRIANGLE_FAN";
    case PrimitiveType::PT_QUADS:           return "QUADS";
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return "UNKNOWN";
    }
    // @formatter:on
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

