/**
 * @file   PoolInterface.hpp
 * @brief  PoolInterface class prototype.
 * @author zer0
 * @date   2016-11-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOLINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOLINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

std::string s;

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

/**
 * PoolInterface class prototype.
 *
 * @author zer0
 * @date   2016-11-23
 */
struct TBAG_API PoolInterface
{
    // using value_type	T	Element type
    // using pointer	T*	Pointer to element
    // using reference	T&	Reference to element
    // using const_pointer	const T*	Pointer to constant element
    // using const_reference // const T&	Reference to constant element
    // using size_type	size_t // Quantities of elements
    // using difference_type = ptrdiff_t;
    // using rebind<Type> // member class	Its member type other is the equivalent allocator type to allocate elements of type Type

    // address
    // allocate
    // deallocate
    // max_size
    // construct
    // destroy
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOLINTERFACE_HPP__

