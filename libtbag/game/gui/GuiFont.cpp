/**
 * @file   GuiFont.cpp
 * @brief  GuiFont class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiFont.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/typography/font/Ngc.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

#include <SFML/Graphics.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

/**
 * GuiFont::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiFont::Impl : public sf::Font
{
private:
    GuiFont * _parent;

public:
    Impl(GuiFont * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------
// GuiFont implementation
// ----------------------

using Glyph = GuiFont::Glyph;

GuiFont::GuiFont() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiFont::GuiFont(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiFont::GuiFont(GuiFont const & obj) TBAG_NOEXCEPT : GuiFont(nullptr)
{
    (*this) = obj;
}

GuiFont::GuiFont(GuiFont && obj) TBAG_NOEXCEPT : GuiFont(nullptr)
{
    (*this) = std::move(obj);
}

GuiFont::~GuiFont()
{
    // EMPTY.
}

GuiFont & GuiFont::operator =(GuiFont const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiFont & GuiFont::operator =(GuiFont && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiFont::copy(GuiFont const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiFont::swap(GuiFont & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiFont::reset()
{
    _impl.reset();
}

bool GuiFont::loadFromFile(std::string const & filename)
{
    assert(exists());
    return _impl->loadFromFile(filename);
}

bool GuiFont::loadFromMemory(void const * data, std::size_t size)
{
    assert(exists());
    return _impl->loadFromMemory(data, size);
}

bool GuiFont::loadNanumGothicCodingBold()
{
    assert(exists());
    auto const BUFFER = libtbag::typography::font::getNgcBold();
    return loadFromMemory(BUFFER.data(), BUFFER.size());
}

std::string GuiFont::getFamily() const
{
    assert(exists());
    return _impl->getInfo().family;
}

Glyph GuiFont::getGlyph(std::uint32_t code_point, unsigned int character_size, bool bold, float outline_thickness) const
{
    assert(exists());
    auto const GLYPH = _impl->getGlyph(code_point, character_size, bold, outline_thickness);
    Glyph result;
    result.advance = GLYPH.advance;
    result.bounds.x = GLYPH.bounds.left;
    result.bounds.y = GLYPH.bounds.top;
    result.bounds.width = GLYPH.bounds.width;
    result.bounds.height = GLYPH.bounds.height;
    result.texture_rect.x = GLYPH.textureRect.left;
    result.texture_rect.y = GLYPH.textureRect.top;
    result.texture_rect.width = GLYPH.textureRect.width;
    result.texture_rect.height = GLYPH.textureRect.height;
    return result;
}

float GuiFont::getKerning(std::uint32_t first, std::uint32_t second, unsigned int character_size) const
{
    assert(exists());
    return _impl->getKerning(first, second, character_size);
}

float GuiFont::getLineSpacing(unsigned int character_size) const
{
    assert(exists());
    return _impl->getLineSpacing(character_size);
}

float GuiFont::getUnderlinePosition(unsigned int character_size) const
{
    assert(exists());
    return _impl->getLineSpacing(character_size);
}

float GuiFont::getUnderlineThickness(unsigned int character_size) const
{
    assert(exists());
    return _impl->getUnderlineThickness(character_size);
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

