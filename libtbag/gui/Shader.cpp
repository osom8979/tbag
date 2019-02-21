/**
 * @file   Shader.cpp
 * @brief  Shader class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/Shader.hpp>
#include <libtbag/gui/Texture.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
STATIC_ASSERT_INTEGER_EQUAL(sf::Shader::Vertex,   libtbag::gui::Shader::Type::T_VERTEX);
STATIC_ASSERT_INTEGER_EQUAL(sf::Shader::Geometry, libtbag::gui::Shader::Type::T_GEOMETRY);
STATIC_ASSERT_INTEGER_EQUAL(sf::Shader::Fragment, libtbag::gui::Shader::Type::T_FRAGMENT);
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

void Shader::setUniform(std::string const & name, float x)
{
    _self_sf()->setUniform(name, x);
}

void Shader::setUniform(std::string const & name, Vec2 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Vec2(vector.x, vector.y));
}

void Shader::setUniform(std::string const & name, Vec3 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Vec3(vector.x, vector.y, vector.z));
}

void Shader::setUniform(std::string const & name, Vec4 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Vec4(vector.x, vector.y, vector.z, vector.w));
}

void Shader::setUniform(std::string const & name, int x)
{
    _self_sf()->setUniform(name, x);
}

void Shader::setUniform(std::string const & name, Ivec2 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Ivec2(vector.x, vector.y));
}

void Shader::setUniform(std::string const & name, Ivec3 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Ivec3(vector.x, vector.y, vector.z));
}

void Shader::setUniform(std::string const & name, Ivec4 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Ivec4(vector.x, vector.y, vector.z, vector.w));
}

void Shader::setUniform(std::string const & name, bool x)
{
    _self_sf()->setUniform(name, x);
}

void Shader::setUniform(std::string const & name, Bvec2 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Bvec2(vector.x, vector.y));
}

void Shader::setUniform(std::string const & name, Bvec3 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Bvec3(vector.x, vector.y, vector.z));
}

void Shader::setUniform(std::string const & name, Bvec4 const & vector)
{
    _self_sf()->setUniform(name, sf::Glsl::Bvec4(vector.x, vector.y, vector.z, vector.w));
}

void Shader::setUniform(std::string const & name, Mat3 const & matrix)
{
    _self_sf()->setUniform(name, sf::Glsl::Mat3(matrix.array));
}

void Shader::setUniform(std::string const & name, Mat4 const & matrix)
{
    _self_sf()->setUniform(name, sf::Glsl::Mat4(matrix.array));
}

//void Shader::setUniform(std::string const & name, Texture const & texture)
//{
//    _self_sf()->setUniform(name, sf::Texture());
//}

void Shader::setUniform(std::string const & name, current_texture_t)
{
    _self_sf()->setUniform(name, sf::Shader::CurrentTextureType{});
}

void Shader::setUniformArray(std::string const & name, float const * scalar_arr, std::size_t length)
{
    _self_sf()->setUniformArray(name, scalar_arr, length);
}

void Shader::setUniformArray(std::string const & name, Vec2 const * vector_arr, std::size_t length)
{
    auto vector = std::make_unique<sf::Glsl::Vec2[]>(length);
    for (std::size_t i = 0; i < length; ++i) {
        vector[i].x = vector_arr[i].x;
        vector[i].y = vector_arr[i].y;
    }
    _self_sf()->setUniformArray(name, vector.get(), length);
}

void Shader::setUniformArray(std::string const & name, Vec3 const * vector_arr, std::size_t length)
{
    auto vector = std::make_unique<sf::Glsl::Vec3[]>(length);
    for (std::size_t i = 0; i < length; ++i) {
        vector[i].x = vector_arr[i].x;
        vector[i].y = vector_arr[i].y;
        vector[i].z = vector_arr[i].z;
    }
    _self_sf()->setUniformArray(name, vector.get(), length);
}

void Shader::setUniformArray(std::string const & name, Vec4 const * vector_arr, std::size_t length)
{
    auto vector = std::make_unique<sf::Glsl::Vec4[]>(length);
    for (std::size_t i = 0; i < length; ++i) {
        vector[i].x = vector_arr[i].x;
        vector[i].y = vector_arr[i].y;
        vector[i].z = vector_arr[i].z;
        vector[i].w = vector_arr[i].w;
    }
    _self_sf()->setUniformArray(name, vector.get(), length);
}

void Shader::setUniformArray(std::string const & name, Mat3 const * matrix_arr, std::size_t length)
{
    std::vector<sf::Glsl::Mat3> matrix;
    matrix.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        matrix.emplace_back(matrix_arr[i].array);
    }
    _self_sf()->setUniformArray(name, &(matrix[0]), length);
}

void Shader::setUniformArray(std::string const & name, Mat4 const * matrix_arr, std::size_t length)
{
    std::vector<sf::Glsl::Mat4> matrix;
    matrix.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        matrix.emplace_back(matrix_arr[i].array);
    }
    _self_sf()->setUniformArray(name, &(matrix[0]), length);
}

unsigned int Shader::getNativeHandle() const
{
    return _self_sf()->getNativeHandle();
}

void Shader::bind(Shader const & shader)
{
    return sf::Shader::bind(shader.cast<sf::Shader>());
}

bool Shader::isAvailable()
{
    return sf::Shader::isAvailable();
}

bool Shader::isGeometryAvailable()
{
    return sf::Shader::isGeometryAvailable();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

