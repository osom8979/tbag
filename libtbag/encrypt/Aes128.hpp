/**
 * @file   Aes128.hpp
 * @brief  Aes128 class prototype.
 * @author zer0
 * @date   2016-11-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_AES128_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_AES128_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdint>
#include <array>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

/**
 * Aes128 class prototype.
 *
 * @author zer0
 * @date   2016-11-18
 */
class TBAG_API Aes128 : public Noncopyable
{
public:
    static int const KEY_SIZE = 128 / 8; // 16Byte.
    using Key    = std::array <uint8_t, KEY_SIZE>;
    using Buffer = std::vector<uint8_t>;

    static bool const AES_ENCODE = true;
    static bool const AES_DECODE = false;

public:
    Aes128();
    ~Aes128();

public:
    static std::size_t coding(Buffer & output, Key const & key, uint8_t const * input, std::size_t size, bool encode = AES_DECODE);
    static std::size_t predictSize(std::size_t size);

public:
    static std::size_t encrypt(Buffer & output, Key const & key, uint8_t const * input, std::size_t size);
    static std::size_t decrypt(Buffer & output, Key const & key, uint8_t const * input, std::size_t size);
};

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_AES128_HPP__

