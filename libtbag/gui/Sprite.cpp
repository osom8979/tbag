/**
 * @file   Sprite.cpp
 * @brief  Sprite class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/Sprite.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

Sprite::Sprite() : Drawable(SfType::ST_SPRITE)
{
    assert(ptr != nullptr);
}

Sprite::Sprite(Sprite && obj) TBAG_NOEXCEPT
        : Drawable(SfType::ST_SPRITE, no_init_no_ref)
{
    *this = std::move(obj);
}

Sprite::~Sprite()
{
    // EMPTY.
}

Sprite & Sprite::operator =(Sprite && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Sprite::swap(Sprite & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

