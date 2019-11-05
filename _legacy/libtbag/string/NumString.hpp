/**
 * @file   NumString.hpp
 * @brief  NumString class prototype.
 * @author zer0
 * @date   2018-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_NUMSTRING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_NUMSTRING_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <string>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * NumString class prototype.
 *
 * @author zer0
 * @date   2018-07-14
 */
class TBAG_API NumString
{
private:
    std::string _str;

public:
    NumString();
    explicit NumString(std::string const & str);
    explicit NumString(char const * str);
    NumString(NumString const & obj);
    NumString(NumString && obj);
    ~NumString();

public:
    template <typename T>
    NumString(T const & value) : NumString()
    {
        set(value);
    }

public:
    NumString & operator =(NumString const & obj);
    NumString & operator =(NumString && obj);

public:
    void copy(NumString const & obj);
    void swap(NumString & obj);

public:
    friend void swap(NumString & lh, NumString & rh)
    { lh.swap(rh); }

public:
    std::string get() const
    { return _str; }

    template <typename T>
    T get(std::size_t * index = nullptr, int base = 10) const
    {
        T value = T();
        if (libtbag::string::toVal(_str, value, index, base)) {
            return value;
        }
        return T();
    }

    operator std::string  () const { return _str; }
    operator char const * () const { return _str.c_str(); }

    template <typename T>
    operator T () const
    {
        return get<T>();
    }

public:
    void set(std::string const & str) { _str = str; }
    void set(char const * str) { _str = str; }

    template <typename T>
    void set(T const & value)
    {
        _str = libtbag::string::toString(value);
    }

    void operator ()(std::string const & str) { set(str); }
    void operator ()(char const * str) { set(str); }

    template <typename T>
    void operator ()(T const & value)
    {
        return set<T>(value);
    }

public:
    std::string toString() const;
};

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, NumString const & obj)
{
    return os << obj.toString();
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_NUMSTRING_HPP__

