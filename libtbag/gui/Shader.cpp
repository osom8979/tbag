/**
 * @file   Shader.cpp
 * @brief  Shader class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/Shader.hpp>
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
#define _self_sf() Parent::cast<sf::Shader>()
#endif

Shader::Shader() : SfNative(SfType::ST_SHADER)
{
    // EMPTY.
}

Shader::Shader(Shader && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_SHADER, no_init)
{
    *this = std::move(obj);
}

Shader::~Shader()
{
    // EMPTY.
}

Shader & Shader::operator =(Shader && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Shader::swap(Shader & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

