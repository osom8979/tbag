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
#include <libtbag/preprocessor/details/Empty.hpp>
#include <libtbag/preprocessor/details/ExpendParams.hpp>

#include <functional>

#ifndef TBAG_CALLBACK_HELPER_IMPL
#define TBAG_CALLBACK_HELPER_IMPL(callback_name, return_keyword, return_type, return_default_value, ...)    \
    public:                                                                                                 \
        using callback_name##_func = std::function<return_type(TBAG_PP_EXPEND_TYPES(__VA_ARGS__))>;         \
    private:                                                                                                \
        callback_name##_func __##callback_name##_cb;                                                        \
    public:                                                                                                 \
        inline void set_##callback_name(callback_name##_func const & cb)                                    \
        {                                                                                                   \
            __##callback_name##_cb = cb;                                                                    \
        }                                                                                                   \
    protected:                                                                                              \
        virtual return_type callback_name(TBAG_PP_EXPEND_PARAMS(__VA_ARGS__)) override                      \
        {                                                                                                   \
            if (static_cast<bool>(__##callback_name##_cb)) {                                                \
                return_keyword __##callback_name##_cb(TBAG_PP_EXPEND_VALUES(__VA_ARGS__));                  \
            }                                                                                               \
            return_keyword return_default_value                                                             \
        }                                                                                                   \
    private:
#endif

#ifndef TBAG_CALLBACK_HELPER
#define TBAG_CALLBACK_HELPER(callback_name, return_type, return_default_value, ...) \
    TBAG_CALLBACK_HELPER_IMPL(callback_name, return, return_type, return_default_value, __VA_ARGS__)
#endif

#ifndef TBAG_VOID_CALLBACK_HELPER
#define TBAG_VOID_CALLBACK_HELPER(callback_name, ...) \
    TBAG_CALLBACK_HELPER_IMPL(callback_name, TBAG_PP_EMPTY(), void, TBAG_PP_EMPTY(), __VA_ARGS__)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_CALLBACKHELPER_HPP__

