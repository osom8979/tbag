/**
 * @file   Shader.hpp
 * @brief  Shader class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point3.hpp>
#include <libtbag/geometry/Point4.hpp>
#include <libtbag/math/Matrix.hpp>
#include <libtbag/gui/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

// Forward declaration.
class Texture;

/**
 * Shader class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API Shader : public SfNative
{
public:
    enum class Type
    {
        T_VERTEX,
        T_GEOMETRY,
        T_FRAGMENT
    };

    using Vec2  = libtbag::geometry::Point2f;
    using Ivec2 = libtbag::geometry::Point2i;
    using Bvec2 = libtbag::geometry::Point2b;

    using Vec3  = libtbag::geometry::Point3f;
    using Ivec3 = libtbag::geometry::Point3i;
    using Bvec3 = libtbag::geometry::Point3b;

    using Vec4  = libtbag::geometry::Point4f;
    using Ivec4 = libtbag::geometry::Point4i;
    using Bvec4 = libtbag::geometry::Point4b;

    using Mat3 = libtbag::math::Matrix3x3f;
    using Mat4 = libtbag::math::Matrix4x4f;

public:
    struct current_texture_t {};

public:
    TBAG_CONSTEXPR static current_texture_t current_texture = current_texture_t{};

public:
    Shader();
    Shader(void * handle, no_init_no_ref_t);
    Shader(Shader && obj) TBAG_NOEXCEPT;
    ~Shader();

public:
    Shader & operator =(Shader && obj) TBAG_NOEXCEPT;

public:
    void swap(Shader & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Shader & lh, Shader & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    bool loadFromFile(std::string const & filename, Type type);
    bool loadFromFile(std::string const & vertex, std::string const & fragment);
    bool loadFromFile(std::string const & vertex, std::string const & geometry, std::string const & fragment);

    bool loadFromMemory(std::string const & shader, Type type);
    bool loadFromMemory(std::string const & vertex, std::string const & fragment);
    bool loadFromMemory(std::string const & vertex, std::string const & geometry, std::string const & fragment);

public:
    void setUniform(std::string const & name, float x);
    void setUniform(std::string const & name, Vec2 const & vector);
    void setUniform(std::string const & name, Vec3 const & vector);
    void setUniform(std::string const & name, Vec4 const & vector);

    void setUniform(std::string const & name, int x);
    void setUniform(std::string const & name, Ivec2 const & vector);
    void setUniform(std::string const & name, Ivec3 const & vector);
    void setUniform(std::string const & name, Ivec4 const & vector);

    void setUniform(std::string const & name, bool x);
    void setUniform(std::string const & name, Bvec2 const & vector);
    void setUniform(std::string const & name, Bvec3 const & vector);
    void setUniform(std::string const & name, Bvec4 const & vector);

    void setUniform(std::string const & name, Mat3 const & matrix);
    void setUniform(std::string const & name, Mat4 const & matrix);

    void setUniform(std::string const & name, Texture const & texture);
    void setUniform(std::string const & name, current_texture_t);

    void setUniformArray(std::string const & name, float const * scalar_arr, std::size_t length);
    void setUniformArray(std::string const & name, Vec2  const * vector_arr, std::size_t length);
    void setUniformArray(std::string const & name, Vec3  const * vector_arr, std::size_t length);
    void setUniformArray(std::string const & name, Vec4  const * vector_arr, std::size_t length);
    void setUniformArray(std::string const & name, Mat3  const * matrix_arr, std::size_t length);
    void setUniformArray(std::string const & name, Mat4  const * matrix_arr, std::size_t length);

    unsigned int getNativeHandle() const;

public:
    static void bind(Shader const & shader);
    static bool isAvailable();
    static bool isGeometryAvailable();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHADER_HPP__

