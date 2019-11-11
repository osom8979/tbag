/**
 * @file   ErrPair.hpp
 * @brief  ErrPair class prototype.
 * @author zer0
 * @date   2019-09-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <type_traits>
#include <string>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * ErrPair class template.
 *
 * @author zer0
 * @date   2019-09-11
 */
template <typename T>
struct ErrPair
{
    using ValueType = T;

    static_assert(!std::is_void<ValueType>::value, "Void type is not supported.");
    static_assert(!std::is_reference<ValueType>::value, "Reference type is not supported.");

    Err code;
    ValueType value;

    TBAG_CONSTEXPR static bool const nothrow_def = std::is_nothrow_default_constructible<ValueType>::value;
    TBAG_CONSTEXPR static bool const nothrow_copy = std::is_nothrow_copy_constructible<ValueType>::value;
    TBAG_CONSTEXPR static bool const nothrow_move = std::is_nothrow_move_constructible<ValueType>::value;
    TBAG_CONSTEXPR static bool const nothrow_copy_assign = std::is_nothrow_copy_assignable<ValueType>::value;
    TBAG_CONSTEXPR static bool const nothrow_move_assign = std::is_nothrow_move_assignable<ValueType>::value;

    ErrPair() TBAG_NOEXCEPT_SPECIFIER(nothrow_def)
            : code(E_UNKNOWN), value()
    { /* EMPTY. */ }
    ErrPair(Err c) TBAG_NOEXCEPT_SPECIFIER(nothrow_def)
            : code(c), value()
    { /* EMPTY. */ }

    ErrPair(ValueType const & v) TBAG_NOEXCEPT_SPECIFIER(nothrow_copy)
            : code(E_UNKNOWN), value(v)
    { /* EMPTY. */ }
    ErrPair(Err c, ValueType const & v) TBAG_NOEXCEPT_SPECIFIER(nothrow_copy)
            : code(c), value(v)
    { /* EMPTY. */ }

    ErrPair(ValueType && v) TBAG_NOEXCEPT_SPECIFIER(nothrow_move)
            : code(E_UNKNOWN), value(std::move(v))
    { /* EMPTY. */ }
    ErrPair(Err c, ValueType && v) TBAG_NOEXCEPT_SPECIFIER(nothrow_move)
            : code(c), value(std::move(v))
    { /* EMPTY. */ }

    ErrPair(ErrPair const & obj) TBAG_NOEXCEPT_SPECIFIER(nothrow_copy)
            : code(obj.code), value(obj.value)
    { /* EMPTY. */ }
    ErrPair(ErrPair && obj) TBAG_NOEXCEPT_SPECIFIER(nothrow_move)
            : code(obj.code), value(std::move(obj.value))
    { /* EMPTY. */ }

    ~ErrPair()
    { /* EMPTY. */ }

    ErrPair & operator =(Err c) TBAG_NOEXCEPT
    {
        code = c;
        return *this;
    }

    ErrPair & operator =(ValueType const & v) TBAG_NOEXCEPT_SPECIFIER(nothrow_copy_assign)
    {
        value = v;
        return *this;
    }

    ErrPair & operator =(ValueType && v) TBAG_NOEXCEPT_SPECIFIER(nothrow_move_assign)
    {
        value = std::move(v);
        return *this;
    }

    ErrPair & operator =(ErrPair const & obj) TBAG_NOEXCEPT_SPECIFIER(nothrow_copy_assign)
    {
        if (this != &obj) {
            code = obj.code;
            value = obj.value;
        }
        return *this;
    }

    ErrPair & operator =(ErrPair && obj) TBAG_NOEXCEPT_SPECIFIER(nothrow_move_assign)
    {
        if (this != &obj) {
            code = obj.code;
            value = std::move(obj.value);
        }
        return *this;
    }

    char const * name() const TBAG_NOEXCEPT
    {
        return libtbag::getErrName(code);
    }

    char const * detail() const TBAG_NOEXCEPT
    {
        return libtbag::getErrDetail(code);
    }

    inline operator Err() const TBAG_NOEXCEPT
    {
        return code;
    }

    inline operator bool() const TBAG_NOEXCEPT
    {
        return isSuccess(code);
    }

    inline int toInt() const TBAG_NOEXCEPT
    {
        return static_cast<int>(code);
    }

    inline void fromInt(int v) TBAG_NOEXCEPT
    {
        code = static_cast<Err>(v);
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, ErrPair const & err)
    {
        return os << err.name();
    }
};

/**
 * ErrMsgPair class template.
 *
 * @author zer0
 * @date   2019-11-11
 */
template <typename T>
struct ErrMsgPair : public ErrPair<T>
{
    using ValueType = typename ErrPair<T>::ValueType;

    std::string msg;

    ErrMsgPair() : ErrPair<T>()
    { /* EMPTY. */ }
    ErrMsgPair(Err c) : ErrPair<T>(c)
    { /* EMPTY. */ }

    ErrMsgPair(ValueType const & v) : ErrPair<T>(v)
    { /* EMPTY. */ }
    ErrMsgPair(Err c, ValueType const & v) : ErrPair<T>(c, v)
    { /* EMPTY. */ }

    ErrMsgPair(ValueType && v) : ErrPair<T>(std::move(v))
    { /* EMPTY. */ }
    ErrMsgPair(Err c, ValueType && v) : ErrPair<T>(c, std::move(v))
    { /* EMPTY. */ }

    ErrMsgPair(ErrMsgPair const & obj) : ErrPair<T>(obj), msg(obj.msg)
    { /* EMPTY. */ }
    ErrMsgPair(ErrMsgPair && obj) TBAG_NOEXCEPT : ErrPair<T>(std::move(obj)), msg(std::move(obj.msg))
    { /* EMPTY. */ }

    ~ErrMsgPair()
    { /* EMPTY. */ }

    ErrMsgPair & operator =(ErrMsgPair const & obj)
    {
        if (this != &obj) {
            ErrPair<T>::operator =(obj);
            msg = obj.msg;
        }
        return *this;
    }

    ErrMsgPair & operator =(ErrMsgPair && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            ErrPair<T>::operator =(std::move(obj));
            msg = std::move(obj.msg);
        }
        return *this;
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__

