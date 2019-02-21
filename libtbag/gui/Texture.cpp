/**
 * @file   Texture.cpp
 * @brief  Texture class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/Texture.hpp>
#include <libtbag/gui/Image.hpp>
#include <libtbag/gui/RenderWindow.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
STATIC_ASSERT_INTEGER_EQUAL(sf::Texture::CoordinateType::Normalized, libtbag::gui::Texture::CoordinateType::CT_NORMALIZED);
STATIC_ASSERT_INTEGER_EQUAL(sf::Texture::CoordinateType::Pixels,     libtbag::gui::Texture::CoordinateType::CT_PIXELS);
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
#define _self_sf() Pointer::cast<sf::Texture>()
#endif

using Sizeu = Texture::Sizeu;

Texture::Texture() : SfNative(SfType::ST_TEXTURE)
{
    assert(ptr != nullptr);
}

Texture::Texture(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_TEXTURE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

Texture::Texture(Texture const & obj)
        : SfNative(SfType::ST_TEXTURE, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Texture(*obj.cast<sf::Texture>());
    assert(ptr != nullptr);
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

Texture & Texture::operator =(Texture const & obj)
{
    *Pointer::cast<sf::Texture>() = *obj.cast<sf::Texture>();
    return *this;
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

bool Texture::create(unsigned int width, unsigned int height)
{
    return _self_sf()->create(width, height);
}

bool Texture::loadFromFile(std::string const & filename, Recti const & area)
{
    return _self_sf()->loadFromFile(filename, sf::IntRect(area.x, area.y, area.width, area.height));
}

bool Texture::loadFromMemory(void const * data, std::size_t size, Recti const & area)
{
    return _self_sf()->loadFromMemory(data, size, sf::IntRect(area.x, area.y, area.width, area.height));
}

bool Texture::loadFromImage(Image const & image, Recti const & area)
{
    return _self_sf()->loadFromImage(*image.cast<sf::Image>(), sf::IntRect(area.x, area.y, area.width, area.height));
}

Sizeu Texture::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizeu(SIZE.x, SIZE.y);
}

Image Texture::copyToImage() const
{
    auto const IMAGE = _self_sf()->copyToImage();
    auto const SIZE = IMAGE.getSize();
    Image result;
    result.create(SIZE.x, SIZE.y, IMAGE.getPixelsPtr());
    return result;
}

void Texture::update(std::uint8_t const * pixels)
{
    _self_sf()->update(pixels);
}

void Texture::update(std::uint8_t const * pixels,
                     unsigned int width, unsigned int height,
                     unsigned int x, unsigned int y)
{
    _self_sf()->update(pixels, width, height, x, y);
}

void Texture::update(Texture const & texture)
{
    _self_sf()->update(*texture.cast<sf::Texture>());
}

void Texture::update(Texture const & texture, unsigned int x, unsigned int y)
{
    _self_sf()->update(*texture.cast<sf::Texture>(), x, y);
}

void Texture::update(Image const & image)
{
    _self_sf()->update(*image.cast<sf::Image>());
}

void Texture::update(Image const & image, unsigned int x, unsigned int y)
{
    _self_sf()->update(*image.cast<sf::Image>(), x, y);
}

void Texture::update(RenderWindow const & window)
{
    _self_sf()->update(*window.cast<sf::RenderWindow>());
}

void Texture::update(RenderWindow const & window, unsigned int x, unsigned int y)
{
    _self_sf()->update(*window.cast<sf::RenderWindow>(), x, y);
}

void Texture::setSmooth(bool smooth)
{
    _self_sf()->setSmooth(smooth);
}

bool Texture::isSmooth() const
{
    return _self_sf()->isSmooth();
}

void Texture::setSrgb(bool srgb)
{
    _self_sf()->setSrgb(srgb);
}

bool Texture::isSrgb() const
{
    return _self_sf()->isSrgb();
}

void Texture::setRepeated(bool repeated)
{
    _self_sf()->setRepeated(repeated);
}

bool Texture::isRepeated() const
{
    return _self_sf()->isRepeated();
}

bool Texture::generateMipmap()
{
    return _self_sf()->generateMipmap();
}

unsigned int Texture::getNativeHandle()
{
    return _self_sf()->getNativeHandle();
}

void Texture::bind(Texture const & texture, CoordinateType type)
{
    sf::Texture::bind(texture.cast<sf::Texture>(), (sf::Texture::CoordinateType)type);
}

unsigned int Texture::getMaximumSize()
{
    return sf::Texture::getMaximumSize();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

