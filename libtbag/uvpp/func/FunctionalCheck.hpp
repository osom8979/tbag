/**
 * @file   FunctionalCheck.hpp
 * @brief  FunctionalCheck class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALCHECK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALCHECK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Check.hpp>
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
 * FunctionalCheck class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename CheckType, typename MutexType = lock::FakeLock>
class FunctionalCheck : public CheckType
{
public:
    using Parent = CheckType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Check, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnCheck = std::function<void(void)>;

private:
    Mutex _mutex;
    OnCheck _check_cb;

public:
    FunctionalCheck(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FunctionalCheck()
    { /* EMPTY. */ }

public:
    void setOnCheck(OnCheck const & cb)
    {
        Guard guard(_mutex);
        _check_cb = cb;
    }

public:
    virtual void onCheck() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_check_cb)) {
            _check_cb();
        }
    }
};

/**
 * FuncCheck typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncCheck = FunctionalCheck<libtbag::uvpp::Check>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALCHECK_HPP__

