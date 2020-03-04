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
#include <libtbag/ErrPair.hpp>
#include <libtbag/time/TimePoint.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * NtpClient class prototype.
 *
 * @author zer0
 * @date   2020-03-03
 */
class TBAG_API NtpClient : private Noncopyable
{
public:
    using TimeResult = libtbag::ErrPair<TimePoint>;

public:
    /** Difference between Jan 1, 1900 and Jan 1, 1970 */
    TBAG_CONSTEXPR static std::size_t const TIMESTAMP_DELTA = 2208988800ul;
    TBAG_CONSTEXPR static char const * const DEFAULT_SERVER = "us.pool.ntp.org";
    TBAG_CONSTEXPR static int const DEFAULT_PORT = 123;
    TBAG_CONSTEXPR static int const TIMEOUT_MS = 8*1000;

private:
    std::string _server;
    int _port;

public:
    NtpClient(std::string const & server = DEFAULT_SERVER, int port = DEFAULT_PORT);
    virtual ~NtpClient();

public:
    TimeResult request(int timeout_ms = TIMEOUT_MS) const;
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_NTPCLIENT_HPP__

