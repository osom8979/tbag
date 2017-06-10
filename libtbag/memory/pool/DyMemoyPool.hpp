/**
 * @file   DyMemoyPool.hpp
 * @brief  DyMemoyPool class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_DYMEMOYPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_DYMEMOYPOOL_HPP__

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

namespace memory {
namespace pool   {

/**
 * DyMemoyPool class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 */
class TBAG_API DyMemoyPool : private Noncopyable
{
private:
    // Insert member variables.

public:
    DyMemoyPool();
    virtual ~DyMemoyPool();
};

} // namespace pool
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_DYMEMOYPOOL_HPP__

