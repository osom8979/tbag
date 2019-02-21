/**
 * @file   PlDevice.hpp
 * @brief  PlDevice class prototype.
 * @author zer0
 * @date   2019-02-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PLDEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PLDEVICE_HPP__

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

namespace parallel {

// Forward declaration.

/**
 * PlDevice class prototype.
 *
 * @author zer0
 * @date   2019-02-21
 */
class TBAG_API PlDevice : private Noncopyable
{
private:
    // Insert member variables.

public:
    PlDevice();
    virtual ~PlDevice();
};

} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PLDEVICE_HPP__

