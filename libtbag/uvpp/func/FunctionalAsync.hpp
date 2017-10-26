/**
 * @file   FunctionalAsync.hpp
 * @brief  FunctionalAsync class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Async.hpp>
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
 * FunctionalAsync class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename AsyncType, typename MutexType = lock::FakeLock>
struct FunctionalAsync : public AsyncType
{
public:
    using Parent = AsyncType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Async, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

    using OnAsync = std::function<void(void)>;

private:
    Mutex _mutex;
    OnAsync _async_cb;

public:
    template <typename ... Args>
    FunctionalAsync(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalAsync()
    { /* EMPTY. */ }

public:
    void setOnAsync(OnAsync const & cb)
    {
        Guard guard(_mutex);
        _async_cb = cb;
    }

public:
    virtual void onAsync() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_async_cb)) {
            _async_cb();
        }
    }
};

/**
 * FuncAsync typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncAsync = FunctionalAsync<libtbag::uvpp::Async>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALASYNC_HPP__

