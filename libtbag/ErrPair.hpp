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
    using Val = T;

    static_assert(!std::is_void<Val>::value, "Void type is not supported.");
    static_assert(!std::is_reference<Val>::value, "Reference type is not supported.");

    Err code;
    Val value;

    TBAG_CONSTEXPR static bool const _nothrow_def = std::is_nothrow_default_constructible<Val>::value;
    TBAG_CONSTEXPR static bool const _nothrow_copy = std::is_nothrow_copy_constructible<Val>::value;
    TBAG_CONSTEXPR static bool const _nothrow_move = std::is_nothrow_move_constructible<Val>::value;
    TBAG_CONSTEXPR static bool const _nothrow_copy_assign = std::is_nothrow_copy_assignable<Val>::value;
    TBAG_CONSTEXPR static bool const _nothrow_move_assign = std::is_nothrow_move_assignable<Val>::value;

    ErrPair() TBAG_NOEXCEPT_SPECIFIER(_nothrow_def)
            : code(E_UNKNOWN), value()
    { /* EMPTY. */ }
    ErrPair(Err c) TBAG_NOEXCEPT_SPECIFIER(_nothrow_def)
            : code(c), value()
    { /* EMPTY. */ }

    ErrPair(Val const & v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy)
            : code(E_UNKNOWN), value(v)
    { /* EMPTY. */ }
    ErrPair(Err c, Val const & v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy)
            : code(c), value(v)
    { /* EMPTY. */ }

    ErrPair(Val && v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move)
            : code(E_UNKNOWN), value(std::move(v))
    { /* EMPTY. */ }
    ErrPair(Err c, Val && v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move)
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

    ErrPair & operator =(Val const & v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_copy_assign)
    {
        value = v;
        return *this;
    }

    ErrPair & operator =(Val && v) TBAG_NOEXCEPT_SPECIFIER(_nothrow_move_assign)
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

    template <class CharT, class TraitsT, class ValueT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os,
                                                           ErrPair const & err)
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
    using Base = ErrPair<T>;
    using Val = typename Base::Val;
    using Msg = std::string;

    TBAG_CONSTEXPR static bool const _base_nothrow_move = std::is_nothrow_move_constructible<Base>::value;
    TBAG_CONSTEXPR static bool const _base_nothrow_move_assign = std::is_nothrow_move_assignable<Base>::value;
    TBAG_CONSTEXPR static bool const _msg_nothrow_move = std::is_nothrow_move_constructible<Msg>::value;
    TBAG_CONSTEXPR static bool const _msg_nothrow_move_assign = std::is_nothrow_move_assignable<Msg>::value;

    Msg msg;

    ErrMsgPair() : ErrPair<T>()
    { /* EMPTY. */ }
    ErrMsgPair(Err c) : ErrPair<T>(c)
    { /* EMPTY. */ }
    ErrMsgPair(Msg const & m, Err c) : ErrPair<T>(c), msg(m)
    { /* EMPTY. */ }
    ErrMsgPair(Msg && m, Err c) : ErrPair<T>(c), msg(std::move(m))
    { /* EMPTY. */ }

    ErrMsgPair(Val const & v) : ErrPair<T>(v)
    { /* EMPTY. */ }
    ErrMsgPair(Err c, Val const & v) : ErrPair<T>(c, v)
    { /* EMPTY. */ }
    ErrMsgPair(Msg const & m, Err c, Val const & v) : ErrPair<T>(c, v), msg(m)
    { /* EMPTY. */ }
    ErrMsgPair(Msg && m, Err c, Val const & v) : ErrPair<T>(c, v), msg(std::move(m))
    { /* EMPTY. */ }

    ErrMsgPair(Val && v) : ErrPair<T>(std::move(v))
    { /* EMPTY. */ }
    ErrMsgPair(Err c, Val && v) : ErrPair<T>(c, std::move(v))
    { /* EMPTY. */ }
    ErrMsgPair(Msg const & m, Err c, Val && v) : ErrPair<T>(c, std::move(v)), msg(m)
    { /* EMPTY. */ }
    ErrMsgPair(Msg && m, Err c, Val && v) : ErrPair<T>(c, std::move(v)), msg(std::move(m))
    { /* EMPTY. */ }

    ErrMsgPair(ErrMsgPair const & obj) : ErrPair<T>(obj), msg(obj.msg)
    { /* EMPTY. */ }
    ErrMsgPair(ErrMsgPair && obj) TBAG_NOEXCEPT_SPECIFIER(_base_nothrow_move && _msg_nothrow_move)
            : ErrPair<T>(std::move(obj)), msg(std::move(obj.msg))
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

    ErrMsgPair & operator =(ErrMsgPair && obj)
            TBAG_NOEXCEPT_SPECIFIER(_base_nothrow_move_assign && _msg_nothrow_move_assign)
    {
        if (this != &obj) {
            ErrPair<T>::operator =(std::move(obj));
            msg = std::move(obj.msg);
        }
        return *this;
    }

    operator ErrMsgPair<std::nullptr_t>() const
    {
        return ErrMsgPair<std::nullptr_t>(this->msg, this->code);
    }

    operator ErrPair<Val>() const
    {
        return ErrPair<Val>(this->code, this->value);
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os,
                                                           ErrMsgPair const & obj)
    {
        if (obj.msg.empty()) {
            return os << obj.name();
        } else {
            return os << obj.name() << "('" << obj.msg << "')";
        }
    }
};

using ErrMsg = ErrMsgPair<std::nullptr_t>;
STATIC_ASSERT_CHECK_IS_SAME(typename ErrMsg::Val, std::nullptr_t);

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <libtbag/string/Format.hpp>

#ifndef TBAG_ERR_FMT
#define TBAG_ERR_FMT(code, ...) { ::libtbag::string::fformat(__VA_ARGS__), code }
#endif

#ifndef TBAG_ERR_OK
#define TBAG_ERR_OK(value) { E_SUCCESS, value }
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__

