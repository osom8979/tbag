/**
 * @file   FuncPrepare.hpp
 * @brief  FuncPrepare class prototype.
 * @author zer0
 * @date   2017-05-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCPREPARE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCPREPARE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/uvpp/Prepare.hpp>
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
 * FuncPrepare class prototype.
 *
 * @author zer0
 * @date   2017-05-30
 */
template <typename PrepareType, typename MutexType = lock::FakeLock>
class FuncPrepare : public PrepareType
{
public:
    using Parent = PrepareType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Prepare, Parent);

public:
    using OnClose   = std::function<void(void)>;
    using OnPrepare = std::function<void(void)>;

public:
    mutable Mutex _mutex;

private:
    OnClose   _close_cb;
    OnPrepare _prepare_cb;

public:
    FuncPrepare(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FuncPrepare()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline void setOnClose  (OnClose   const & cb) { Guard g(_mutex); _close_cb   = cb; }
    inline void setOnPrepare(OnPrepare const & cb) { Guard g(_mutex); _prepare_cb = cb; }
    // @formatter:on

public:
    virtual void onClose() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_close_cb)) {
            _close_cb();
        }
    }

    virtual void onPrepare() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_prepare_cb)) {
            _prepare_cb();
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCPREPARE_HPP__

