/**
 * @file   SfShader.cpp
 * @brief  SfShader class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/sfpp/SfShader.hpp>
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

namespace sfpp {

SfShader::SfShader() : SfNative(SfType::ST_SHADER)
{
    // EMPTY.
}

SfShader::SfShader(SfShader && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_SHADER, no_init)
{
    *this = std::move(obj);
}

SfShader::~SfShader()
{
    // EMPTY.
}

SfShader & SfShader::operator =(SfShader && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SfShader::swap(SfShader & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::Shader>()
#endif

#undef _self_sf

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

