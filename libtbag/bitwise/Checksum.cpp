/**
 * @file   Checksum.cpp
 * @brief  Checksum class implementation.
 * @author zer0
 * @date   2018-12-13
 */

#include <libtbag/bitwise/Checksum.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

uint8_t calcXorChecksum(uint8_t * data, std::size_t size) TBAG_NOEXCEPT
{
    uint8_t sum = 0;
    for (; size > 0; --size, ++data) {
        sum ^= *data;
    }
    return sum;
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

