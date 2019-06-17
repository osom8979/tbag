/**
 * @file   CivetWebExtension.cpp
 * @brief  CivetWebExtension class implementation.
 * @author zer0
 * @date   2019-06-17
 */

#include <libtbag/http/CivetWebExtension.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

int mg_write_string(mg_connection * conn, std::string const & text)
{
    return mg_write(conn, text.c_str(), text.size());
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

