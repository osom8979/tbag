/**
 * @file   StdContainerFacade.hpp
 * @brief  StdContainerFacade class prototype.
 * @author zer0
 * @date   2018-07-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_STDCONTAINERFACADE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_STDCONTAINERFACADE_HPP__

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

namespace pattern {

template <typename T>
struct HasStdReverseIterator
{
    typedef char Yes;
    typedef long No;

    template <typename C> static Yes test(typename C::reverse_iterator *);
    template <typename C> static No  test(...);

    enum { value = (sizeof(test<T>(0)) == sizeof(Yes)) };
};

template <typename T, bool enable_reverse>
struct StdContainerFacadeBackend;

template <typename T>
struct StdContainerFacadeBackend<T, false>
{
    using Container = T;

    using value_type        = typename Container::value_type;
    using allocator_type    = typename Container::allocator_type;
    using reference         = typename Container::reference;
    using const_reference   = typename Container::const_reference;
    using size_type         = typename Container::size_type;
    using difference_type   = typename Container::difference_type;
    using pointer           = typename Container::pointer;
    using const_pointer     = typename Container::const_pointer;

    static_assert(!std::is_pointer<value_type>::value,
                  "value_type must not be a pointer type");
    static_assert(!std::is_reference<value_type>::value,
                  "value_type must not be a reference type");
    static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                  "allocator_type::value_type must be same type as value_type");

    Container & _c;

    StdContainerFacadeBackend(Container & c) TBAG_NOEXCEPT : _c(c) { /* EMPTY. */ }
    ~StdContainerFacadeBackend() { /* EMPTY. */ }

    size_type max_size() const TBAG_NOEXCEPT_SP_OP(_c.max_size()) { return _c.max_size(); }
    size_type     size() const TBAG_NOEXCEPT_SP_OP(_c.    size()) { return _c.    size(); }

    bool empty() const TBAG_NOEXCEPT_SP_OP(_c.empty()) { return _c.empty(); }

    using       iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;

    iterator begin() TBAG_NOEXCEPT_SP_OP(_c.begin()){ return _c.begin(); }
    iterator   end() TBAG_NOEXCEPT_SP_OP(_c.  end()){ return _c.  end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(_c.begin()){ return _c.begin(); }
    const_iterator   end() const TBAG_NOEXCEPT_SP_OP(_c.  end()){ return _c.  end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(_c.cbegin()) { return _c.cbegin(); }
    const_iterator   cend() const TBAG_NOEXCEPT_SP_OP(_c.  cend()) { return _c.  cend(); }
};

template <typename T>
struct StdContainerFacadeBackend<T, true> : public StdContainerFacadeBackend<T, false>
{
    using Parent    = StdContainerFacadeBackend<T, false>;
    using Container = typename Parent::Container;

    StdContainerFacadeBackend(Container & c) TBAG_NOEXCEPT : Parent(c) { /* EMPTY. */ }
    ~StdContainerFacadeBackend() { /* EMPTY. */ }

    using       reverse_iterator = typename Parent::Container::reverse_iterator;
    using const_reverse_iterator = typename Parent::Container::const_reverse_iterator;

    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(Parent::_c.rbegin()) { return Parent::_c.rbegin(); }
    reverse_iterator   rend() TBAG_NOEXCEPT_SP_OP(Parent::_c.  rend()) { return Parent::_c.  rend(); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(Parent::_c.rbegin()) { return Parent::_c.rbegin(); }
    const_reverse_iterator   rend() const TBAG_NOEXCEPT_SP_OP(Parent::_c.  rend()) { return Parent::_c.  rend(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(Parent::_c.crbegin()) { return Parent::_c.crbegin(); }
    const_reverse_iterator   crend() const TBAG_NOEXCEPT_SP_OP(Parent::_c.  crend()) { return Parent::_c.  crend(); }
};

/**
 * StdContainerFacade class prototype.
 *
 * @author zer0
 * @date   2018-07-15
 */
template <typename T>
struct StdContainerFacade : public StdContainerFacadeBackend<T, HasStdReverseIterator<T>::value>
{
    TBAG_CONSTEXPR static bool const has_reverse_iterator = HasStdReverseIterator<T>::value;

    using Parent    = StdContainerFacadeBackend<T, has_reverse_iterator>;
    using Container = typename Parent::Container;

    StdContainerFacade(Container & c) TBAG_NOEXCEPT : Parent(c) { /* EMPTY. */ }
    ~StdContainerFacade() { /* EMPTY. */ }
};

} // namespace pattern

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_STDCONTAINERFACADE_HPP__

