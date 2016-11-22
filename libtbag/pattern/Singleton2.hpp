/**
 * @file   Singleton2.hpp
 * @brief  Singleton2 class prototype.
 * @author zer0
 * @date   2016-11-22
 *
 * @warning
 *  Be careful of the initialization sequence.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON2_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//#include <libtbag/config.h>
//#include <libtbag/predef.hpp>

#ifndef SINGLETON2_PROTOTYPE
#define SINGLETON2_PROTOTYPE(class_name)         \
    private:                                     \
        static class_name * __instance;          \
    public:                                      \
        inline static void createInstance()      \
        {                                        \
            if (__instance == nullptr) {         \
                __instance = new (std::nothrow) class_name(); \
            }                                    \
        }                                        \
        inline static void releaseInstance()     \
        {                                        \
            if (__instance != nullptr) {         \
                delete __instance;               \
                __instance = nullptr;            \
            }                                    \
        }                                        \
        inline static class_name * getInstance() \
        {                                        \
            if (__instance == nullptr) {         \
                createInstance();                \
            }                                    \
            return __instance;                   \
        }                                        \
    protected:                                   \
        class_name() { /* EMPTY. */ }            \
    public:                                      \
        ~class_name() { /* EMPTY. */ }           \
    private: // default access modifier of class.
#endif // SINGLETON2_PROTOTYPE

#ifndef SINGLETON2_IMPLEMENT
#define SINGLETON2_IMPLEMENT(class_name) \
    class_name * class_name::__instance = nullptr;
#endif // SINGLETON2_IMPLEMENT

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON2_HPP__

