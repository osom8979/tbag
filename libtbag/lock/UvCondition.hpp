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
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

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
 * Example
 * @code{.cpp}
 *  Lock lock;
 *  Condition signal;
 *  bool task_end = false;
 *  auto const run_async_result = run_thread([&](){
 *
 *      run_tack_callback(); // RUN ASYNC TASK !!
 *
 *      lock.lock();
 *      task_end = true;
 *      signal.signal();
 *      lock.unlock();
 *  });
 *  if (run_async_result) {
 *      lock.lock();
 *      while (task_end == false) {
 *          signal.wait(lock);
 *      }
 *      lock.unlock();
 *  }
 *  return run_async_result;
 * @endcode
 *
 * @author zer0
 * @date   2016-10-12
 */
class TBAG_API UvCondition : private Noncopyable
{
private:
    void * _handle;

public:
    UvCondition();
    ~UvCondition();

public:
    inline void * getHandle() TBAG_NOEXCEPT
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

