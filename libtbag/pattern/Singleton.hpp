/**
 * @file   Singleton.hpp
 * @brief  Singleton class prototype.
 * @author zer0
 * @date   2016-04-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <atomic>
#include <mutex>

#ifndef SINGLETON_PROTOTYPE
#define SINGLETON_PROTOTYPE(base_class)             \
    protected:                                      \
        base_class();                               \
    public:                                         \
        ~base_class();                              \
    private:                                        \
        static std::atomic<base_class*> __instance; \
        static std::mutex __instance_lock;          \
    public:                                         \
        static base_class * getInstance();
#endif

#ifndef SINGLETON_IMPLEMENT
#define SINGLETON_IMPLEMENT(base_class)                             \
        std::atomic<base_class*> base_class::__instance(nullptr);   \
        std::mutex base_class::__instance_lock;                     \
                                                                    \
        base_class * base_class::getInstance()                      \
        {                                                           \
            base_class * temp = __instance.load();                  \
            if (temp == nullptr) {                                  \
                std::lock_guard<std::mutex> guard(__instance_lock); \
                temp = __instance.load();                           \
                if (temp == nullptr) {                              \
                    temp = new base_class();                        \
                    __instance.store(temp);                         \
                }                                                   \
            }                                                       \
            return temp;                                            \
        }
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__

