/**
 * @file   HttpReader.cpp
 * @brief  HttpReader class implementation.
 * @author zer0
 * @date   2017-10-03
 */

#include <libtbag/network/http/HttpReader.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/http/ws/WsUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpReader::HttpReader(bool use_websocket) : HttpReader(ws::generateRandomWebSocketKey(), use_websocket)
{
    // EMPTY.
}

HttpReader::HttpReader(std::string const & key, bool use_websocket)
        : _enable_websocket(use_websocket), _switching_protocol(false), _key(key)
{
    // EMPTY.
}

HttpReader::~HttpReader()
{
    // EMPTY.
}

Err HttpReader::parse(char const * buffer, std::size_t size, void * arg)
{
    if (buffer == nullptr || size == 0) {
        onParseError(Err::E_ILLARGS, arg);
        return Err::E_ILLARGS;
    }

    if (_enable_websocket && _switching_protocol) {
        _frame_buffer.push(buffer, size);

        int frame_count = 0;
        Err next_code = Err::E_UNKNOWN;
        while (_frame_buffer.next(&next_code)) {
            ++frame_count;
            onWsMessage(_frame_buffer.getOpCode(), _frame_buffer.atPayload(), arg);
        }

        if (next_code == Err::E_CONTINUE) {
            onContinue(arg);
        } else if (next_code != Err::E_SUCCESS) {
            onParseError(next_code, arg);
        } else {
            assert(frame_count >= 1);
        }
        return next_code;
    }

    std::size_t read_size = 0;
    ParserType direction = ParserType::BOTH;
    Err const EXEC_CODE = execute(buffer, size, &read_size, &direction);

    if (TBAG_ERR_FAILURE(EXEC_CODE)) {
        tDLogE("HttpReader::parse() Execute {} error", getErrName(EXEC_CODE));
        onParseError(EXEC_CODE, arg);
        return EXEC_CODE;
    }

    if (isFinish() == false) {
        onContinue(arg);
        return Err::E_CONTINUE;
    }

    if (direction == ParserType::REQUEST) {
        // This block can be accessed form the 'Server' object.
        if (_enable_websocket && isUpgrade()) {
            // WebSocket interrupt process (HTTP Request).
            if (testWsRequest(*this)) {
                tDLogI("HttpReader::parse() Switching protocol!");
                _switching_protocol = onSwitchingProtocol(*this, arg);
                return Err::E_SUCCESS;
            } else {
                tDLogW("HttpReader::parse() Unknown WebSocket request. Switches to the regular HTTP protocol.");
            }
        }

        onRegularHttp(*this, arg);
        return Err::E_SUCCESS;

    } else if (direction == ParserType::RESPONSE) {
        // This block can be accessed form the 'Client' object.
        if (_enable_websocket && testWsResponse(*this, _key)) {
            tDLogI("HttpReader::parse() Upgrade complete!");
            _switching_protocol = onSwitchingProtocol(*this, arg);
        } else {
            onRegularHttp(*this, arg);
        }
        return Err::E_SUCCESS;
    }

    onParseError(Err::E_ENOSYS, arg);
    return Err::E_ENOSYS;
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

bool testWsRequest(common::HttpProperty const & request, int test_version)
{
    using namespace common;

    if (request.getMethod() != getHttpMethodName(HttpMethod::M_GET)) {
        return false;
    }
    if (request.exists(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        return false;
    }
    if (request.exists(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        return false;
    }
    if (testWsVersion(request, test_version) == false) {
        return false;
    }

    return true;
}

bool testWsResponse(common::HttpProperty const & response, std::string const & key)
{
    using namespace common;

    if (response.getStatusCode() != getHttpStatusNumber(HttpStatus::SC_SWITCHING_PROTOCOLS)) {
        return false;
    }

    if (response.exists(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        return false;
    }

    if (response.exists(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        return false;
    }

    if (response.exists(HEADER_SEC_WEBSOCKET_ACCEPT) == false) {
        return false;
    }

    std::string const ACCEPT_KEY = response.get(HEADER_SEC_WEBSOCKET_ACCEPT);
    if (ACCEPT_KEY != ws::getUpgradeWebSocketKey(key)) {
        return false;
    }

    return true;
}

bool testWsVersion(common::HttpProperty const & property, int test_version)
{
    for (auto & ver : common::getWsVersions(property)) {
        if (ver == test_version) {
            return true;
        }
    }
    return false;
}

Err updateWsRequest(common::HttpProperty & request, std::string const & key)
{
    using namespace common;

    request.setVersion(1, 1);
    if (request.getMethod().empty()) {
        request.setMethod(HttpMethodGET::getMethod());
    }

    request.insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
    request.insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
    request.insertIfNotExists(HEADER_SEC_WEBSOCKET_VERSION, std::to_string(ws::WEBSOCKET_VERSION_HYBI13));

    if (request.exists(HEADER_SEC_WEBSOCKET_KEY) == false) {
        if (key.empty()) {
            return Err::E_KEYGEN;
        }
        request.insert(HEADER_SEC_WEBSOCKET_KEY, key);
    }
    return Err::E_SUCCESS;
}

Err updateWsResponse(common::HttpProperty & response, std::string const & key)
{
    using namespace common;

    std::string const UPGRADE_KEY = ws::getUpgradeWebSocketKey(key);
    if (UPGRADE_KEY.empty()) {
        return Err::E_ILLARGS;
    }

    response.setVersion(1, 1);
    response.setHttpStatus(HttpStatus::SC_SWITCHING_PROTOCOLS);
    response.insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
    response.insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
    response.insertIfNotExists(HEADER_SEC_WEBSOCKET_ACCEPT, UPGRADE_KEY);

    return Err::E_SUCCESS;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

