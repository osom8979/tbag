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

bool encryptMd5(std::string const & input, Md5Hash & output)
{
    static_assert(MD5_DIGEST_LENGTH == libtbag::encrypt::MD5_DIGEST_BYTE, "MD5 must be 16 digits.");
    MD5_CTX context;
    MD5_Init(&context);
    MD5_Update(&context, input.c_str(), input.size());
    MD5_Final(&output[0], &context);
    return true;
}

bool encryptMd5(std::string const & input, std::string & output)
{
    Md5Hash md5;
    if (encryptMd5(input, md5) == false) {
        return false;
    }

    std::vector<uint8_t> hex_input;
    hex_input.assign(md5.begin(), md5.end());
    output = string::convertByteArrayToHexString(hex_input, "");
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);

    return true;
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
