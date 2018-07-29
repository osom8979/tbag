/**
 * @file   TestAllocator.hpp
 * @brief  TestAllocator class prototype.
 * @author zer0
 * @date   2018-07-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_TESTALLOCATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_TESTALLOCATOR_HPP__

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
 * TestAllocator class prototype.
 *
 * @author zer0
 * @date   2018-07-29
 */
template <typename Type>
struct TestAllocator
{
    using value_type      = Type;
    using pointer         = typename std::add_pointer<value_type>::type;
    using const_pointer   = typename std::add_const<pointer>::type;
    using reference       = typename std::add_lvalue_reference<value_type>::type;
    using const_reference = typename std::add_const<reference>::type;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    size_type   allocate_count = 0;
    size_type deallocate_count = 0;
    size_type   allocate_elem_size = 0;
    size_type deallocate_elem_size = 0;
    size_type  construct_count = 0;
    size_type    destroy_count = 0;

    template <typename Up>
    struct rebind
    {
        typedef TestAllocator<Up> other;
    };

    TestAllocator() TBAG_NOEXCEPT
    { /* EMPTY. */ }

    template <typename Up>
    TestAllocator(Allocator<Up> const & obj) TBAG_NOEXCEPT
    { /* EMPTY. */ }

    ~TestAllocator()
    { /* EMPTY. */ }

    TBAG_CONSTEXPR static size_type max_size() TBAG_NOEXCEPT
    {
#if defined(max)
        TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
#if defined(__RESTORE_MAX__)
        TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif
    }

    const_pointer address(const_reference val) const TBAG_NOEXCEPT
    {
        return ::libtbag::memory::alloc::addressof(val);
    }

    pointer allocate(size_type size, void const * hint = 0)
    {
        assert(size > 0);
        ++allocate_count;
        allocate_elem_size += size;
        return static_cast<pointer>(::libtbag::memory::alloc::allocate(size * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type allocated_size)
    {
        assert(ptr != nullptr);
        ++deallocate_count;
        deallocate_elem_size += allocated_size;
        ::libtbag::memory::alloc::deallocate((void*)ptr);
    }

    template <typename U, typename ... Args>
    void construct(U * ptr, Args && ... args)
    {
        assert(ptr != nullptr);
        ++construct_count;
        ::new ((void*)ptr) U(std::forward<Args>(args) ...);
    }

    template <class U>
    void destroy(U * ptr)
    {
        assert(ptr != nullptr);
        ++destroy_count;
        ptr->~U();
    }

    // ------------------
    // Extension methods.
    // ------------------

    void clear() TBAG_NOEXCEPT
    {
        allocate_count = 0;
        deallocate_count = 0;
        allocate_elem_size = 0;
        deallocate_elem_size = 0;
        construct_count = 0;
        destroy_count = 0;
    }
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_TESTALLOCATOR_HPP__

