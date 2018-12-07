/**
 * @file   FunctionalFsEvent.hpp
 * @brief  FunctionalFsEvent class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALFSEVENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALFSEVENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/FsEvent.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalFsEvent class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
template <typename CheckType>
struct FunctionalFsEvent : public FunctionalHandle<CheckType>
{
    using Parent    = FunctionalHandle<CheckType>;
    using Event     = typename Parent::Event;
    using OnFsEvent = std::function<void(char const *, Event, Err)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::FsEvent, Parent);
    STATIC_ASSERT_CHECK_IS_SAME(libtbag::uvpp::FsEvent::Event, Event);

    OnFsEvent fs_event_cb;

    template <typename ... Args>
    FunctionalFsEvent(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalFsEvent()
    { /* EMPTY. */ }

    virtual void onFsEvent(char const * filename, Event events, Err status) override
    {
        if (fs_event_cb) {
            fs_event_cb(filename, events, status);
        } else {
            Parent::onFsEvent(filename, events, status);
        }
    }
};

/**
 * FuncFsEvent typedef.
 *
 * @author zer0
 * @date   2018-12-06
 */
using FuncFsEvent = FunctionalFsEvent<libtbag::uvpp::FsEvent>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALFSEVENT_HPP__

