/**
 * @file   Aes128.cpp
 * @brief  Aes128 class implementation.
 * @author zer0
 * @date   2016-11-18
 */

#include <libtbag/encrypt/Aes128.hpp>
#include <libtbag/3rd/tinyaes128c/aes.h>
#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

Aes128::Aes128()
{
    // EMPTY.
}

Aes128::~Aes128()
{
    // EMPTY.
}

std::size_t Aes128::coding(Buffer & output, Key const & key, uint8_t const * input, std::size_t size, bool encode)
{
    uint8_t  in[KEY_SIZE] = {0,};
    uint8_t out[KEY_SIZE] = {0,};

    std::size_t read_index = 0;
    std::size_t avail_in   = 0;
    int eof = false;

    output.clear();

    while (eof == false) {
        if (size - read_index > KEY_SIZE) {
            avail_in = KEY_SIZE;
            eof = false;
        } else {
            avail_in = size - read_index;
            eof = true; // EOF.
        }

        memset(in, 0x00, sizeof(in));
        memcpy(in, input + read_index, avail_in);
        read_index += avail_in;

        if (encode) {
            AES128_ECB_encrypt(in, &key[0], out);
        } else {
            AES128_ECB_decrypt(in, &key[0], out);
        }
        output.insert(output.end(), out, out + KEY_SIZE);
    }

    return output.size();
}

std::size_t Aes128::predictSize(std::size_t size)
{
    if (size % KEY_SIZE == 0) {
        return size;
    }
    return size + KEY_SIZE - (size % KEY_SIZE);
}

std::size_t Aes128::encrypt(Buffer & output, Key const & key, uint8_t const * input, std::size_t size)
{
    return coding(output, key, input, size, AES_ENCODE);
}

std::size_t Aes128::decrypt(Buffer & output, Key const & key, uint8_t const * input, std::size_t size)
{
    return coding(output, key, input, size, AES_DECODE);
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

