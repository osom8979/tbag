/**
 * @file   String.hpp
 * @brief  String class prototype.
 * @author zer0
 * @date   2016-11-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRING_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * String class prototype.
 *
 * @author zer0
 * @date   2016-11-23
 */
class TBAG_API String
{
public:
    // using value_type = char;
    // using traits_type = char_traits<char>;
    // using allocator_type = allocator<char>;
    // using reference = char&;
    // using const_reference = const char&;
    // using pointer = char*;
    // using const_pointer = const char*;
    // using difference_type = ptrdiff_t;
    // using size_type = size_t;

public:
    // iterator // a random access iterator to char (convertible to const_iterator);
    // const_iterator // a random access iterator to const char;
    // reverse_iterator // reverse_iterator<iterator>;
    // const_reverse_iterator // reverse_iterator<const_iterator>;

public:
    String();
    ~String();

public:
    String & operator =(String const & str);

// Iterators:
public:
    // begin
    // end
    // rbegin
    // rend

// Capacity:
public:
    // size
    // length
    // max_size
    // resize
    // capacity
    // reserve
    // clear
    // empty

// Element access:
public:
    // operator[]
    // at
    // back
    // front

// Modifiers:
public:
    // operator+=
    // append
    // push_back
    // assign
    // insert
    // erase
    // replace
    // swap
    // pop_back

// String operations:
public:
    // c_str
    // data
    // get_allocator
    // copy
    // find
    // rfind
    // find_first_of
    // find_last_of
    // find_first_not_of
    // find_last_not_of
    // substr
    // compare

// Member constants:
public:
    // npos

// Non-member function overloads
public:
    // operator+
    // swap
    // operator>>
    // operator<<
    // getline
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRING_HPP__

