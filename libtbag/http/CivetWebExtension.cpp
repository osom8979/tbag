/**
 * @file   CivetWebExtension.cpp
 * @brief  CivetWebExtension class implementation.
 * @author zer0
 * @date   2019-06-17
 */

#include <libtbag/http/CivetWebExtension.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

int mg_write_string(mg_connection * conn, std::string const & text)
{
    return mg_write(conn, text.c_str(), text.size());
}

std::string getRequestLine(mg_connection const * conn)
{
    assert(conn != nullptr);
    std::stringstream ss;
    ss << mg_get_request_info(conn)->request_method << SP
       << mg_get_request_info(conn)->request_uri << SP
       << HTTP << "/" << mg_get_request_info(conn)->http_version;
    return ss.str();
}

HttpUniqueHeaders getHeaders(mg_connection const * conn, bool lower_key)
{
    assert(conn != nullptr);

    auto const size = mg_get_request_info(conn)->num_headers;
    auto const * headers = mg_get_request_info(conn)->http_headers;
    assert(headers != nullptr);

    HttpUniqueHeaders result;
    for (auto i = 0; i < size; ++i) {
        std::string name;
        if (headers[i].name != nullptr) {
            if (lower_key) {
                name = libtbag::string::lower(headers[i].name);
            } else {
                name = headers[i].name;
            }
            std::string value;
            if (headers[i].value != nullptr) {
                value = headers[i].value;
            }
            result.emplace(std::move(name), std::move(value));
        }
    }
    return result;
}

TBAG_CONSTEXPR static std::size_t TEMP_BUFFER_SIZE = (4*1024);

std::string getBody(mg_connection * conn)
{
    assert(conn != nullptr);

    auto const content_length = mg_get_request_info(conn)->content_length;
    if (content_length <= 0) {
        return {};
    }

    std::stringstream ss;
    std::size_t total_read_size = 0;
    char buffer[TEMP_BUFFER_SIZE] = {0,};

    while (total_read_size < static_cast<std::size_t>(content_length)) {
        auto remaining_size = static_cast<std::size_t>(content_length) - total_read_size;
        if (remaining_size > TEMP_BUFFER_SIZE) {
            remaining_size = TEMP_BUFFER_SIZE;
        }

        auto const read_size = mg_read(conn, buffer, remaining_size);
        if (read_size <= 0) {
            break;
        }

        ss.write(buffer, read_size);
        total_read_size += read_size;
    }

    return ss.str();
}

Json::Value getJsonBody(mg_connection * conn)
{
    return libtbag::dom::json::getJsonValue(getBody(conn));
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

