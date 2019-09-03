/**
 * @file   CivetWebExtension.cpp
 * @brief  CivetWebExtension class implementation.
 * @author zer0
 * @date   2019-06-17
 */

#include <libtbag/http/CivetWebExtension.hpp>

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

int mg_write_string(mg_connection * conn, std::string const & text)
{
    return mg_write(conn, text.c_str(), text.size());
}

TBAG_CONSTEXPR static std::size_t TEMP_BUFFER_SIZE = (4*1024);

Json::Value getJsonBody(mg_connection * conn)
{
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

    return libtbag::dom::json::getJsonValue(ss.str());
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

