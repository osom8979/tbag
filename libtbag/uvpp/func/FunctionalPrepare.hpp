/**
 * @file   FunctionalPrepare.hpp
 * @brief  FunctionalPrepare class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPREPARE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPREPARE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
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
 * FunctionalPrepare class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename PrepareType, typename MutexType = lock::FakeLock>
class FunctionalPrepare : public PrepareType
{
public:
    using Parent = PrepareType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Prepare, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnPrepare = std::function<void(void)>;

private:
    Mutex _mutex;
    OnPrepare _prepare_cb;

public:
    template <typename ... Args>
    FunctionalPrepare(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalPrepare()
    { /* EMPTY. */ }

public:
    void setOnPrepare(OnPrepare const & cb)
    {
        Guard guard(_mutex);
        _prepare_cb = cb;
    }

public:
    virtual void onPrepare() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_prepare_cb)) {
            _prepare_cb();
        }
    }
};

/**
 * FuncPrepare typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncPrepare = FunctionalPrepare<libtbag::uvpp::Prepare>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALPREPARE_HPP__

