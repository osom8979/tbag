/**
 * @file   TpotRequest.cpp
 * @brief  TpotRequest class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/client/TpotRequest.hpp>
#include <libtbag/tpot/structure/TpotProtocol.hpp>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/log/Log.hpp>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

int runTpotRequestWithInteractiveMode()
{
    using namespace libtbag::network::http;
    using namespace libtbag::tpot::structure;

    char const * const TEST_REQUEST_JSON = R"({
"file": "${EXE_DIR}/tbproc",
"args": ["file", "${TITLE}"]
})";

    Exec obj;
    if (obj.fromRequestJsonString(TEST_REQUEST_JSON) != Err::E_SUCCESS) {
        std::cout << "Json parsing error.\n";
        return EXIT_FAILURE;
    }

    std::string json;
    if (obj.toRequestJsonString(json) != Err::E_SUCCESS) {
        std::cout << "Json convert error.\n";
        return EXIT_FAILURE;
    }

    HttpRequest request;
    request.insertHeader(obj.getAcceptKey(), obj.getAcceptValue());
    request.method = obj.getMethod();
    request.body = json;
    std::string const URI = std::string("http://localhost:2100") + obj.getPath();

    HttpResponse response;
    if (requestWithSync(URI, request, 1000, response) != Err::E_SUCCESS) {
        std::cout << "requestWithSync error.\n";
        return EXIT_FAILURE;
    }

    std::cout << "> Status code: " << response.status << std::endl;
    std::cout << "> Body: " << response.body << std::endl;
    return EXIT_SUCCESS;
}

} // namespace client
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

