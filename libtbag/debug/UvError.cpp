/**
 * @file   UvError.cpp
 * @brief  UvError helper method implementation.
 * @author zer0
 * @date   2016-07-15
 */

#include <libtbag/debug/UvError.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

std::string getUvErrorString(int uv_error_code)
{
    return std::string(uv_strerror(uv_error_code));
}

std::string getUvErrorName(int uv_error_code)
{
    return std::string(uv_err_name(uv_error_code));
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

