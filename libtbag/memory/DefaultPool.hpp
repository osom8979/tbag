/**
 * @file   DefaultPool.hpp
 * @brief  DefaultPool class prototype.
 * @author zer0
 * @date   2016-11-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_DEFAULTPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_DEFAULTPOOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// TODO: include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

/**
 * DefaultPool class prototype.
 *
 * @author zer0
 * @date   2016-11-23
 */
class TBAG_API DefaultPool : public Noncopyable
{
public:
    DefaultPool();
    virtual ~DefaultPool();
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_DEFAULTPOOL_HPP__

