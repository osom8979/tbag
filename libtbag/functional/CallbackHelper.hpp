/**
 * @file   CallbackHelper.hpp
 * @brief  CallbackHelper macro prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_CALLBACKHELPER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_CALLBACKHELPER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/preprocessor/control/If.hpp>
#include <libtbag/preprocessor/facilities/Empty.hpp>
#include <libtbag/preprocessor/variadic/VariadicIsEmpty.hpp>
#include <libtbag/preprocessor/ExpendParams.hpp>

#include <functional>

#ifndef TBAG_CALLBACK_HELPER_IMPL
#define TBAG_CALLBACK_HELPER_IMPL(name, rkey, rtype, rval, ...)         \
    public:                                                             \
        using name##_func =                                             \
            std::function<rtype(TBAG_PP_EXPEND_TYPES(__VA_ARGS__))>;    \
    private:                                                            \
        name##_func __##name##_cb;                                      \
    public:                                                             \
        inline void set_##name(name##_func const & cb)                  \
        {                                                               \
            __##name##_cb = cb;                                         \
        }                                                               \
    protected:                                                          \
        virtual rtype name(TBAG_PP_EXPEND_PARAMS(__VA_ARGS__)) override \
        {                                                               \
            if (static_cast<bool>(__##name##_cb)) {                     \
                rkey __##name##_cb(TBAG_PP_EXPEND_VALUES(__VA_ARGS__)); \
            }                                                           \
            rkey rval;                                                  \
        }                                                               \
    private:
#endif

#ifndef TBAG_NOPARAM_CALLBACK_HELPER_IMPL
#define TBAG_NOPARAM_CALLBACK_HELPER_IMPL(name, rkey, rtype, rval)  \
    public:                                                         \
        using name##_func = std::function<rtype()>;                 \
    private:                                                        \
        name##_func __##name##_cb;                                  \
    public:                                                         \
        inline void set_##name(name##_func const & cb)              \
        {                                                           \
            __##name##_cb = cb;                                     \
        }                                                           \
    protected:                                                      \
        virtual rtype name() override                               \
        {                                                           \
            if (static_cast<bool>(__##name##_cb)) {                 \
                rkey __##name##_cb();                               \
            }                                                       \
            rkey rval;                                              \
        }                                                           \
    private:
#endif

#define TBAG_CALLBACK_HELPER(name, rkey, rtype, rval, ...)              \
    TBAG_PP_IF(                                                         \
        TBAG_PP_VARIADIC_IS_EMPTY(__VA_ARGS__),                         \
        TBAG_NOPARAM_CALLBACK_HELPER_IMPL(name, rkey, rtype, rval),     \
        TBAG_CALLBACK_HELPER_IMPL(name, rkey, rtype, rval, __VA_ARGS__) \
    )
#define TBAG_RETN_CALLBACK_HELPER(name, rtype, rval, ...)   \
    TBAG_CALLBACK_HELPER(name, return, rtype, rval, __VA_ARGS__)
#define TBAG_VOID_CALLBACK_HELPER(name, ...)                \
    TBAG_CALLBACK_HELPER(name, TBAG_PP_EMPTY(), void, TBAG_PP_EMPTY(), __VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_CALLBACKHELPER_HPP__

