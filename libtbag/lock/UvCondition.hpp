/**
 * @file   UvCondition.hpp
 * @brief  UvCondition class prototype.
 * @author zer0
 * @date   2016-10-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVCONDITION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVCONDITION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

// Forward declaration.
class UvLock;

/**
 * UvCondition class prototype.
 *
 * @author zer0
 * @date   2016-10-12
 */
class UvCondition : public Noncopyable
{
private:
    void * _handle;

public:
    UvCondition() throw (InitializeException);
    virtual ~UvCondition();

public:
    inline void * getHandle() noexcept
    { return _handle; }

public:
    void wait(UvLock & lock);
    void wait(UvLock & lock, int64_t timeout_nano);

public:
    void signal();
    void broadcast();
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVCONDITION_HPP__

