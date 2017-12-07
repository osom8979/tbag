/**
 * @file   PseudoRandom.cpp
 * @brief  PseudoRandom class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/crypto/PseudoRandom.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/rand.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

util::Buffer generateRandom(std::size_t size)
{
    util::Buffer result(size);
    int const WRITTEN = RAND_bytes((unsigned char *)result.data(), result.size());
    RAND_seed(result.data(), WRITTEN);
    return result;
}

util::Buffer generatePseudoRandom(std::size_t size)
{
    util::Buffer result(size);
    int const WRITTEN = RAND_pseudo_bytes((unsigned char *)result.data(), result.size());
    RAND_seed(result.data(), WRITTEN);
    return result;
}

std::string generateRandomString(std::size_t size)
{
    return string::convertByteVectorToHexString(generateRandom(size), std::string(), std::string());
}

std::string generatePseudoRandomString(std::size_t size)
{
    return string::convertByteVectorToHexString(generatePseudoRandom(size), std::string(), std::string());
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

