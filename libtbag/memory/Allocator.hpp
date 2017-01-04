/**
 * @file   Allocator.hpp
 * @brief  Allocator class prototype.
 * @author zer0
 * @date   2017-01-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOCATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOCATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <memory>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

template <typename T>
inline T * addressof(T & val) TBAG_NOEXCEPT
{
    return (T*)&reinterpret_cast<const volatile char&>(val);
}

inline void * allocate(std::size_t size)
{
    return ::operator new(size);
}

inline void deallocate(void * ptr)
{
    ::operator delete(ptr);
}

/**
 * Allocator interface class template prototype.
 *
 * @author zer0
 * @date   2017-01-04
 */
template <typename Type>
struct Allocator
{
public:
    using value_type = Type;
    using pointer       = typename std::add_pointer<value_type>::type;
    using const_pointer = typename std::add_const<pointer>::type*;

    using reference       = typename std::add_lvalue_reference<value_type>::type;
    using const_reference = typename std::add_const<reference>::type;

    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    /** Rebind proxy class template. */
    template <typename Up>
    struct rebind
    {
        typedef Allocator<Up> other;
    };

public:
    TBAG_CONSTEXPR Allocator() TBAG_NOEXCEPT { /* EMPTY. */ }
    ~Allocator() { /* EMPTY. */ }

public:
    template <typename Up> Allocator(Allocator<Up> const & ) TBAG_NOEXCEPT { /* EMPTY. */ }

public:
    /** obtains the address of an object. */
    const_pointer address(const_reference val) const TBAG_NOEXCEPT
    {
        return addressof(val);
    }

    /** returns the largest supported allocation size. */
    size_type max_size() const TBAG_NOEXCEPT
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

public:
    /** allocates uninitialized storage. */
    pointer allocate(size_type size, Allocator<void>::const_pointer hint = 0)
    {
        return static_cast<pointer>(::libtbag::memory::allocate(size * sizeof(value_type)));
    }

    /** deallocates storage. */
    void deallocate(pointer ptr, size_type allocated_size) TBAG_NOEXCEPT
    {
        ::libtbag::memory::deallocate((void*)ptr);
    }

public:
    /** constructs an object in allocated storage. */
    template <typename U, typename ... Args>
    void construct(U * p, Args && ... args)
    {
        ::new ((void*)p) U(std::forward<Args>(args) ...);
    }

    /** destructs an object in allocated storage. */
    template <class U>
    void destroy(U * p)
    {
        p->~U();
    }
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOCATOR_HPP__

