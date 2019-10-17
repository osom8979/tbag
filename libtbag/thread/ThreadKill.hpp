/**
 * @file   ThreadKill.hpp
 * @brief  ThreadKill class prototype.
 * @author zer0
 * @date   2019-10-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADKILL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADKILL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

TBAG_API Err killThread(libtbag::uvpp::uthread id);

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADKILL_HPP__

