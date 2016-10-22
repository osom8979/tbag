/**
 * @file   unit.hpp
 * @brief  unit package.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UNIT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UNIT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tmp/Power.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * @defgroup __DOXYGEN_GROUP__BYTE__ Byte.
 * @brief Unit of digital information.
 * @{
 */

uint64_t const      BYTE_UNIT = tmp::Power<     1024, 1>::value;
uint64_t const KILO_BYTE_UNIT = tmp::Power<BYTE_UNIT, 2>::value;
uint64_t const MEGA_BYTE_UNIT = tmp::Power<BYTE_UNIT, 3>::value;
uint64_t const GIGA_BYTE_UNIT = tmp::Power<BYTE_UNIT, 4>::value;
uint64_t const TERA_BYTE_UNIT = tmp::Power<BYTE_UNIT, 5>::value;

/**
 * @}
 */

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UNIT_HPP__

