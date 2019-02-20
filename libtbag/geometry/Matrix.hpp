/**
 * @file   Matrix.hpp
 * @brief  Matrix class prototype.
 * @author zer0
 * @date   2019-02-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_MATRIX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_MATRIX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <algorithm>
#include <utility>
#include <type_traits>
#include <initializer_list>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

/**
 * BaseMatrix class prototype.
 *
 * @author zer0
 * @date   2019-02-20
 */
template <typename BaseType, std::size_t ColumnSize, std::size_t RowSize>
struct BaseMatrix
{
    using Type = BaseType;

    TBAG_CONSTEXPR static std::size_t const columns = ColumnSize;
    TBAG_CONSTEXPR static std::size_t const rows = RowSize;
    TBAG_CONSTEXPR static std::size_t const array_size = columns * rows;
    TBAG_CONSTEXPR static std::size_t const array_byte = sizeof(Type) * array_size;

    TBAG_CONSTEXPR static std::size_t size() TBAG_NOEXCEPT { return array_size; }
    TBAG_CONSTEXPR static std::size_t byte() TBAG_NOEXCEPT { return array_byte; }

    Type array[array_size];

    BaseMatrix() TBAG_NOEXCEPT_SP_OP(std::is_nothrow_default_constructible<Type>::value)
    {
        std::fill(array, array + array_size, Type());
    }

    BaseMatrix(std::initializer_list<Type> list) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_default_constructible<Type>::value)
    {
        std::copy(list.begin(), list.end(), array);
    }

    BaseMatrix(Type const * p)
    {
        std::copy(p, p + array_size, array);
    }

    BaseMatrix(BaseMatrix const & mat) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_copy_constructible<Type>::value)
    {
        *this = mat;
    }

    BaseMatrix(BaseMatrix && mat) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_move_constructible<Type>::value)
    {
        *this = std::move(mat);
    }

    ~BaseMatrix()
    {
        // EMPTY.
    }

    BaseMatrix & operator =(BaseMatrix const & mat) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_copy_assignable<Type>::value)
    {
        if (this != &mat) {
            std::copy(mat.array, mat.array + mat.array_size, array);
        }
        return *this;
    }

    BaseMatrix & operator =(BaseMatrix && mat) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_move_assignable<Type>::value)
    {
        swap(mat);
        return *this;
    }

    void swap(BaseMatrix & mat) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_move_assignable<Type>::value)
    {
        if (this != &mat) {
            std::swap_ranges(mat.array, mat.array + mat.array_size, array);
        }
    }

    friend void swap(BaseMatrix & lh, BaseMatrix & rh) TBAG_NOEXCEPT_SP_OP(std::is_nothrow_move_assignable<Type>::value)
    {
        lh.swap(rh);
    }

    Type & operator [](std::size_t index) TBAG_NOEXCEPT
    {
        return array[index];
    }

    Type const & operator [](std::size_t index) const TBAG_NOEXCEPT
    {
        return array[index];
    }

    Type & at(std::size_t index) TBAG_NOEXCEPT
    {
        return array[index];
    }

    Type const & at(std::size_t index) const TBAG_NOEXCEPT
    {
        return array[index];
    }

    Type & at(std::size_t c, std::size_t r) TBAG_NOEXCEPT
    {
        return array[c * rows + r];
    }

    Type const & at(std::size_t c, std::size_t r) const TBAG_NOEXCEPT
    {
        return array[c * rows + r];
    }
};

// @formatter:off
template <std::size_t c, std::size_t r> using Matrixb  = BaseMatrix<bool    , c, r>;
template <std::size_t c, std::size_t r> using Matrixc  = BaseMatrix<char    , c, r>;
template <std::size_t c, std::size_t r> using Matrixs  = BaseMatrix<short   , c, r>;
template <std::size_t c, std::size_t r> using Matrixi  = BaseMatrix<int     , c, r>;
template <std::size_t c, std::size_t r> using Matrixu  = BaseMatrix<unsigned, c, r>;
template <std::size_t c, std::size_t r> using Matrixl  = BaseMatrix<long    , c, r>;
template <std::size_t c, std::size_t r> using Matrixll = BaseMatrix<int64_t , c, r>;
template <std::size_t c, std::size_t r> using Matrixf  = BaseMatrix<float   , c, r>;
template <std::size_t c, std::size_t r> using Matrixd  = BaseMatrix<double  , c, r>;
template <std::size_t c, std::size_t r> using Matrix   = Matrixi<c, r>;

using Matrix2x2b  = Matrixb <2, 2>;
using Matrix2x2c  = Matrixc <2, 2>;
using Matrix2x2s  = Matrixs <2, 2>;
using Matrix2x2i  = Matrixi <2, 2>;
using Matrix2x2u  = Matrixu <2, 2>;
using Matrix2x2l  = Matrixl <2, 2>;
using Matrix2x2ll = Matrixll<2, 2>;
using Matrix2x2f  = Matrixf <2, 2>;
using Matrix2x2d  = Matrixd <2, 2>;
using Matrix2x2   = Matrix2x2i;

using Matrix3x3b  = Matrixb <3, 3>;
using Matrix3x3c  = Matrixc <3, 3>;
using Matrix3x3s  = Matrixs <3, 3>;
using Matrix3x3i  = Matrixi <3, 3>;
using Matrix3x3u  = Matrixu <3, 3>;
using Matrix3x3l  = Matrixl <3, 3>;
using Matrix3x3ll = Matrixll<3, 3>;
using Matrix3x3f  = Matrixf <3, 3>;
using Matrix3x3d  = Matrixd <3, 3>;
using Matrix3x3   = Matrix3x3i;

using Matrix4x4b  = Matrixb <4, 4>;
using Matrix4x4c  = Matrixc <4, 4>;
using Matrix4x4s  = Matrixs <4, 4>;
using Matrix4x4i  = Matrixi <4, 4>;
using Matrix4x4u  = Matrixu <4, 4>;
using Matrix4x4l  = Matrixl <4, 4>;
using Matrix4x4ll = Matrixll<4, 4>;
using Matrix4x4f  = Matrixf <4, 4>;
using Matrix4x4d  = Matrixd <4, 4>;
using Matrix4x4   = Matrix4x4i;
// @formatter:on

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_MATRIX_HPP__

