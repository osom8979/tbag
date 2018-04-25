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
#include <libtbag/memory/alloc/TraceNew.hpp>

#include <cassert>
#include <memory>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

/**
 * Allocator interface class template prototype.
 *
 * @author zer0
 * @date   2017-01-04
 */
template <typename Type>
struct Allocator
{
    using value_type      = Type;
    using pointer         = typename std::add_pointer<value_type>::type;
    using const_pointer   = typename std::add_const<pointer>::type;
    using reference       = typename std::add_lvalue_reference<value_type>::type;
    using const_reference = typename std::add_const<reference>::type;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename Up>
    struct rebind
    {
        typedef Allocator<Up> other;
    };

    TBAG_CONSTEXPR Allocator() TBAG_NOEXCEPT
    { /* EMPTY. */ }

    template <typename Up>
    Allocator(Allocator<Up> const & obj) TBAG_NOEXCEPT
    { /* EMPTY. */ }

    ~Allocator()
    { /* EMPTY. */ }

    /** returns the largest supported allocation size. */
    TBAG_CONSTEXPR static size_type max_size() TBAG_NOEXCEPT
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    /** obtains the address of an object. */
    const_pointer address(const_reference val) const TBAG_NOEXCEPT
    {
        return ::libtbag::memory::alloc::addressof(val);
    }

    /** allocates uninitialized storage. */
    pointer allocate(size_type size, void const * hint = 0)
    {
        assert(size > 0);
        return static_cast<pointer>(::libtbag::memory::alloc::allocate(size * sizeof(value_type)));
    }

    /** deallocates storage. */
    void deallocate(pointer ptr, size_type allocated_size)
    {
        assert(ptr != nullptr);
        ::libtbag::memory::alloc::deallocate((void*)ptr);
    }

    /** constructs an object in allocated storage. */
    template <typename U, typename ... Args>
    void construct(U * ptr, Args && ... args)
    {
        assert(ptr != nullptr);
        ::new ((void*)ptr) U(std::forward<Args>(args) ...);
    }

    /** destructs an object in allocated storage. */
    template <class U>
    void destroy(U * ptr)
    {
        assert(ptr != nullptr);
        ptr->~U();
    }
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_ALLOCATOR_HPP__

