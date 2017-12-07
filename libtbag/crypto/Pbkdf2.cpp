/**
 * @file   Pbkdf2.cpp
 * @brief  Pbkdf2 class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/crypto/Pbkdf2.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/evp.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

util::Buffer generatePbkdf2(std::string const & password, util::Buffer const & salt, std::size_t size,
                            Pbkdf2Type type, int iteration_count)
{
    EVP_MD const * digest = nullptr;
    switch (type) {
    case Pbkdf2Type::PT_MD_NULL: digest = EVP_md_null(); break;
#ifndef OPENSSL_NO_MD5
    case Pbkdf2Type::PT_MD5:     digest = EVP_md5();     break;
#endif
#ifndef OPENSSL_NO_SHA
    case Pbkdf2Type::PT_SHA1:    digest = EVP_sha1();    break;
#endif
#ifndef OPENSSL_NO_SHA256
    case Pbkdf2Type::PT_SHA224:  digest = EVP_sha224();  break;
    case Pbkdf2Type::PT_SHA256:  digest = EVP_sha256();  break;
#endif
#ifndef OPENSSL_NO_SHA512
    case Pbkdf2Type::PT_SHA384:  digest = EVP_sha384();  break;
    case Pbkdf2Type::PT_SHA512:  digest = EVP_sha512();  break;
#endif
    default:                     digest = EVP_md_null(); break;
    }

    util::Buffer result(size);
    int const CODE = PKCS5_PBKDF2_HMAC(password.data(), (int)password.size(),
                                       (unsigned char const *)salt.data(), (int)salt.size(),
                                       iteration_count, digest, (int)size,
                                       (unsigned char *)result.data());
    if (CODE != 1) {
        return util::Buffer();
    }
    return result;
}

util::Buffer generatePbkdf2(std::string const & password, std::string const & salt, std::size_t size,
                            Pbkdf2Type type, int iteration_count)
{
    util::Buffer const SALT_BUFFER(salt.begin(), salt.end());
    return generatePbkdf2(password, SALT_BUFFER, size, type, iteration_count);
}

std::string generatePbkdf2String(std::string const & password, util::Buffer const & salt, std::size_t size,
                                 Pbkdf2Type type, int iteration_count)
{
    return string::convertByteVectorToHexString(generatePbkdf2(password, salt, size, type, iteration_count),
                                                std::string(), std::string());
}

std::string generatePbkdf2String(std::string const & password, std::string const & salt, std::size_t size,
                                 Pbkdf2Type type, int iteration_count)
{
    return string::convertByteVectorToHexString(generatePbkdf2(password, salt, size, type, iteration_count),
                                                std::string(), std::string());
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

