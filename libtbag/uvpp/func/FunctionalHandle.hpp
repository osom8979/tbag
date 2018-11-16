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
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/Handle.hpp>

#include <functional>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

template <typename UvType>
struct FunctionalHandle : public UvType
{
    using OnClose = std::function<void(void)>;
    using OnWalk  = std::function<void(void*)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Handle, UvType);

    OnClose close_cb;
    OnWalk  walk_cb;

    template <typename ... Args>
    FunctionalHandle(Args && ... args) : UvType(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalHandle()
    { /* EMPTY. */ }

    virtual void onClose() override
    {
        if (close_cb) {
            close_cb();
        } else {
            UvType::onClose();
        }
    }

    virtual void onWalk(void * arg) override
    {
        if (walk_cb) {
            walk_cb(arg);
        } else {
            UvType::onWalk(arg);
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALHANDLE_HPP__

