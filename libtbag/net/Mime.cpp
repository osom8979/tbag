/**
 * @file   Mime.cpp
 * @brief  Mime class implementation.
 * @author zer0
 * @date   2020-05-11
 */

#include <libtbag/net/Mime.hpp>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

std::string Mime::toString() const
{
    std::stringstream ss;
    if (type.empty()) {
        return {};
    }
    ss << type;
    if (!subtype.empty()) {
        ss << TYPE_DELIMITER << subtype;
    }
    if (!parameter.empty()) {
        ss << PARAMETER_DELIMITER << parameter;
    }
    return ss.str();
}

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

