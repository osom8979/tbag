/**
 * @file   GuiTexture.cpp
 * @brief  GuiTexture class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiTexture.hpp>
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

// clang-format off
STATIC_ASSERT_INTEGER_EQUAL(sf::Texture::Normalized, GuiTexture::CoordinateType::CT_NORMALIZED);
STATIC_ASSERT_INTEGER_EQUAL(sf::Texture::Pixels,     GuiTexture::CoordinateType::CT_PIXELS);
// clang-format on

/**
 * GuiTexture::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiTexture::Impl : public sf::Texture
{
private:
    GuiTexture * _parent;

public:
    Impl(GuiTexture * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// -------------------------
// GuiTexture implementation
// -------------------------

using Size2i = GuiTexture::Size2i;

GuiTexture::GuiTexture() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiTexture::GuiTexture(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiTexture::GuiTexture(GuiTexture const & obj) TBAG_NOEXCEPT : GuiTexture(nullptr)
{
    (*this) = obj;
}

GuiTexture::GuiTexture(GuiTexture && obj) TBAG_NOEXCEPT : GuiTexture(nullptr)
{
    (*this) = std::move(obj);
}

GuiTexture::~GuiTexture()
{
    // EMPTY.
}

GuiTexture & GuiTexture::operator =(GuiTexture const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiTexture & GuiTexture::operator =(GuiTexture && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiTexture::copy(GuiTexture const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiTexture::swap(GuiTexture & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiTexture::reset()
{
    _impl.reset();
}

bool GuiTexture::create(unsigned int width, unsigned int height)
{
    assert(exists());
    return _impl->create(width, height);
}

bool GuiTexture::loadFromFile(std::string const & filename, int x, int y, int w, int h)
{
    assert(exists());
    return _impl->loadFromFile(filename, sf::IntRect(x, y, w, h));
}

bool GuiTexture::loadFromMemory(void const * data, std::size_t size, int x, int y, int w, int h)
{
    assert(exists());
    return _impl->loadFromMemory(data, size, sf::IntRect(x, y, w, h));
}

bool GuiTexture::loadFromImage(GuiImage const & image, int x, int y, int w, int h)
{
    assert(exists());
    return _impl->loadFromImage((sf::Image const &)image.ref(), sf::IntRect(x, y, w, h));
}

bool GuiTexture::loadFromFile(std::string const & filename, Rect2i const & area)
{
    return loadFromFile(filename, area.x, area.y, area.width, area.height);
}

bool GuiTexture::loadFromMemory(void const * data, std::size_t size, Rect2i const & area)
{
    return loadFromMemory(data, size, area.x, area.y, area.width, area.height);
}

bool GuiTexture::loadFromImage(GuiImage const & image, Rect2i const & area)
{
    return loadFromImage(image, area.x, area.y, area.width, area.height);
}

bool GuiTexture::loadFromFile(std::string const & filename)
{
    return loadFromFile(filename, Rect2i());
}

bool GuiTexture::loadFromMemory(void const * data, std::size_t size)
{
    return loadFromMemory(data, size, Rect2i());
}

bool GuiTexture::loadFromImage(GuiImage const & image)
{
    return loadFromImage(image, Rect2i());
}

void GuiTexture::getSize(int * w, int * h) const
{
    assert(exists());
    auto const SIZE = _impl->getSize();
    // clang-format off
    if (w) { *w = SIZE.x; }
    if (h) { *h = SIZE.y; }
    // clang-format on
}

Size2i GuiTexture::getSize() const
{
    Size2i size;
    getSize(&size.width, &size.height);
    return size;
}

GuiImage GuiTexture::copyToImage() const
{
    assert(exists());
    auto const IMAGE = _impl->copyToImage();
    auto const SIZE = IMAGE.getSize();
    GuiImage image;
    image.create(SIZE.x, SIZE.y, IMAGE.getPixelsPtr());
    return image;
}

void GuiTexture::update(std::uint8_t const * pixels)
{
    assert(exists());
    _impl->update(pixels);
}

void GuiTexture::update(std::uint8_t const * pixels, unsigned int width, unsigned int height,
                        unsigned int x, unsigned int y)
{
    assert(exists());
    _impl->update(pixels, width, height, x, y);
}

void GuiTexture::update(GuiTexture const & texture)
{
    assert(exists());
    _impl->update(texture.ref());
}

void GuiTexture::update(GuiTexture const & texture, unsigned int x, unsigned int y)
{
    assert(exists());
    _impl->update(texture.ref(), x, y);
}

void GuiTexture::update(GuiImage const & image)
{
    assert(exists());
    _impl->update((sf::Image const &)image.ref());
}

void GuiTexture::update(GuiImage const & image, unsigned int x, unsigned int y)
{
    assert(exists());
    _impl->update((sf::Image const &)image.ref(), x, y);
}

void GuiTexture::setSmooth(bool smooth)
{
    assert(exists());
    _impl->setSmooth(smooth);
}

bool GuiTexture::isSmooth() const
{
    assert(exists());
    return _impl->isSmooth();
}

void GuiTexture::setSrgb(bool srgb)
{
    assert(exists());
    _impl->setSrgb(srgb);
}

bool GuiTexture::isSrgb() const
{
    assert(exists());
    return _impl->isSrgb();
}

void GuiTexture::setRepeated(bool repeated)
{
    assert(exists());
    _impl->setRepeated(repeated);
}

bool GuiTexture::isRepeated() const
{
    assert(exists());
    return _impl->isRepeated();
}

bool GuiTexture::generateMipmap()
{
    assert(exists());
    return _impl->generateMipmap();
}

unsigned int GuiTexture::getNativeHandle() const
{
    assert(exists());
    return _impl->getNativeHandle();
}

void GuiTexture::bind(GuiTexture const & texture, CoordinateType coordinate)
{
    sf::Texture::bind(texture.get(), (sf::Texture::CoordinateType)coordinate);
}

unsigned int GuiTexture::getMaximumSize()
{
    return sf::Texture::getMaximumSize();
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

