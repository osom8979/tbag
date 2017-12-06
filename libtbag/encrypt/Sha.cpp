/**
 * @file   Sha.cpp
 * @brief  Sha class implementation.
 * @author zer0
 * @date   2017-04-11
 * @date   2017-12-06 (Rename: Sha1 -> Sha)
 */

#include <libtbag/encrypt/Sha.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cctype>
#include <algorithm>

#include <openssl/sha.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

bool encryptSha1(std::string const & input, Sha1Hash & output)
{
    Sha1Hash sha1;
    SHA1((unsigned char *)&input[0], input.size(), output.data());
    return true;
}

bool encryptSha1(std::string const & input, std::string & output)
{
    Sha1Hash sha1;
    encryptSha1(input, sha1);

    std::vector<uint8_t> hex_input;
    hex_input.assign(sha1.begin(), sha1.end());
    output = string::convertByteVectorToHexString(hex_input, "");
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);

    return true;
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

