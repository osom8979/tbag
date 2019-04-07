/**
 * @file   GuiImage.cpp
 * @brief  GuiImage class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiImage.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

#include <SFML/Graphics.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

using Size2u = GuiImage::Size2u;
using Rgb32 = GuiImage::Rgb32;

/**
 * GuiImage::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiImage::Impl : public sf::Image
{
private:
    GuiImage * _parent;

public:
    Impl(GuiImage * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// -----------------------
// GuiImage implementation
// -----------------------

GuiImage::GuiImage() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiImage::GuiImage(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiImage::GuiImage(GuiImage const & obj) TBAG_NOEXCEPT : GuiImage(nullptr)
{
    (*this) = obj;
}

GuiImage::GuiImage(GuiImage && obj) TBAG_NOEXCEPT : GuiImage(nullptr)
{
    (*this) = std::move(obj);
}

GuiImage::~GuiImage()
{
    // EMPTY.
}

GuiImage & GuiImage::operator =(GuiImage const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiImage & GuiImage::operator =(GuiImage && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiImage::copy(GuiImage const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiImage::swap(GuiImage & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiImage::reset()
{
    _impl.reset();
}

void GuiImage::create(unsigned int width, unsigned int height,
                      unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->create(width, height, sf::Color(r, g, b, a));
}

void GuiImage::create(unsigned int width, unsigned int height, std::uint8_t const * pixels)
{
    assert(exists());
    _impl->create(width, height, pixels);
}

bool GuiImage::loadFromFile(std::string const & filename)
{
    assert(exists());
    return _impl->loadFromFile(filename);
}

bool GuiImage::loadFromMemory(void const * data, std::size_t size)
{
    assert(exists());
    return _impl->loadFromMemory(data, size);
}

bool GuiImage::saveToFile(std::string const & filename) const
{
    assert(exists());
    return _impl->saveToFile(filename);
}

void GuiImage::getSize(unsigned int * width, unsigned int * height) const
{
    assert(exists());
    auto const SIZE = _impl->getSize();
    // clang-format off
    if (width) { *width = SIZE.x; }
    if (height) { *height = SIZE.y; }
    // clang-format on
}

void GuiImage::createMaskFromColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a,
                                   unsigned char assign_alpha)
{
    assert(exists());
    _impl->createMaskFromColor(sf::Color(r, g, b, a), assign_alpha);
}

void GuiImage::copyFrom(GuiImage const & source, unsigned int dest_x, unsigned int dest_y,
                        int x, int y, int w, int h, bool apply_alpha)
{
    assert(exists());
    _impl->copy(*(sf::Image const *)source.get(), dest_x, dest_y, sf::IntRect(x, y, w, h), apply_alpha);
}

void GuiImage::setPixel(unsigned int x, unsigned int y,
                        unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setPixel(x, y, sf::Color(r, g, b, a));
}

void GuiImage::getPixel(unsigned int x, unsigned int y,
                        unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
{
    assert(exists());
    auto const COLOR = _impl->getPixel(x, y);
    // clang-format off
    if (r) { *r = COLOR.r; }
    if (g) { *g = COLOR.g; }
    if (b) { *b = COLOR.b; }
    if (a) { *a = COLOR.a; }
    // clang-format on
}

unsigned char const * GuiImage::getPixelsPtr() const
{
    assert(exists());
    return (unsigned char const *)_impl->getPixelsPtr();
}

void GuiImage::flipHorizontally()
{
    assert(exists());
    _impl->flipHorizontally();
}

void GuiImage::flipVertically()
{
    assert(exists());
    _impl->flipVertically();
}

void GuiImage::create(Size2u const & size, Rgb32 const & color)
{
    create(size.width, size.height, color.r, color.g, color.b, color.a);
}

Size2u GuiImage::getSize() const
{
    Size2u size;
    getSize(&size.width, &size.height);
    return size;
}

void GuiImage::createMaskFromColor(Rgb32 const & color, unsigned char assign_alpha)
{
    createMaskFromColor(color.r, color.g, color.b, color.a, assign_alpha);
}

void GuiImage::copyFrom(GuiImage const & source, Point2u const & dest, Rect2i const & pos, bool apply_alpha)
{
    copyFrom(source, dest.x, dest.y, pos.x, pos.y, pos.width, pos.height, apply_alpha);
}

void GuiImage::setPixel(Point2u const & pos, Rgb32 const & color)
{
    setPixel(pos.x, pos.y, color.r, color.g, color.b, color.a);
}

Rgb32 GuiImage::getPixel(Point2u const & pos) const
{
    Rgb32 color;
    getPixel(pos.x, pos.y, &color.r, &color.g, &color.b, &color.a);
    return color;
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

