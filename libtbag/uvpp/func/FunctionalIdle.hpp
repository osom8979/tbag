/**
 * @file   FunctionalIdle.hpp
 * @brief  FunctionalIdle class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALIDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALIDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Idle.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <functional>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalIdle class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename IdleType, typename MutexType = lock::FakeLock>
class FunctionalIdle : public IdleType
{
public:
    using Parent = IdleType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Idle, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnIdle = std::function<void(void)>;

private:
    Mutex _mutex;
    OnIdle _idle_cb;

public:
    template <typename ... Args>
    FunctionalIdle(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalIdle()
    { /* EMPTY. */ }

public:
    inline void setOnIdle(OnIdle const & cb)
    {
        Guard guard(_mutex);
        _idle_cb = cb;
    }

public:
    virtual void onIdle() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_idle_cb)) {
            _idle_cb();
        }
    }
};

/**
 * FuncIdle typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncIdle = FunctionalIdle<libtbag::uvpp::Idle>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALIDLE_HPP__

