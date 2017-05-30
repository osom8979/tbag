/**
 * @file   FuncAsync.hpp
 * @brief  FuncAsync class prototype.
 * @author zer0
 * @date   2017-05-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
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
 * FuncAsync class prototype.
 *
 * @author zer0
 * @date   2017-05-30
 */
template <typename AsyncType, typename MutexType = lock::FakeLock>
class FuncAsync : public AsyncType
{
public:
    using Parent = AsyncType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Async, Parent);

public:
    using OnClose = std::function<void(void)>;
    using OnAsync = std::function<void(void)>;

public:
    mutable Mutex _mutex;

private:
    OnClose _close_cb;
    OnAsync _async_cb;

public:
    FuncAsync(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FuncAsync()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline void setOnClose(OnClose const & cb) { Guard g(_mutex); _close_cb = cb; }
    inline void setOnAsync(OnAsync const & cb) { Guard g(_mutex); _async_cb = cb; }
    // @formatter:on

public:
    virtual void onClose() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_close_cb)) {
            _close_cb();
        }
    }

    virtual void onAsync() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_async_cb)) {
            _async_cb();
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCASYNC_HPP__

