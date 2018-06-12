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

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <utility>
#include <new>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pattern {

/**
 * Singleton property template class.
 *
 * @author zer0
 * @date   2017-02-16
 */
template <typename T>
class Singleton2 : private Noncopyable
{
public:
    using BaseType = T;

private:
    static BaseType * __instance;

protected:
    TBAG_CONSTEXPR Singleton2() { /* EMPTY. */ }
    ~Singleton2() { /* EMPTY. */ }

public:
    inline static bool exists() TBAG_NOEXCEPT
    {
        return __instance != nullptr;
    }

    template <typename ... Args>
    inline static void createInstance(Args && ... args) TBAG_NOEXCEPT
    {
        if (!exists()) {
            __instance = new (std::nothrow) BaseType(std::forward<Args>(args) ...);
        }
    }

    inline static void releaseInstance() TBAG_NOEXCEPT
    {
        if (exists()) {
            delete __instance;
            __instance = nullptr;
        }
    }

    inline static BaseType * getInstanceOfNoSafe() TBAG_NOEXCEPT
    {
        return __instance;
    }

    template <typename ... Args>
    inline static BaseType * getInstance(Args && ... args) TBAG_NOEXCEPT
    {
        //if (!exists()) {
        createInstance(std::forward<Args>(args) ...);
        //}
        return getInstanceOfNoSafe();
    }
};

template <typename T>
typename Singleton2<T>::BaseType * Singleton2<T>::__instance = nullptr;

} // namespace pattern

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef SINGLETON2_PROTOTYPE
#define SINGLETON2_PROTOTYPE(class_name)         \
    public:                                      \
        friend class ::libtbag::pattern::Singleton2<class_name>; \
    protected:                                   \
        class_name() { /* EMPTY. */ }            \
    public:                                      \
        virtual ~class_name() { /* EMPTY. */ }   \
    private: // default access modifier of class.
#endif // SINGLETON2_PROTOTYPE

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON2_HPP__

