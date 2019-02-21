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

#ifndef _self_sf
#define _self_sf() Pointer::cast<sf::Image>()
#endif

using Sizeu = Image::Sizeu;
using Rgb32 = Image::Rgb32;

Image::Image() : SfNative(SfType::ST_IMAGE)
{
    assert(ptr != nullptr);
}

Image::Image(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_IMAGE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
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

void Image::create(unsigned int width, unsigned int height, Rgb32 const & color)
{
    _self_sf()->create(width, height, sf::Color(color.r, color.g, color.b, color.a));
}

void Image::create(unsigned int width, unsigned int height, std::uint8_t const * pixels)
{
    _self_sf()->create(width, height, pixels);
}

bool Image::loadFromFile(std::string const & filename)
{
    return _self_sf()->loadFromFile(filename);
}

bool Image::loadFromMemory(void const * data, std::size_t size)
{
    return _self_sf()->loadFromMemory(data, size);
}

bool Image::saveToFile(std::string const & filename) const
{
    return _self_sf()->saveToFile(filename);
}

Sizeu Image::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizeu(SIZE.x, SIZE.y);
}

void Image::createMaskFromColor(Rgb32 const & color, std::uint8_t alpha)
{
    _self_sf()->createMaskFromColor(sf::Color(color.r, color.g, color.b, color.a), alpha);
}

void Image::copy(Image const & source, unsigned int dest_x, unsigned int dest_y,
                 Recti const & source_rect, bool apply_alpha)
{
    _self_sf()->copy(*source.cast<sf::Image>(), dest_x, dest_y,
                     sf::IntRect(source_rect.x, source_rect.y, source_rect.width, source_rect.height),
                     apply_alpha);
}

void Image::setPixel(unsigned int x, unsigned int y, Rgb32 const & color)
{
    _self_sf()->setPixel(x, y, sf::Color(color.r, color.g, color.b, color.a));
}

Rgb32 Image::getPixel(unsigned int x, unsigned int y) const
{
    auto const COLOR = _self_sf()->getPixel(x, y);
    return Rgb32(COLOR.r, COLOR.g, COLOR.b, COLOR.a);
}

std::uint8_t const * Image::getPixelsPtr() const
{
    return _self_sf()->getPixelsPtr();
}

void Image::flipHorizontally()
{
    _self_sf()->flipHorizontally();
}

void Image::flipVertically()
{
    _self_sf()->flipVertically();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

