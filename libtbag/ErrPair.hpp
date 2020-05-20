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
#include <libtbag/Type.hpp>
#include <libtbag/string/Format.hpp>

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

    TBAG_CONSTEXPR static bool const _nothrow_def = std::is_nothrow_default_constructible<ValueType>::value;
    TBAG_CONSTEXPR static bool const _nothrow_copy = std::is_nothrow_copy_constructible<ValueType>::value;
    TBAG_CONSTEXPR static bool const _nothrow_move = std::is_nothrow_move_constructible<ValueType>::value;
    TBAG_CONSTEXPR static bool const _nothrow_copy_assign = std::is_nothrow_copy_assignable<ValueType>::value;
    TBAG_CONSTEXPR static bool const _nothrow_move_assign = std::is_nothrow_move_assignable<ValueType>::value;

    ErrPair() TBAG_NOEXCEPT_SPECIFIER(_nothrow_def)
            : code(E_UNKNOWN), value()
    { /* EMPTY. */ }
    ErrPair(Err c) TBAG_NOEXCEPT_SPECIFIER(_nothrow_def)
            : code(c), value()
    { /* EMPTY. */ }

    ErrPair(ValueType const & v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy)
            : code(E_UNKNOWN), value(v)
    { /* EMPTY. */ }
    ErrPair(Err c, ValueType const & v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy)
            : code(c), value(v)
    { /* EMPTY. */ }

    ErrPair(ValueType && v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move)
            : code(E_UNKNOWN), value(std::move(v))
    { /* EMPTY. */ }
    ErrPair(Err c, ValueType && v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move)
            : code(c), value(std::move(v))
    { /* EMPTY. */ }

    ErrPair(ErrPair const & obj) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy)
            : code(obj.code), value(obj.value)
    { /* EMPTY. */ }
    ErrPair(ErrPair && obj) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move)
            : code(obj.code), value(std::move(obj.value))
    { /* EMPTY. */ }

    ~ErrPair()
    { /* EMPTY. */ }

    ErrPair & operator =(Err c) TBAG_NOEXCEPT
    {
        code = c;
        return *this;
    }

    ErrPair & operator =(ValueType const & v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy_assign)
    {
        value = v;
        return *this;
    }

    ErrPair & operator =(ValueType && v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move_assign)
    {
        value = std::move(v);
        return *this;
    }

    ErrPair & operator =(ErrPair const & obj) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy_assign)
    {
        if (this != &obj) {
            code = obj.code;
            value = obj.value;
        }
        return *this;
    }

    ErrPair & operator =(ErrPair && obj) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move_assign)
    {
        if (this != &obj) {
            code = obj.code;
            value = std::move(obj.value);
        }
        return *this;
    }

    inline bool operator ==(Err c) const TBAG_NOEXCEPT
    {
        return code == c;
    }

    inline bool operator !=(Err c) const TBAG_NOEXCEPT
    {
        return code != c;
    }

    char const * name() const TBAG_NOEXCEPT
    {
        return libtbag::getErrName(code);
    }

    char const * detail() const TBAG_NOEXCEPT
    {
        return libtbag::getErrDetail(code);
    }

    inline bool isSuccess() const TBAG_NOEXCEPT
    {
        return ::isSuccess(code);
    }

    inline bool isFailure() const TBAG_NOEXCEPT
    {
        return ::isFailure(code);
    }

    inline operator Err() const TBAG_NOEXCEPT
    {
        return code;
    }

    inline explicit operator bool() const TBAG_NOEXCEPT
    {
        return this->isSuccess();
    }

    inline int toInt() const TBAG_NOEXCEPT
    {
        return static_cast<int>(code);
    }

    inline void fromInt(int v) TBAG_NOEXCEPT
    {
        code = static_cast<Err>(v);
    }

    template <typename ... Args>
    void format(std::string const & f, Args && ... args)
    {
        value = libtbag::string::fformat(f, std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    static ErrPair createFmt(Err code, std::string const & f, Args && ... args)
    {
        ErrPair result(code);
        result.format(f, std::forward<Args>(args) ...);
        return result;
    }
};

template <class CharT, class TraitsT, class ValueT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os,
                                                ErrPair<ValueT> const & err)
{
    return os << err.name();
}

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os,
                                                ErrPair<std::string> const & err)
{
    if (err.value.empty()) {
        return os << err.name();
    } else {
        return os << err.name() << "('" << err.value << "')";
    }
}

using ErrString = ErrPair<std::string>;
using ErrStr = ErrString;
using ErrMsg = ErrString;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_ERR_FMT
#define TBAG_ERR_FMT(err, ...) ::libtbag::ErrMsg::createFmt(err, __VA_ARGS__)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__

