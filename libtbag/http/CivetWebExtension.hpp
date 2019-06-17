/**
 * @file   CivetWebExtension.hpp
 * @brief  CivetWebExtension class prototype.
 * @author zer0
 * @date   2019-06-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBEXTENSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBEXTENSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/http/CivetWebBypass.hpp>
#include <libtbag/string/Format.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

TBAG_API int mg_write_string(mg_connection * conn, std::string const & text);

template <typename ... Args>
inline int mg_write_format(mg_connection * conn, std::string const & format, Args && ... args)
{
    return mg_write_string(conn, libtbag::string::fformat(format, std::forward<Args>(args) ...));
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBEXTENSION_HPP__

