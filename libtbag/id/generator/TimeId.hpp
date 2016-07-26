/**
 * @file   TimeId.hpp
 * @brief  TimeId class prototype.
 * @author zer0
 * @date   2016-07-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ID_GENERATOR_TIMEID_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ID_GENERATOR_TIMEID_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/id/Id.hpp>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id        {
namespace generator {

inline Id genTimeId()
{
    return static_cast<Id>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

} // namespace generator
} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ID_GENERATOR_TIMEID_HPP__

