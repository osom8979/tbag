/**
 * @file   StackTrace.cpp
 * @brief  StackTrace class implementation.
 * @author zer0
 * @date   2016-12-25
 */

#include <libtbag/debug/StackTrace.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

std::string getStackTrace()
{
//    std::stringstream ss;
//    ss << boost::stacktrace::stacktrace();
//    return ss.str();
    return std::string();
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

