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
#include <libtbag/string/details/rstr_base.hpp>
#include <libtbag/string/Format.hpp>

#include <cstdio>

#include <type_traits>
#include <iterator>
#include <string>
#include <memory>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Restricted string class prototype.
 *
 * @author zer0
 * @date   2016-11-23
 * @date   2019-07-01 (Rename: String -> rstr)
 */
class TBAG_API rstr
{
public:
    using value_type      = char;
    using traits_type     = std::char_traits<value_type>;
    using reference       = value_type &;
    using const_reference = value_type const &;
    using pointer         = value_type *;
    using const_pointer   = value_type const *;
    using difference_type = std::ptrdiff_t;
    using size_type       = int;

public:
    using       iterator = pointer;
    using const_iterator = const_pointer;

    using       reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    using        rstr_base = libtbag::string::details::rstr_base;
    using shared_rstr_base = std::shared_ptr<rstr_base>;

public:
    TBAG_CONSTEXPR static bool const NOTHROW_COPY_CONSTRUCTIBLE =
            std::is_nothrow_copy_constructible<shared_rstr_base>::value;
    TBAG_CONSTEXPR static bool const NOTHROW_MOVE_CONSTRUCTIBLE =
            std::is_nothrow_move_constructible<shared_rstr_base>::value;
    TBAG_CONSTEXPR static bool const NOTHROW_COPY_ASSIGNABLE =
            std::is_nothrow_copy_assignable<shared_rstr_base>::value;
    TBAG_CONSTEXPR static bool const NOTHROW_MOVE_ASSIGNABLE =
            std::is_nothrow_move_assignable<shared_rstr_base>::value;

public:
    TBAG_CONSTEXPR static size_type const npos = -1;

public:
    TBAG_CONSTEXPR static size_type max_size() TBAG_NOEXCEPT
    { return libtbag::string::details::RSTR_MAX_SIZE; }

private:
    shared_rstr_base _base;

public:
    rstr();
    explicit rstr(std::nullptr_t) TBAG_NOEXCEPT;
    rstr(rstr const & obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_COPY_CONSTRUCTIBLE);
    rstr(rstr && obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_MOVE_CONSTRUCTIBLE);
    ~rstr();

public:
    rstr & operator =(rstr const & obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_COPY_ASSIGNABLE);
    rstr & operator =(rstr && obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_MOVE_ASSIGNABLE);

public:
    void swap(rstr & obj) TBAG_NOEXCEPT;

public:
    friend void swap(rstr & lh, rstr & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_base); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline rstr_base       * get()       TBAG_NOEXCEPT { return _base.get(); }
    inline rstr_base const * get() const TBAG_NOEXCEPT { return _base.get(); }

    inline rstr_base       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline rstr_base const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline rstr_base       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline rstr_base const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline rstr_base       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline rstr_base const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    rstr clone() const;

// Iterators:
public:
    iterator begin() TBAG_NOEXCEPT;
    iterator end() TBAG_NOEXCEPT;

    const_iterator begin() const TBAG_NOEXCEPT;
    const_iterator end() const TBAG_NOEXCEPT;

    const_iterator cbegin() const TBAG_NOEXCEPT;
    const_iterator cend() const TBAG_NOEXCEPT;

public:
    reverse_iterator rbegin() TBAG_NOEXCEPT;
    reverse_iterator rend() TBAG_NOEXCEPT;

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT;
    const_reverse_iterator rend() const TBAG_NOEXCEPT;

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT;
    const_reverse_iterator crend() const TBAG_NOEXCEPT;

public:
    size_type size() const TBAG_NOEXCEPT;
    size_type length() const TBAG_NOEXCEPT;
    size_type capacity() const TBAG_NOEXCEPT;
    bool empty() const TBAG_NOEXCEPT;

public:
    void resize(size_type n, int flags);
    void resize(size_type n);
    void reserve(size_type n, int flags);
    void reserve(size_type n);
    void clear() TBAG_NOEXCEPT;

public:
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

    reference at(size_type i);
    const_reference at(size_type i) const;

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

public:
    void assign(rstr const & obj);
    void assign(const_pointer src, size_type n);
    void assign(const_pointer src);
    void assign(std::string const & src);

public:
    rstr & append(rstr const & obj) TBAG_NOEXCEPT;
    rstr & append(const_pointer src, size_type n) TBAG_NOEXCEPT;
    rstr & append(const_pointer src) TBAG_NOEXCEPT;
    rstr & append(std::string const & src) TBAG_NOEXCEPT;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_PRINTF_BUFFER =
            libtbag::string::details::RSTR_LARGE_CAPACITY;

    template <int BufferSize = DEFAULT_PRINTF_BUFFER, typename ... Args>
    rstr & appendPrintf(char const * format, Args && ... args)
    {
        value_type buffer[BufferSize];
        auto const result = snprintf(buffer, BufferSize, format, std::move<Args>(args) ...);
        if (0 < COMPARE_AND(result) < BufferSize) {
            return append(buffer, result);
        }
        return *this;
    }

    template <typename ... Args>
    rstr & appendFormat(std::string const & f, Args && ... args)
    {
        return append(libtbag::string::fformat(f, std::forward<Args>(args) ...));
    }

public:
    // operator+=
    // insert
    // erase
    // replace
    // push_back
    // pop_back

public:
    pointer c_str() TBAG_NOEXCEPT;
    const_pointer c_str() const TBAG_NOEXCEPT;

    pointer data() TBAG_NOEXCEPT;
    const_pointer data() const TBAG_NOEXCEPT;

public:
    // copy
    // find
    // rfind
    // find_first_of
    // find_last_of
    // find_first_not_of
    // find_last_not_of
    // substr

public:
    int compare(rstr const & obj) const TBAG_NOEXCEPT;
    int compare(const_pointer s) const TBAG_NOEXCEPT;

    bool operator ==(rstr const & obj) const TBAG_NOEXCEPT;
    bool operator ==(const_pointer s) const TBAG_NOEXCEPT;
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRING_HPP__

