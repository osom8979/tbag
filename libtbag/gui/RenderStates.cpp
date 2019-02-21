/**
 * @file   RenderStates.cpp
 * @brief  RenderStates class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/RenderStates.hpp>
#include <libtbag/gui/BlendMode.hpp>
#include <libtbag/gui/Shader.hpp>
#include <libtbag/gui/Texture.hpp>
#include <libtbag/gui/Transform.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <cassert>
#include <cstring>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

#ifndef _self_sf
#define _self_sf() Pointer::cast<sf::RenderStates>()
#endif

RenderStates::RenderStates() : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
}

RenderStates::RenderStates(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_RENDER_STATES, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

RenderStates::RenderStates(RenderStates && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_RENDER_STATES, no_init)
{
    *this = std::move(obj);
}

RenderStates::~RenderStates()
{
    // EMPTY.
}

RenderStates & RenderStates::operator =(RenderStates && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void RenderStates::swap(RenderStates & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

void RenderStates::setBlendMode(BlendMode const & m)
{
    _self_sf()->blendMode = *m.cast<sf::BlendMode>();
}

BlendMode RenderStates::getBlendMode() const
{
    auto const & BLEND_MODE = _self_sf()->blendMode;
    return BlendMode((BlendMode::Factor)BLEND_MODE.colorSrcFactor,
                     (BlendMode::Factor)BLEND_MODE.colorDstFactor,
                     (BlendMode::Equation)BLEND_MODE.colorEquation,
                     (BlendMode::Factor)BLEND_MODE.alphaSrcFactor,
                     (BlendMode::Factor)BLEND_MODE.alphaDstFactor,
                     (BlendMode::Equation)BLEND_MODE.alphaEquation);
}

//void RenderStates::setTransform(Transform const & t)
//{
//    auto * destination = const_cast<float*>(_self_sf()->transform.getMatrix());
//    auto * source = t.getMatrix();
//    memcpy(destination, source, Transform::MATRIX_BYTE_SIZE);
//}
//
//Transform RenderStates::getTransform() const
//{
//    return Transform(_self_sf()->transform.getMatrix());
//}
//
//void RenderStates::setTexture(Texture const & t)
//{
//    _self_sf()->texture = *t.cast<sf::Texture>();
//}
//
//Texture RenderStates::getTexture() const
//{
//    return Texture(Texture((void*)_self_sf()->texture, Texture::no_init_no_ref));
//}
//
//void RenderStates::setShader(Shader const & s)
//{
//}
//
//Shader RenderStates::getShader() const
//{
//}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

