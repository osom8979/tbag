/**
 * @file   Texture.cpp
 * @brief  Texture class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/Texture.hpp>
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

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::Texture>()
#endif

Texture::Texture() : SfNative(SfType::ST_TEXTURE)
{
    // EMPTY.
}

Texture::Texture(Texture && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_TEXTURE, no_init)
{
    *this = std::move(obj);
}

Texture::~Texture()
{
    // EMPTY.
}

Texture & Texture::operator =(Texture && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Texture::swap(Texture & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

