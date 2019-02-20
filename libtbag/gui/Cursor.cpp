/**
 * @file   Cursor.cpp
 * @brief  Cursor class implementation.
 * @author zer0
 * @date   2019-02-17
 */

#include <libtbag/gui/Cursor.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#ifndef _STATIC_ASSERT_EQUAL
#define _STATIC_ASSERT_EQUAL(x, y) STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
_STATIC_ASSERT_EQUAL(sf::Cursor::Arrow,      libtbag::gui::Cursor::CursorType::CT_ARROW);
_STATIC_ASSERT_EQUAL(sf::Cursor::NotAllowed, libtbag::gui::Cursor::CursorType::CT_NOT_ALLOWED);
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

#ifndef _self_sf
#define _self_sf() Pointer::cast<sf::Cursor>()
#endif

Cursor::Cursor() : SfNative(SfType::ST_CURSOR)
{
    assert(ptr != nullptr);
}

Cursor::Cursor(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_CURSOR, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

Cursor::Cursor(Cursor && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_CURSOR, no_init)
{
    *this = std::move(obj);
}

Cursor::~Cursor()
{
    // EMPTY.
}

Cursor & Cursor::operator =(Cursor && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Cursor::swap(Cursor & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

bool Cursor::loadFromPixels(std::uint8_t const * pixels, Sizeu const & size, Pointu const & hot_spot)
{
    return _self_sf()->loadFromPixels(pixels,
                                      sf::Vector2u(size.width, size.height),
                                      sf::Vector2u(hot_spot.x, hot_spot.y));
}

bool Cursor::loadFromSystem(CursorType type)
{
    return _self_sf()->loadFromSystem((sf::Cursor::Type)type);
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

