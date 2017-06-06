/**
 * @file   Singleton3.hpp
 * @brief  Singleton3 class prototype.
 * @author zer0
 * @date   2017-06-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON3_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON3_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <new>

#ifndef SINGLETON3_INSTANCE
#define SINGLETON3_INSTANCE(type) \
    private:                                                                        \
        static type * __instance;                                                   \
    public:                                                                         \
        inline static void createInstance() TBAG_NOEXCEPT                           \
        { if (__instance == nullptr) { __instance = new (std::nothrow) type(); } }  \
        inline static void releaseInstance() TBAG_NOEXCEPT                          \
        { if (__instance != nullptr) { delete __instance; __instance = nullptr; } } \
        inline static type * getInstance() TBAG_NOEXCEPT                            \
        { if (__instance == nullptr) { createInstance(); } return __instance; }     \
    private:                                                                        \
    /* -- END -- */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON3_HPP__

