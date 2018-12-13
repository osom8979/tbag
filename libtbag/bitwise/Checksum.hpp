/**
 * @file   Checksum.hpp
 * @brief  Checksum class prototype.
 * @author zer0
 * @date   2018-12-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_CHECKSUM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_CHECKSUM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>
#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

TBAG_API uint8_t calcXorChecksum(uint8_t * data, std::size_t size) TBAG_NOEXCEPT;

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_CHECKSUM_HPP__

