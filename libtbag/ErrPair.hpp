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
    using Val = T;
    using Msg = std::string;

    static_assert(!std::is_void<Val>::value, "Void type is not supported.");
    static_assert(!std::is_reference<Val>::value, "Reference type is not supported.");

    TBAG_CONSTEXPR static bool const m_def_cons = std::is_nothrow_default_constructible<Msg>::value;
    TBAG_CONSTEXPR static bool const m_copy_cons = std::is_nothrow_copy_constructible<Msg>::value;
    TBAG_CONSTEXPR static bool const m_move_cons = std::is_nothrow_move_constructible<Msg>::value;
    TBAG_CONSTEXPR static bool const m_copy_assign = std::is_nothrow_copy_assignable<Msg>::value;
    TBAG_CONSTEXPR static bool const m_move_assign = std::is_nothrow_move_assignable<Msg>::value;

    TBAG_CONSTEXPR static bool const v_def_cons = std::is_nothrow_default_constructible<Val>::value;
    TBAG_CONSTEXPR static bool const v_copy_cons = std::is_nothrow_copy_constructible<Val>::value;
    TBAG_CONSTEXPR static bool const v_move_cons = std::is_nothrow_move_constructible<Val>::value;
    TBAG_CONSTEXPR static bool const v_copy_assign = std::is_nothrow_copy_assignable<Val>::value;
    TBAG_CONSTEXPR static bool const v_move_assign = std::is_nothrow_move_assignable<Val>::value;

    Msg msg;
    Err code;
    Val val;

    ErrPair() TBAG_NOEXCEPT_SPECIFIER(m_def_cons&&v_def_cons)
            : code(E_UNKNOWN)
    { /* EMPTY. */ }
    ErrPair(Err c) TBAG_NOEXCEPT_SPECIFIER(m_def_cons&&v_def_cons)
            : code(c)
    { /* EMPTY. */ }

    ErrPair(Val const & v) TBAG_NOEXCEPT_SPECIFIER(m_def_cons&&v_copy_cons)
            : code(E_SUCCESS), val(v)
    { /* EMPTY. */ }
    ErrPair(Val && v) TBAG_NOEXCEPT_SPECIFIER(m_def_cons&&v_move_cons)
            : code(E_SUCCESS), val(std::move(v))
    { /* EMPTY. */ }

    ErrPair(Err c, Val const & v) TBAG_NOEXCEPT_SPECIFIER(m_def_cons&&v_copy_cons)
            : code(c), val(v)
    { /* EMPTY. */ }
    ErrPair(Err c, Val && v) TBAG_NOEXCEPT_SPECIFIER(m_def_cons&&v_move_cons)
            : code(c), val(std::move(v))
    { /* EMPTY. */ }

    ErrPair(Msg const & m, Err c) TBAG_NOEXCEPT_SPECIFIER(m_copy_cons&&v_def_cons)
            : msg(m), code(c)
    { /* EMPTY. */ }
    ErrPair(Msg && m, Err c) TBAG_NOEXCEPT_SPECIFIER(m_move_cons&&v_def_cons)
            : msg(std::move(m)), code(c)
    { /* EMPTY. */ }

    ErrPair(Msg const & m, Err c, Val const & v) TBAG_NOEXCEPT_SPECIFIER(m_copy_cons&&v_copy_cons)
            : msg(m), code(c), val(v)
    { /* EMPTY. */ }
    ErrPair(Msg const & m, Err c, Val && v) TBAG_NOEXCEPT_SPECIFIER(m_copy_cons&&v_move_cons)
            : msg(m), code(c), val(std::move(v))
    { /* EMPTY. */ }

    ErrPair(Msg && m, Err c, Val const & v) TBAG_NOEXCEPT_SPECIFIER(m_move_cons&&v_copy_cons)
            : msg(std::move(m)), code(c), val(v)
    { /* EMPTY. */ }
    ErrPair(Msg && m, Err c, Val && v) TBAG_NOEXCEPT_SPECIFIER(m_move_cons&&v_move_cons)
            : msg(std::move(m)), code(c), val(std::move(v))
    { /* EMPTY. */ }

    ErrPair(ErrPair const & obj) TBAG_NOEXCEPT_SPECIFIER(m_copy_cons&&v_copy_cons)
            : msg(obj.msg), code(obj.code), val(obj.val)
    { /* EMPTY. */ }
    ErrPair(ErrPair && obj) TBAG_NOEXCEPT_SPECIFIER(m_move_cons&&v_move_cons)
            : msg(std::move(obj.msg)), code(obj.code), val(std::move(obj.val))
    { /* EMPTY. */ }

    ~ErrPair()
    { /* EMPTY. */ }

    ErrPair & operator =(Err c) TBAG_NOEXCEPT
    {
        code = c;
        return *this;
    }

    ErrPair & operator =(Val const & v) TBAG_NOEXCEPT_SPECIFIER(v_copy_assign)
    {
        val = v;
        return *this;
    }

    ErrPair & operator =(Val && v) TBAG_NOEXCEPT_SPECIFIER(v_move_assign)
    {
        val = v;
        return *this;
    }

    ErrPair & operator =(ErrPair const & obj) TBAG_NOEXCEPT_SPECIFIER(m_copy_assign&&v_copy_assign)
    {
        if (this != &obj) {
            msg = obj.msg;
            code = obj.code;
            val = obj.val;
        }
        return *this;
    }

    ErrPair & operator =(ErrPair && obj) TBAG_NOEXCEPT_SPECIFIER(m_move_assign&&v_move_assign)
    {
        if (this != &obj) {
            msg = std::move(obj.msg);
            code = obj.code;
            val = std::move(obj.val);
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
        msg = libtbag::string::fformat(f, std::forward<Args>(args) ...);
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os,
                                                           ErrPair const & err)
    {
        if (err.msg.empty()) {
            return os << err.name();
        } else {
            return os << err.name() << "('" << err.msg << "')";
        }
    }
};

using ErrMsg = ErrPair<std::nullptr_t>;
STATIC_ASSERT_CHECK_IS_SAME(typename ErrMsg::Val, std::nullptr_t);

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_ERR_FMT
#define TBAG_ERR_FMT(code, ...) { ::libtbag::string::fformat(__VA_ARGS__), ::libtbag::Err(code) }
#endif

#ifndef TBAG_ERR_OK
#define TBAG_ERR_OK(value) { ::libtbag::E_SUCCESS, value }
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__

