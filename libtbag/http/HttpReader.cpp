/**
 * @file   HttpReader.cpp
 * @brief  HttpReader class implementation.
 * @author zer0
 * @date   2017-10-03
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */

#include <libtbag/http/HttpReader.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

HttpReader::HttpReader(bool use_websocket) : HttpReader(generateRandomWebSocketKey(), use_websocket)
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

        if (next_code == Err::E_SMALLBUF || next_code == Err::E_CONTINUE) {
            onContinue(arg);
        } else if (next_code != Err::E_SUCCESS) {
            onParseError(next_code, arg);
        } else {
            assert(next_code == Err::E_SUCCESS);
        }
        return next_code;
    }

    std::size_t read_size = 0;
    ParserType direction = ParserType::BOTH;
    Err const EXEC_CODE = execute(buffer, size, &read_size, &direction);

    if (isFailure(EXEC_CODE)) {
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
            if (checkWsRequest(property())) {
                tDLogI("HttpReader::parse() Switching protocol!");
                _switching_protocol = onSwitchingProtocol(property(), arg);
                return Err::E_SUCCESS;
            } else {
                tDLogW("HttpReader::parse() Unknown WebSocket request. Switches to the regular HTTP protocol.");
            }
        }

        onRegularHttp(property(), arg);
        return Err::E_SUCCESS;

    } else if (direction == ParserType::RESPONSE) {
        // This block can be accessed form the 'Client' object.
        if (_enable_websocket && checkWsResponse(property(), _key)) {
            tDLogI("HttpReader::parse() Upgrade complete!");
            _switching_protocol = onSwitchingProtocol(property(), arg);
        } else {
            onRegularHttp(property(), arg);
        }
        return Err::E_SUCCESS;
    }

    onParseError(Err::E_ENOSYS, arg);
    return Err::E_ENOSYS;
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

