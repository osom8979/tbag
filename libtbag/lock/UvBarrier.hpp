/**
 * @file   UvBarrier.hpp
 * @brief  UvBarrier class prototype.
 * @author zer0
 * @date   2017-05-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVBARRIER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVBARRIER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

/**
 * UvBarrier class prototype.
 *
 * @author zer0
 * @date   2017-05-18
 */
class TBAG_API UvBarrier : private Noncopyable
{
private:
    int const COUNT;
    void * _handle;

public:
    UvBarrier(int count);
    virtual ~UvBarrier();

public:
    inline int getCount() const TBAG_NOEXCEPT
    { return COUNT; }
    inline void * getHandle() TBAG_NOEXCEPT
    { return _handle; }

public:
    /**
     * @return
     *  value > 0 to an arbitrarily chosen “serializer” thread to facilitate cleanup, i.e. @n
     *  <code>
     *   if (uv_barrier_wait(&barrier) > 0) {
     *       uv_barrier_destroy(&barrier);
     *   }
     *  </code>
     */
    int wait();
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVBARRIER_HPP__

