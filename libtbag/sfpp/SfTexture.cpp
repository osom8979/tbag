/**
 * @file   SfTexture.cpp
 * @brief  SfTexture class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/sfpp/SfTexture.hpp>
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

namespace sfpp {

SfTexture::SfTexture() : SfNative(SfType::ST_TEXTURE)
{
    // EMPTY.
}

SfTexture::SfTexture(SfTexture && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_TEXTURE, no_init)
{
    *this = std::move(obj);
}

SfTexture::~SfTexture()
{
    // EMPTY.
}

SfTexture & SfTexture::operator =(SfTexture && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SfTexture::swap(SfTexture & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::Texture>()
#endif

#undef _self_sf

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

