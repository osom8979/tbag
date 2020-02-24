/**
 * @file   DefaultMsg.hpp
 * @brief  DefaultMsg class prototype.
 * @author zer0
 * @date   2020-02-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_DEFAULTMSG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_DEFAULTMSG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <thread>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

TBAG_CONSTEXPR char const * const DEFAULT_MSG_PREFIX_SAMPLE
        = "D YYYY-MM-DDThh:mm:ss.iiicccnnn @0xffffffffffffffff ";
TBAG_CONSTEXPR char const THREAD_PREFIX_CHAR = '@';

template <class CharT, class TraitsT>
void makeDefaultPrefix(
        std::basic_ostream<CharT, TraitsT> & ss,
        int level = libtbag::log::INFO_LEVEL,
        libtbag::time::TimePoint const & tp = libtbag::time::TimePoint::now(),
        decltype(std::this_thread::get_id()) const & thread_id = std::this_thread::get_id())
{
    ss.put(libtbag::log::getShortPrefix(level));
    ss.put(libtbag::string::CHAR_SPACE);
    ss << tp.toLocalLongString();
    ss.put(libtbag::string::CHAR_SPACE);
    ss.put(THREAD_PREFIX_CHAR);
    ss << thread_id;
    ss.put(libtbag::string::CHAR_SPACE);
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_DEFAULTMSG_HPP__

