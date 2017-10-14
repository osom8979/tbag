/**
 * @file   HttpProperty.cpp
 * @brief  HttpProperty class implementation.
 * @author zer0
 * @date   2017-09-30
 */

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ------------------------
// Miscellaneous utilities.
// ------------------------

std::vector<int> getWsVersions(HttpHeader const & header)
{
    std::vector<int> result;
    auto const VERSIONS = header.getIgnoreCase(HEADER_SEC_WEBSOCKET_VERSION);
    for (auto & ver : string::splitTokens(VERSIONS, DEFAULT_VALUE_DELIMITER)) {
        try {
            result.push_back(std::stoi(string::trim(ver)));
        } catch (...) {
            // SKIP.
        }
    }
    return result;
}

bool testWsVersion(HttpHeader const & header, int test_version)
{
    for (auto & ver : getWsVersions(header)) {
        if (ver == test_version) {
            return true;
        }
    }
    return false;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

