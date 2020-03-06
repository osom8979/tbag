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

libtbag::util::Buffer generateRandom(std::size_t size)
{
    libtbag::util::Buffer result(size);
    auto const written = RAND_bytes((unsigned char *)result.data(), result.size());
    RAND_seed(result.data(), written);
    return result;
}

libtbag::util::Buffer generatePseudoRandom(std::size_t size)
{
    libtbag::util::Buffer result(size);
    auto const written = RAND_pseudo_bytes((unsigned char *)result.data(), result.size());
    RAND_seed(result.data(), written);
    return result;
}

std::string generateRandomString(std::size_t size)
{
    using namespace libtbag::string;
    return convertByteVectorToHexString(generateRandom(size), STRING_EMPTY, STRING_EMPTY).substr(0, size);
}

std::string generatePseudoRandomString(std::size_t size)
{
    using namespace libtbag::string;
    return convertByteVectorToHexString(generatePseudoRandom(size), STRING_EMPTY, STRING_EMPTY).substr(0, size);
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

