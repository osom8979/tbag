/**
 * @file   FunctionalHandle.hpp
 * @brief  FunctionalHandle class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALHANDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALHANDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <functional>
#include <mutex>

#ifndef TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT
#define TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(guard_type, mutex) \
    public:                                         \
        using OnClose = std::function<void(void)>;  \
        using OnWalk  = std::function<void(void*)>; \
    private:                                        \
        OnClose  _close_cb;                         \
        OnWalk    _walk_cb;                         \
    public:                                         \
        inline void setOnClose(OnClose const & cb)  \
        { guard_type g(mutex); _close_cb = cb; }    \
        inline void setOnWalk(OnWalk const & cb)    \
        { guard_type g(mutex); _walk_cb = cb; }     \
    public:                                         \
        virtual void onClose() override             \
        {                                           \
            guard_type g(mutex);                    \
            if (static_cast<bool>(_close_cb)) {     \
                _close_cb();                        \
            }                                       \
        }                                           \
        virtual void onWalk(void * arg) override    \
        {                                           \
            guard_type g(mutex);                    \
            if (static_cast<bool>(_walk_cb)) {      \
                _walk_cb(arg);                      \
            }                                       \
        }                                           \
    /* END */
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

// EMPTY.

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALHANDLE_HPP__

