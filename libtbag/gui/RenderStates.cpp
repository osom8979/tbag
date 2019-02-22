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

RenderStates::RenderStates(BlendMode const & blend_mode)
        : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
    setBlendMode(blend_mode);
}

RenderStates::RenderStates(Transform const & transform)
        : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
    setTransform(transform);
}

RenderStates::RenderStates(Texture const * texture)
        : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
    setTexturePtr(texture);
}

RenderStates::RenderStates(Shader const * shader)
        : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
    setShaderPtr(shader);
}

RenderStates::RenderStates(BlendMode const & blend_mode,
                           Transform const & transform,
                           Texture const * texture,
                           Shader const * shader)
        : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
    setBlendMode(blend_mode);
    setTransform(transform);
    setTexturePtr(texture);
    setShaderPtr(shader);
}

RenderStates::RenderStates(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_RENDER_STATES, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

RenderStates::RenderStates(RenderStates const & obj)
        : SfNative(SfType::ST_RENDER_STATES)
{
    assert(ptr != nullptr);
    *this = obj;
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

RenderStates & RenderStates::operator =(RenderStates const & obj)
{
    if (this != &obj) {
        _self_sf()->blendMode = obj.cast<sf::RenderStates>()->blendMode;
        _self_sf()->transform = obj.cast<sf::RenderStates>()->transform;
        _self_sf()->texture   = obj.cast<sf::RenderStates>()->texture;
        _self_sf()->shader    = obj.cast<sf::RenderStates>()->shader;
    }
    return *this;
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

void RenderStates::setTransform(Transform const & t)
{
    auto * destination = const_cast<float*>(_self_sf()->transform.getMatrix());
    auto const * SOURCE = t.getMatrix();
    memcpy(destination, SOURCE, Transform::MATRIX_BYTE_SIZE);
}

void RenderStates::setTexturePtr(Texture const * t)
{
    _self_sf()->texture = t->cast<sf::Texture>();
}

void RenderStates::setShaderPtr(Shader const * s)
{
    _self_sf()->shader = s->cast<sf::Shader>();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

