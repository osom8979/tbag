/**
 * @file   Shader.cpp
 * @brief  Shader class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/Shader.hpp>
#include <libtbag/log/Log.hpp>

#ifndef _STATIC_ASSERT_EQUAL
#define _STATIC_ASSERT_EQUAL(x, y) STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
_STATIC_ASSERT_EQUAL(sf::Shader::Vertex,   libtbag::gui::Shader::Type::T_VERTEX);
_STATIC_ASSERT_EQUAL(sf::Shader::Geometry, libtbag::gui::Shader::Type::T_GEOMETRY);
_STATIC_ASSERT_EQUAL(sf::Shader::Fragment, libtbag::gui::Shader::Type::T_FRAGMENT);
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
#define _self_sf() Pointer::cast<sf::Shader>()
#endif

Shader::Shader() : SfNative(SfType::ST_SHADER)
{
    // EMPTY.
}

Shader::Shader(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_SHADER, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
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

bool Shader::loadFromFile(std::string const & filename, Type type)
{
    return _self_sf()->loadFromFile(filename, (sf::Shader::Type)type);
}

bool Shader::loadFromFile(std::string const & vertex, std::string const & fragment)
{
    return _self_sf()->loadFromFile(vertex, fragment);
}

bool Shader::loadFromFile(std::string const & vertex, std::string const & geometry, std::string const & fragment)
{
    return _self_sf()->loadFromFile(vertex, geometry, fragment);
}

bool Shader::loadFromMemory(std::string const & shader, Type type)
{
    return _self_sf()->loadFromMemory(shader, (sf::Shader::Type)type);
}

bool Shader::loadFromMemory(std::string const & vertex, std::string const & fragment)
{
    return _self_sf()->loadFromMemory(vertex, fragment);
}

bool Shader::loadFromMemory(std::string const & vertex, std::string const & geometry, std::string const & fragment)
{
    return _self_sf()->loadFromMemory(vertex, geometry, fragment);
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

