/**
 * @file   GuiText.cpp
 * @brief  GuiText class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiText.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/game/gui/GuiFont.hpp>

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
STATIC_ASSERT_INTEGER_EQUAL(sf::Text::Style::Regular,       GuiText::STYLE_REGULAR);
STATIC_ASSERT_INTEGER_EQUAL(sf::Text::Style::Bold,          GuiText::STYLE_BOLD);
STATIC_ASSERT_INTEGER_EQUAL(sf::Text::Style::Italic,        GuiText::STYLE_ITALIC);
STATIC_ASSERT_INTEGER_EQUAL(sf::Text::Style::Underlined,    GuiText::STYLE_UNDERLINED);
STATIC_ASSERT_INTEGER_EQUAL(sf::Text::Style::StrikeThrough, GuiText::STYLE_STRIKETHROUGH);
// clang-format on

// clang-format off
using Size2f  = GuiText::Size2f;
using Point2f = GuiText::Point2f;
using Rgb32   = GuiText::Rgb32;
using Rect2f  = GuiText::Rect2f;
using GuiFont = libtbag::game::gui::GuiFont;
// clang-format on

/**
 * GuiText::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiText::Impl : public sf::Text
{
private:
    GuiText * _parent;
    GuiFont _font;

public:
    Impl(GuiText * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    void setGuiFont(GuiFont const & font)
    {
        _font = font;
        sf::Text::setFont(*((sf::Font const *)_font.get()));
    }

    GuiFont getGuiFont() const
    {
        return _font;
    }
};

// ----------------------
// GuiText implementation
// ----------------------

GuiText::GuiText() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiText::GuiText(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiText::GuiText(GuiText const & obj) TBAG_NOEXCEPT : GuiText(nullptr)
{
    (*this) = obj;
}

GuiText::GuiText(GuiText && obj) TBAG_NOEXCEPT : GuiText(nullptr)
{
    (*this) = std::move(obj);
}

GuiText::~GuiText()
{
    // EMPTY.
}

GuiText & GuiText::operator =(GuiText const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiText & GuiText::operator =(GuiText && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiText::copy(GuiText const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiText::swap(GuiText & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiText::reset()
{
    _impl.reset();
}

void GuiText::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setFillColor(sf::Color(r, g, b, a));
}

void GuiText::setOutlineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setOutlineColor(sf::Color(r, g, b, a));
}

void GuiText::setOutlineThickness(float thickness)
{
    assert(exists());
    _impl->setOutlineThickness(thickness);
}

void GuiText::getFillColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
{
    assert(exists());
    auto const COLOR = _impl->getFillColor();
    // clang-format off
    if (r) { *r = COLOR.r; }
    if (g) { *g = COLOR.g; }
    if (b) { *b = COLOR.b; }
    if (a) { *a = COLOR.a; }
    // clang-format on
}

void GuiText::getOutlineColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
{
    assert(exists());
    auto const COLOR = _impl->getOutlineColor();
    // clang-format off
    if (r) { *r = COLOR.r; }
    if (g) { *g = COLOR.g; }
    if (b) { *b = COLOR.b; }
    if (a) { *a = COLOR.a; }
    // clang-format on
}

void GuiText::getOutlineThickness(float * thickness) const
{
    assert(exists());
    if (thickness) {
        *thickness = _impl->getOutlineThickness();
    }
}

void GuiText::getLocalBounds(float * x, float * y, float * w, float * h) const
{
    assert(exists());
    auto const BOUNDS = _impl->getLocalBounds();
    // clang-format off
    if (x) { *x = BOUNDS.left; }
    if (y) { *y = BOUNDS.top; }
    if (w) { *w = BOUNDS.width; }
    if (h) { *h = BOUNDS.height; }
    // clang-format on
}

void GuiText::getGlobalBounds(float * x, float * y, float * w, float * h) const
{
    assert(exists());
    auto const BOUNDS = _impl->getGlobalBounds();
    // clang-format off
    if (x) { *x = BOUNDS.left; }
    if (y) { *y = BOUNDS.top; }
    if (w) { *w = BOUNDS.width; }
    if (h) { *h = BOUNDS.height; }
    // clang-format on
}

void GuiText::setPosition(float x, float y)
{
    assert(exists());
    _impl->setPosition(x, y);
}

void GuiText::setRotation(float angle)
{
    assert(exists());
    _impl->setRotation(angle);
}

void GuiText::setScale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->setScale(factor_x, factor_y);
}

void GuiText::setOrigin(float x, float y)
{
    assert(exists());
    _impl->setOrigin(x, y);
}

void GuiText::getPosition(float * x, float * y) const
{
    assert(exists());
    auto const & POS = _impl->getPosition();
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiText::getRotation(float * angle) const
{
    assert(exists());
    if (angle) {
        *angle = _impl->getRotation();
    }
}

void GuiText::getScale(float * x, float * y) const
{
    assert(exists());
    auto const & SCALE = _impl->getScale();
    // clang-format off
    if (x) { *x = SCALE.x; }
    if (y) { *y = SCALE.y; }
    // clang-format on
}

void GuiText::getOrigin(float * x, float * y) const
{
    assert(exists());
    auto const & ORIGIN = _impl->getOrigin();
    // clang-format off
    if (x) { *x = ORIGIN.x; }
    if (y) { *y = ORIGIN.y; }
    // clang-format on
}

void GuiText::move(float offset_x, float offset_y)
{
    assert(exists());
    _impl->move(offset_x, offset_y);
}

void GuiText::rotate(float angle)
{
    assert(exists());
    _impl->rotate(angle);
}

void GuiText::scale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->scale(factor_x, factor_y);
}

void GuiText::setString(std::string const & string)
{
    assert(exists());
    _impl->setString(string);
}

void GuiText::setFont(GuiFont const & font)
{
    assert(exists());
    _impl->setGuiFont(font);
}

void GuiText::setCharacterSize(unsigned int size)
{
    assert(exists());
    _impl->setCharacterSize(size);
}

void GuiText::setLineSpacing(float spacing_factor)
{
    assert(exists());
    _impl->setLineSpacing(spacing_factor);
}

void GuiText::setLetterSpacing(float spacing_factor)
{
    assert(exists());
    _impl->setLineSpacing(spacing_factor);
}

void GuiText::setStyle(unsigned style)
{
    assert(exists());
    _impl->setStyle(style);
}

std::string GuiText::geString() const
{
    assert(exists());
    return _impl->getString();
}

GuiFont GuiText::getFont() const
{
    assert(exists());
    return _impl->getGuiFont();
}

unsigned int GuiText::getCharacterSize() const
{
    assert(exists());
    return _impl->getCharacterSize();
}

float GuiText::getLetterSpacing() const
{
    assert(exists());
    return _impl->getLetterSpacing();
}

float GuiText::getLineSpacing() const
{
    assert(exists());
    return _impl->getLineSpacing();
}

unsigned GuiText::getStyle() const
{
    assert(exists());
    return _impl->getStyle();
}

void GuiText::findCharacterPos(std::size_t index, float * x, float * y) const
{
    assert(exists());
    auto const POS = _impl->findCharacterPos(index);
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiText::setFillColor(Rgb32 const & color)
{
    setFillColor(color.r, color.g, color.b, color.a);
}

void GuiText::setOutlineColor(Rgb32 const & color)
{
    setOutlineColor(color.r, color.g, color.b, color.a);
}

Rgb32 GuiText::getFillColor() const
{
    Rgb32 color;
    getFillColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rgb32 GuiText::getOutlineColor() const
{
    Rgb32 color;
    getOutlineColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rect2f GuiText::getLocalBounds() const
{
    Rect2f rect;
    getLocalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

Rect2f GuiText::getGlobalBounds() const
{
    Rect2f rect;
    getGlobalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

void GuiText::setPosition(Point2f const & pos)
{
    setPosition(pos.x, pos.y);
}

void GuiText::setScale(Point2f const & factor)
{
    setScale(factor.x, factor.y);
}

void GuiText::setOrigin(Point2f const & origin)
{
    setScale(origin.x, origin.y);
}

Point2f GuiText::getPosition() const
{
    Point2f pos;
    getPosition(&pos.x, &pos.y);
    return pos;
}

Point2f GuiText::getScale() const
{
    Point2f scale;
    getScale(&scale.x, &scale.y);
    return scale;
}

Point2f GuiText::getOrigin() const
{
    Point2f origin;
    getOrigin(&origin.x, &origin.y);
    return origin;
}

void GuiText::move(Point2f const & offset)
{
    move(offset.x, offset.y);
}

void GuiText::scale(Point2f const & factor)
{
    scale(factor.x, factor.y);
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

