/**
 * @file   HttpClient.hpp
 * @brief  HttpClient class prototype.
 * @author zer0
 * @date   2020-03-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

// Forward declaration.

/**
 * HttpClient class prototype.
 *
 * @author zer0
 * @date   2020-03-10
 */
class TBAG_API HttpClient : private Noncopyable
{
private:
    // Insert member variables.

public:
    HttpClient();
    virtual ~HttpClient();
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

