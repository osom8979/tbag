/**
 * @file   SfCursor.cpp
 * @brief  SfCursor class implementation.
 * @author zer0
 * @date   2019-02-17
 */

#include <libtbag/gui/SfCursor.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#ifndef _STATIC_ASSERT_EQUAL
#define _STATIC_ASSERT_EQUAL(x, y) STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
_STATIC_ASSERT_EQUAL(sf::Cursor::Arrow,      libtbag::gui::SfCursor::CursorType::CT_ARROW);
_STATIC_ASSERT_EQUAL(sf::Cursor::NotAllowed, libtbag::gui::SfCursor::CursorType::CT_NOT_ALLOWED);
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfCursor::SfCursor() : SfNative(SfType::ST_CURSOR)
{
    // EMPTY.
}

SfCursor::~SfCursor()
{
    // EMPTY.
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::Cursor>()
#endif

bool SfCursor::loadFromPixels(std::uint8_t const * pixels, Sizeu const & size, Pointu const & hot_spot)
{
    return _self_sf()->loadFromPixels(pixels,
                                      sf::Vector2u(size.width, size.height),
                                      sf::Vector2u(hot_spot.x, hot_spot.y));
}

bool SfCursor::loadFromSystem(CursorType type)
{
    return _self_sf()->loadFromSystem((sf::Cursor::Type)type);
}

#undef _self_sf

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

