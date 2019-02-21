/**
 * @file   BlendMode.cpp
 * @brief  BlendMode class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/BlendMode.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::Zero,             libtbag::gui::BlendMode::Factor::F_ZERO);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::One,              libtbag::gui::BlendMode::Factor::F_ONE);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::SrcColor,         libtbag::gui::BlendMode::Factor::F_SRC_COLOR);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::OneMinusSrcColor, libtbag::gui::BlendMode::Factor::F_ONE_MINUS_SRC_COLOR);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::DstColor,         libtbag::gui::BlendMode::Factor::F_DST_COLOR);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::OneMinusDstColor, libtbag::gui::BlendMode::Factor::F_ONE_MINUS_DST_COLOR);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::SrcAlpha,         libtbag::gui::BlendMode::Factor::F_SRC_ALPHA);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::OneMinusSrcAlpha, libtbag::gui::BlendMode::Factor::F_ONE_MINUS_SRC_ALPHA);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::DstAlpha,         libtbag::gui::BlendMode::Factor::F_DST_ALPHA);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::OneMinusDstAlpha, libtbag::gui::BlendMode::Factor::F_ONE_MINUS_DST_ALPHA);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::Add,              libtbag::gui::BlendMode::Equation::E_ADD);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::Subtract,         libtbag::gui::BlendMode::Equation::E_SUBTRACT);
STATIC_ASSERT_INTEGER_EQUAL(sf::BlendMode::ReverseSubtract , libtbag::gui::BlendMode::Equation::E_REVERSE_SUBTRACT);
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
#define _self_sf() Pointer::cast<sf::BlendMode>()
#endif

using Factor   = BlendMode::Factor;
using Equation = BlendMode::Equation;

BlendMode::BlendMode() : SfNative(SfType::ST_BLEND_MODE)
{
    assert(ptr != nullptr);
}

BlendMode::BlendMode(Factor source, Factor destination, Equation blend)
        : SfNative(SfType::ST_BLEND_MODE, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::BlendMode((sf::BlendMode::Factor)source,
                            (sf::BlendMode::Factor)destination,
                            (sf::BlendMode::Equation)blend);
    assert(ptr != nullptr);
}

BlendMode::BlendMode(Factor color_source,
                     Factor color_destination,
                     Equation color_blend,
                     Factor alpha_source,
                     Factor alpha_destination,
                     Equation alpha_blend)
        : SfNative(SfType::ST_BLEND_MODE, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::BlendMode((sf::BlendMode::Factor)color_source,
                            (sf::BlendMode::Factor)color_destination,
                            (sf::BlendMode::Equation)color_blend,
                            (sf::BlendMode::Factor)alpha_source,
                            (sf::BlendMode::Factor)alpha_destination,
                            (sf::BlendMode::Equation)alpha_blend);
    assert(ptr != nullptr);
}

BlendMode::BlendMode(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_BLEND_MODE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

BlendMode::BlendMode(BlendMode const & obj)
        : SfNative(SfType::ST_BLEND_MODE)
{
    assert(ptr != nullptr);
    *this = obj;
}

BlendMode::BlendMode(BlendMode && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_BLEND_MODE, no_init)
{
    *this = std::move(obj);
}

BlendMode::~BlendMode()
{
    // EMPTY.
}

BlendMode & BlendMode::operator =(BlendMode const & obj)
{
    if (this != &obj) {
        auto * destination = Pointer::cast<sf::BlendMode>();
        auto const * source = Pointer::cast<sf::BlendMode>();
        destination->colorSrcFactor = source->colorSrcFactor;
        destination->colorDstFactor = source->colorDstFactor;
        destination->colorEquation  = source->colorEquation;
        destination->alphaSrcFactor = source->alphaSrcFactor;
        destination->alphaDstFactor = source->alphaDstFactor;
        destination->alphaEquation  = source->alphaEquation;
    }
    return *this;
}


BlendMode & BlendMode::operator =(BlendMode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void BlendMode::swap(BlendMode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

void BlendMode::setColorSrcFactor(Factor f) TBAG_NOEXCEPT
{
    _self_sf()->colorSrcFactor = (sf::BlendMode::Factor)f;
}

Factor BlendMode::getColorSrcFactor() const TBAG_NOEXCEPT
{
    return (Factor)_self_sf()->colorSrcFactor;
}

void BlendMode::setColorDstFactor(Factor f) TBAG_NOEXCEPT
{
    _self_sf()->colorDstFactor = (sf::BlendMode::Factor)f;
}

Factor BlendMode::getColorDstFactor() const TBAG_NOEXCEPT
{
    return (Factor)_self_sf()->colorDstFactor;
}

void BlendMode::setColorEquation (Equation e) TBAG_NOEXCEPT
{
    _self_sf()->colorEquation = (sf::BlendMode::Equation)e;
}

Equation BlendMode::getColorEquation () const TBAG_NOEXCEPT
{
    return (Equation)_self_sf()->colorEquation;
}

void BlendMode::setAlphaSrcFactor(Factor f) TBAG_NOEXCEPT
{
    _self_sf()->alphaSrcFactor = (sf::BlendMode::Factor)f;
}

Factor BlendMode::getAlphaSrcFactor() const TBAG_NOEXCEPT
{
    return (Factor)_self_sf()->alphaSrcFactor;
}

void BlendMode::setAlphaDstFactor(Factor f) TBAG_NOEXCEPT
{
    _self_sf()->alphaDstFactor = (sf::BlendMode::Factor)f;
}

Factor BlendMode::getAlphaDstFactor() const TBAG_NOEXCEPT
{
    return (Factor)_self_sf()->alphaDstFactor;
}

void BlendMode::setAlphaEquation (Equation e) TBAG_NOEXCEPT
{
    _self_sf()->alphaEquation = (sf::BlendMode::Equation)e;
}

Equation BlendMode::getAlphaEquation () const TBAG_NOEXCEPT
{
    return (Equation)_self_sf()->alphaEquation;
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

