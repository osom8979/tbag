/**
 * @file   IteratorBypass.hpp
 * @brief  IteratorBypass class prototype.
 * @author zer0
 * @date   2018-07-15
 * @date   2018-07-31 (Rename: StdContainerFacade -> IteratorBypass)
 * @date   2018-08-05 (Move directory: libtbag/iterator/facade -> libtbag/iterator)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ITERATOR_ITERATORBYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ITERATOR_ITERATORBYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace iterator {

template <typename T>
struct HasReverseIterator
{
    typedef char Yes;
    typedef long No;

    template <typename C> static Yes test(typename C::reverse_iterator *);
    template <typename C> static No  test(...);

    enum { value = (sizeof(test<T>(0)) == sizeof(Yes)) };
};

#ifndef TBAG_ITERATOR_TYPES_BYPASS
#define TBAG_ITERATOR_TYPES_BYPASS(container_type)                                      \
    using value_type        = typename container_type::value_type;                      \
    using allocator_type    = typename container_type::allocator_type;                  \
    using reference         = typename container_type::reference;                       \
    using const_reference   = typename container_type::const_reference;                 \
    using size_type         = typename container_type::size_type;                       \
    using difference_type   = typename container_type::difference_type;                 \
    using pointer           = typename container_type::pointer;                         \
    using const_pointer     = typename container_type::const_pointer;                   \
                                                                                        \
    static_assert(!std::is_pointer<value_type>::value,                                  \
            "value_type must not be a pointer type");                                   \
    static_assert(!std::is_reference<value_type>::value,                                \
            "value_type must not be a reference type");                                 \
    static_assert(std::is_same<typename allocator_type::value_type, value_type>::value, \
                  "allocator_type::value_type must be same type as value_type");        \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_SIZER_BYPASS
#define TBAG_ITERATOR_SIZER_BYPASS(container)                               \
    size_type max_size() const TBAG_NOEXCEPT_SP_OP(container.max_size())    \
    { return container.max_size(); }                                        \
    size_type size() const TBAG_NOEXCEPT_SP_OP(container.size())            \
    { return container.size(); }                                            \
    bool empty() const TBAG_NOEXCEPT_SP_OP(container.empty())               \
    { return container.empty(); }                                           \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_AT_BYPASS
#define TBAG_ITERATOR_AT_BYPASS(container)                                                      \
    reference operator [](size_type i) TBAG_NOEXCEPT_SP_OP(container.operator[](i))             \
    { return container.operator[](i); }                                                         \
    const_reference operator [](size_type i) const TBAG_NOEXCEPT_SP_OP(container.operator[](i)) \
    { return container.operator[](i); }                                                         \
                                                                                                \
    reference at(size_type i) TBAG_NOEXCEPT_SP_OP(container.at(i))                              \
    { return container.at(i); }                                                                 \
    const_reference at(size_type i) const TBAG_NOEXCEPT_SP_OP(container.at(i))                  \
    { return container.at(i); }                                                                 \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_CLEAR_BYPASS
#define TBAG_ITERATOR_CLEAR_BYPASS(container)           \
    void clear() TBAG_NOEXCEPT_SP_OP(container.clear()) \
    { return container.clear(); }                       \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_FORWARD_BYPASS
#define TBAG_ITERATOR_FORWARD_BYPASS(container_type, container)             \
    using       iterator = typename container_type::iterator;               \
    using const_iterator = typename container_type::const_iterator;         \
                                                                            \
    iterator begin() TBAG_NOEXCEPT_SP_OP(container.begin())                 \
    { return container.begin(); }                                           \
    iterator end() TBAG_NOEXCEPT_SP_OP(container.end())                     \
    { return container.end(); }                                             \
                                                                            \
    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(container.begin())     \
    { return container.begin(); }                                           \
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(container.end())         \
    { return container.end(); }                                             \
                                                                            \
    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(container.cbegin())   \
    { return container.cbegin(); }                                          \
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(container.cend())       \
    { return container.cend(); }                                            \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_REVERSE_BYPASS
#define TBAG_ITERATOR_REVERSE_BYPASS(container_type, container)                     \
    using       reverse_iterator = typename container_type::reverse_iterator;       \
    using const_reverse_iterator = typename container_type::const_reverse_iterator; \
                                                                                    \
    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(container.rbegin())               \
    { return container.rbegin(); }                                                  \
    reverse_iterator rend() TBAG_NOEXCEPT_SP_OP(container.rend())                   \
    { return container.rend(); }                                                    \
                                                                                    \
    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(container.rbegin())   \
    { return container.rbegin(); }                                                  \
    const_reverse_iterator rend() const TBAG_NOEXCEPT_SP_OP(container.rend())       \
    { return container.rend(); }                                                    \
                                                                                    \
    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(container.crbegin()) \
    { return container.crbegin(); }                                                 \
    const_reverse_iterator crend() const TBAG_NOEXCEPT_SP_OP(container.crend())     \
    { return container.crend(); }                                                   \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_VECTOR_BYPASS
#define TBAG_ITERATOR_VECTOR_BYPASS(container_type, container)  \
    TBAG_ITERATOR_TYPES_BYPASS(container_type);                 \
    TBAG_ITERATOR_AT_BYPASS(container)                          \
    TBAG_ITERATOR_SIZER_BYPASS(container);                      \
    TBAG_ITERATOR_CLEAR_BYPASS(container);                      \
    TBAG_ITERATOR_FORWARD_BYPASS(container_type, container);    \
    TBAG_ITERATOR_REVERSE_BYPASS(container_type, container);    \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_SET_BYPASS
#define TBAG_ITERATOR_SET_BYPASS(container_type, container)     \
    TBAG_ITERATOR_TYPES_BYPASS(container_type);                 \
    TBAG_ITERATOR_SIZER_BYPASS(container);                      \
    TBAG_ITERATOR_CLEAR_BYPASS(container);                      \
    TBAG_ITERATOR_FORWARD_BYPASS(container_type, container);    \
    /* -- END -- */
#endif

#ifndef TBAG_ITERATOR_MAP_BYPASS
#define TBAG_ITERATOR_MAP_BYPASS(container_type, container)     \
    TBAG_ITERATOR_TYPES_BYPASS(container_type);                 \
    TBAG_ITERATOR_SIZER_BYPASS(container);                      \
    TBAG_ITERATOR_CLEAR_BYPASS(container);                      \
    TBAG_ITERATOR_FORWARD_BYPASS(container_type, container);    \
    /* -- END -- */
#endif

} // namespace iterator

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ITERATOR_ITERATORBYPASS_HPP__

