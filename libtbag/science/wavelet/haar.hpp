/**
 * @file   haar.hpp
 * @brief  haar structure prototype.
 * @author zer0
 * @date   2016-05-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCIENCE_WAVELET_HAAR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCIENCE_WAVELET_HAAR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

#include <cassert>
#include <vector>

// haar coefficients.
//#define HAAR_COEFF  (0.7071067811865475244) // == sqrt(2) / 2
#define   HAAR_COEFF  (0.5)

#define HAAR_LOW0   (HAAR_COEFF)
#define HAAR_LOW1   (HAAR_COEFF)
#define HAAR_HIGH0  (HAAR_COEFF)
#define HAAR_HIGH1  (HAAR_COEFF * (-1))

// Programmable flag.
// - If this is a problem in a multi-threaded environment to release this flag.
//#define ENABLE_HAAR_STATIC_CACHE

#if defined(ENABLE_HAAR_STATIC_CACHE)
#define HAAR_STATIC_CACHE(type, name) static type name;
#else
#define HAAR_STATIC_CACHE(type, name) type name;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace science {
namespace wavelet {
namespace haar    {

template <typename T>
bool forward(T const * data, std::size_t size, T * result
        , std::size_t data_step = 1, std::size_t result_step = 1)
{
    assert(data   != nullptr);
    assert(result != nullptr);
    assert(size > 0);
    assert(data_step   >= 1);
    assert(result_step >= 1);

    std::size_t const HALF = (size >> 1);
    std::size_t step = 0;
    std::size_t index = 0;

    HAAR_STATIC_CACHE(std::vector<T>, temp);
    if (size > temp.size()) {
        temp.resize(size);
    }

    for (index = 0; index < HALF; ++index) {
        step = (index << 1);

        /*
         * Code review:
         *
         * temp[cursor       ]:  Low-pass result.
         * temp[cursor + half]: High-pass result.
         */

        // // Non-optimized code.
        // temp[cursor       ] = (data[twice] *  HAAR_LOW0) + (data[twice + 1] *  HAAR_LOW1);
        // temp[cursor + half] = (data[twice] * HAAR_HIGH0) + (data[twice + 1] * HAAR_HIGH1);

        // Optimized code.
        temp[index       ] = (data[step * data_step] + data[(step + 1) * data_step]) * HAAR_COEFF;
        temp[index + HALF] = (data[step * data_step] - data[(step + 1) * data_step]) * HAAR_COEFF;
    }

    if (result != nullptr) {
        for (index = 0; index < size; ++index) {
            result[index * result_step] = temp[index];
        }
    }

    return true;
}

template <typename T>
bool inverse(T const * data, std::size_t size, T * result
        , std::size_t data_step = 1, std::size_t result_step = 1)
{
    assert(data   != nullptr);
    assert(result != nullptr);
    assert(size > 0);

    std::size_t const HALF = (size >> 1);
    std::size_t step = 0;
    std::size_t index = 0;

    HAAR_STATIC_CACHE(std::vector<T>, temp);
    if (size > temp.size()) {
        temp.resize(size);
    }

    for (index = 0; index < HALF; ++index) {
        step = (index << 1);
        temp[step    ] = (data[index * data_step] + data[(index + HALF) * data_step]);
        temp[step + 1] = (data[index * data_step] - data[(index + HALF) * data_step]);
    }

    if (result != nullptr) {
        for (index = 0; index < size; ++index) {
            result[index * result_step] = temp[index];
        }
    }

    return true;
}

/**
 * @remarks
 *  @code
 *  LL | LH
 *  ---+---
 *  HL | HH
 *  @endcode
 *  - LL: Low-Low (Base image of next level).
 *  - LH: Low-High.
 *  - HL: High-Low.
 *  - HH: High-High.
 */
template <typename T>
bool haar2d(T const * data, std::size_t width, std::size_t height, T * result, bool direction = true)
{
    assert(data   != nullptr);
    assert(result != nullptr);
    assert(width  > 0);
    assert(height > 0);

    std::size_t const SIZE = width * height;
    std::size_t index = 0;

    HAAR_STATIC_CACHE(std::vector<T>, data_temp);
    if (SIZE > data_temp.size()) {
        data_temp.resize(SIZE);
    }

    // original to temp.
    for (index = 0; index < SIZE; ++index) {
        data_temp[index] = data[index];
    }

    // Haar of row.
    for (index = 0; index < height; ++index) {
        if (direction) {
            forward(&data_temp[index * width], width, &data_temp[index * width]);
        } else {
            inverse(&data_temp[index * width], width, &data_temp[index * width]);
        }
    }

    // Haar of column.
    for (index = 0; index < width; ++index) {
        if (direction) {
            forward(&data_temp[index], width, &data_temp[index], width, width);
        } else {
            inverse(&data_temp[index], width, &data_temp[index], width, width);
        }
    }

    // temp to result.
    if (result != nullptr) {
        for (index = 0; index < SIZE; ++index) {
            result[index] = data_temp[index];
        }
    }

    return true;
}

template <typename T>
bool forward2d(T const * data, std::size_t width, std::size_t height, T * result)
{
    return haar2d<T>(data, width, height, result, true);
}

template <typename T>
bool inverse2d(T const * data, std::size_t width, std::size_t height, T * result)
{
    return haar2d<T>(data, width, height, result, false);
}

} // namespace haar
} // namespace wavelet
} // namespace science

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCIENCE_WAVELET_HAAR_HPP__

