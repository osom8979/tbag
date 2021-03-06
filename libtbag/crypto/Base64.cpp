/**
 * @file   Base64.cpp
 * @brief  Base64 class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/crypto/Base64.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cstring>
#include <vector>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

std::size_t getDecodeLength(std::string const & base64)
{
    char const BASE64_PADDING_CHAR = '=';

    if (base64.size() < 2) {
        return 0;
    }

    std::size_t length = base64.size();
    std::size_t padding = 0;

    char const LAST1 = base64[length - 1];
    char const LAST2 = base64[length - 2];

    if (LAST1 == BASE64_PADDING_CHAR && LAST2 == BASE64_PADDING_CHAR) {
        padding = 2;
    } else if (LAST1 == BASE64_PADDING_CHAR) {
        padding = 1;
    }

    return ((length * 3) / 4) - padding;
}

bool encodeBase64(std::string const & input, std::string & output)
{
    return encodeBase64(util::Buffer(input.begin(), input.end()), output);
}

bool decodeBase64(std::string const & input, std::string & output)
{
    util::Buffer buffer;
    bool const RESULT = decodeBase64(input, buffer);
    output.assign(buffer.begin(), buffer.end());
    return RESULT;
}

bool encodeBase64(char const * input, std::size_t size, std::string & output)
{
    BIO * b64 = BIO_new(BIO_f_base64());
    BIO * bmem = BIO_new(BIO_s_mem());
    BIO_push(b64, bmem);

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Ignore newlines - write everything in one line
    int const WSIZE = BIO_write(b64, input, size);
    BIO_flush(b64);

    BUF_MEM * bptr = nullptr;
    BIO_get_mem_ptr(b64, &bptr);

    output.clear();
    output.assign(bptr->data, bptr->data + bptr->length);
    auto const BLENGTH = bptr->length;

    BIO_free_all(b64);
    return WSIZE == size && /* CHECK THIS CONDITION */ BLENGTH >= size;
}

bool encodeBase64(util::Buffer const & input, std::string & output)
{
    return encodeBase64(input.data(), input.size(), output);
}

bool decodeBase64(std::string const & input, util::Buffer & output)
{
    std::size_t length = getDecodeLength(input);

    std::vector<char> buffer;
    buffer.resize(length);

    BIO * b64 = BIO_new(BIO_f_base64());
    BIO * bmem = BIO_new_mem_buf((void*)input.data(), input.size());
    BIO_push(b64, bmem);

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    int const RSIZE = BIO_read(b64, buffer.data(), length);
    BIO_flush(b64);

    output.clear();
    output.assign(buffer.begin(), buffer.end());

    BIO_free_all(b64);
    return length == RSIZE;
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

