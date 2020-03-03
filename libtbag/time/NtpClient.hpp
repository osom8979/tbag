/**
 * @file   NtpClient.hpp
 * @brief  NtpClient class prototype.
 * @author zer0
 * @date   2020-03-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_NTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_NTPCLIENT_HPP__

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

namespace time {

// Forward declaration.

/**
 * NtpClient class prototype.
 *
 * @author zer0
 * @date   2020-03-03
 */
class TBAG_API NtpClient : private Noncopyable
{
private:
    // Insert member variables.

public:
    NtpClient();
    virtual ~NtpClient();
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_NTPCLIENT_HPP__

