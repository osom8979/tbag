/**
 * @file   FunctionalThread.hpp
 * @brief  FunctionalThread class prototype.
 * @author zer0
 * @date   2019-07-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_FUNCTIONALTHREAD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_FUNCTIONALTHREAD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/thread/Thread.hpp>
#include <functional>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * FunctionalThread class prototype.
 *
 * @author zer0
 * @date   2019-07-17
 */
class TBAG_API FunctionalThread : public Thread
{
public:
    using Callback = std::function<void(void)>;

private:
    Callback _cb;

public:
    FunctionalThread();
    FunctionalThread(Callback const & cb);
    FunctionalThread(Callback && cb) TBAG_NOEXCEPT;
    virtual ~FunctionalThread();

public:
    void setCallback(Callback const & cb);
    void setCallback(Callback && cb) TBAG_NOEXCEPT;

protected:
    virtual void onRunner() override;
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_FUNCTIONALTHREAD_HPP__

