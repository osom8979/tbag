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

    inline operator bool() const TBAG_NOEXCEPT
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
    using BaseErrPair = ErrPair<T>;
    using ValueType = typename BaseErrPair::ValueType;

    TBAG_CONSTEXPR static bool const _base_nothrow_move = std::is_nothrow_move_constructible<BaseErrPair>::value;
    TBAG_CONSTEXPR static bool const _base_nothrow_move_assign = std::is_nothrow_move_assignable<BaseErrPair>::value;

    TBAG_CONSTEXPR static bool const _str_nothrow_move = std::is_nothrow_move_constructible<std::string>::value;
    TBAG_CONSTEXPR static bool const _str_nothrow_move_assign = std::is_nothrow_move_assignable<std::string>::value;

    std::string msg;

    ErrMsgPair() : ErrPair<T>()
    { /* EMPTY. */ }
    ErrMsgPair(Err c) : ErrPair<T>(c)
    { /* EMPTY. */ }
    ErrMsgPair(std::string const & m, Err c) : ErrPair<T>(c), msg(m)
    { /* EMPTY. */ }

    ErrMsgPair(ValueType const & v) : ErrPair<T>(v)
    { /* EMPTY. */ }
    ErrMsgPair(Err c, ValueType const & v) : ErrPair<T>(c, v)
    { /* EMPTY. */ }
    ErrMsgPair(std::string const & m, Err c, ValueType const & v) : ErrPair<T>(c, v), msg(m)
    { /* EMPTY. */ }

    ErrMsgPair(ValueType && v) : ErrPair<T>(std::move(v))
    { /* EMPTY. */ }
    ErrMsgPair(Err c, ValueType && v) : ErrPair<T>(c, std::move(v))
    { /* EMPTY. */ }
    ErrMsgPair(std::string && m, Err c, ValueType && v) : ErrPair<T>(c, std::move(v)), msg(std::move(m))
    { /* EMPTY. */ }

    ErrMsgPair(ErrMsgPair const & obj) : ErrPair<T>(obj), msg(obj.msg)
    { /* EMPTY. */ }
    ErrMsgPair(ErrMsgPair && obj) TBAG_NOEXCEPT_SPECIFIER(_base_nothrow_move && _str_nothrow_move)
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

    ErrMsgPair & operator =(ErrMsgPair && obj) TBAG_NOEXCEPT_SPECIFIER(
            _base_nothrow_move_assign && _str_nothrow_move_assign)
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

    operator ErrPair<ValueType>() const
    {
        return ErrPair<ValueType>(this->code, this->value);
    }
};

using ErrMsg = ErrMsgPair<std::nullptr_t>;
STATIC_ASSERT_CHECK_IS_SAME(typename ErrMsg::ValueType, std::nullptr_t);

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <libtbag/macro/debug_stamp.hpp>
#include <libtbag/preprocessor/facilities/Overload.hpp>

#define TBAG_MAKE_ERR_MSG3_DBG(m, c, v) { std::string(TBAG_DEBUG_STAMP_SPACE) + m, c, v }
#define TBAG_MAKE_ERR_MSG3_REL(m, c, v) { m, c, v }
#define TBAG_MAKE_ERR_MSG2_DBG(m, c)    { std::string(TBAG_DEBUG_STAMP_SPACE) + m, c }
#define TBAG_MAKE_ERR_MSG2_REL(m, c)    { m, c }
#define TBAG_MAKE_ERR_MSG1_DBG(c)       { TBAG_DEBUG_STAMP, c }
#define TBAG_MAKE_ERR_MSG1_REL(c)       { c }

#if defined(NDEBUG)
# define TBAG_MAKE_ERR_MSG3(m, c, v) TBAG_MAKE_ERR_MSG3_REL(m, c, v)
# define TBAG_MAKE_ERR_MSG2(m, c)    TBAG_MAKE_ERR_MSG2_REL(m, c)
# define TBAG_MAKE_ERR_MSG1(c)       TBAG_MAKE_ERR_MSG1_REL(c)
#else
# define TBAG_MAKE_ERR_MSG3(m, c, v) TBAG_MAKE_ERR_MSG3_DBG(m, c, v)
# define TBAG_MAKE_ERR_MSG2(m, c)    TBAG_MAKE_ERR_MSG2_DBG(m, c)
# define TBAG_MAKE_ERR_MSG1(c)       TBAG_MAKE_ERR_MSG1_DBG(c)
#endif

#if defined(TBAG_COMP_MSVC)
# include <libtbag/preprocessor/details/Cat.hpp>
# include <libtbag/preprocessor/facilities/Empty.hpp>
# define TBAG_MAKE_ERR_MSG(...) TBAG_MAKE_ERR_MSG_I(TBAG_PP_OVERLOAD(TBAG_MAKE_ERR_MSG, __VA_ARGS__), (__VA_ARGS__))
# define TBAG_MAKE_ERR_MSG_I(m, args) TBAG_PP_CAT(m ## args, TBAG_PP_EMPTY())
#else
# define TBAG_MAKE_ERR_MSG(...) TBAG_PP_OVERLOAD(TBAG_MAKE_ERR_MSG, __VA_ARGS__)(__VA_ARGS__)
#endif

#include <libtbag/string/Format.hpp>

#ifndef TBAG_ERR_FMT
#define TBAG_ERR_FMT(code, ...) \
    ::libtbag::ErrMsg(TBAG_MAKE_ERR_MSG(::libtbag::string::fformat(__VA_ARGS__), code))
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERRPAIR_HPP__

