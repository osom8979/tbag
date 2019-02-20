/**
 * @file   Image.cpp
 * @brief  Image class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/Image.hpp>
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

Image::Image() : SfNative(SfType::ST_IMAGE)
{
    // EMPTY.
}

Image::Image(Image && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_IMAGE, no_init)
{
    *this = std::move(obj);
}

Image::~Image()
{
    // EMPTY.
}

Image & Image::operator =(Image && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Image::swap(Image & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::Image>()
#endif

#undef _self_sf

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

