/**
 * @file   Md5.cpp
 * @brief  Md5 class implementation.
 * @author zer0
 * @date   2017-08-15
 */

#include <libtbag/encrypt/Md5.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/md5.h>

#include <cctype>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

bool encryptMd5(char const * input, std::size_t size, Md5Hash & output)
{
    static_assert(MD5_DIGEST_LENGTH == libtbag::encrypt::MD5_DIGEST_BYTE, "MD5 must be 16 digits.");

    MD5_CTX context = {0,};
    if (MD5_Init(&context) != 1) {
        return false;
    }
    if (MD5_Update(&context, input, size) != 1) {
        return false;
    }
    if (MD5_Final(&output[0], &context) != 1) {
        return false;
    }
    return true;
}

bool encryptMd5(util::Buffer const & input, Md5Hash & output)
{
    return encryptMd5(input.data(), input.size(), output);
}

bool encryptMd5(util::Buffer const & input, util::Buffer & output)
{
    Md5Hash md5;
    if (encryptMd5(input, md5) == false) {
        return false;
    }
    output.assign(md5.begin(), md5.end());
    return true;
}

bool encryptMd5(util::Buffer const & input, std::string & output)
{
    Md5Hash md5;
    if (encryptMd5(input, md5) == false) {
        return false;
    }
    std::vector<uint8_t> hex_input;
    hex_input.assign(md5.begin(), md5.end());
    output = string::convertByteVectorToHexString(hex_input, std::string(), std::string());
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);
    return true;
}

bool encryptMd5(std::string const & input, Md5Hash & output)
{
    return encryptMd5(input.data(), input.size(), output);
}

bool encryptMd5(std::string const & input, util::Buffer & output)
{
    return encryptMd5(util::Buffer(input.begin(), input.end()), output);
}

bool encryptMd5(std::string const & input, std::string & output)
{
    return encryptMd5(util::Buffer(input.begin(), input.end()), output);
}

std::string getMd5(util::Buffer const & input)
{
    std::string md5;
    if (encryptMd5(input, md5)) {
        return md5;
    }
    return std::string();
}

std::string getMd5(std::string const & input)
{
    std::string md5;
    if (encryptMd5(input, md5)) {
        return md5;
    }
    return std::string();
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

