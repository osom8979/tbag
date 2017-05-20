/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2017-05-20
 */

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

Err requestWithSync(Uri const & uri, uint64_t timeout, HttpParser & response)
{
    using Loop = uvpp::Loop;
    Loop loop;
    TcpHttpClient http(loop);

//    Err result = Err::E_UNKNOWN;
//
//    uri.isPort()
//
//    if (http.init("osom8979.github.io", 80, 10000) == false) {
//        std::cout << "Network unreachable.\n";
//        return;
//    }
//
//    auto builder = HttpBuilder(1, 1).setMethod("GET").setUri("/")
//            .insertHeader("Host", "osom8979.github.io")
//            .insertHeader("User-Agent", "curl/7.51.0")
//            .insertHeader("Accept", "*/*");
//
//    http.setup(builder, [&](Err code, HttpParser const & response){
//        result = code;
//        status = response.getStatusCode();
//        std::cout << response.getBody() << std::endl;
//    }, TcpHttpClient::Millisec(10000));
//
//    ASSERT_EQ(Err::E_SUCCESS, loop.run());
//    ASSERT_EQ(Err::E_SUCCESS, result);
//    ASSERT_EQ(200, status);

    return Err::E_SUCCESS;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

