/**
 * @file   DbgPair.hpp
 * @brief  DbgPair class prototype.
 * @author zer0
 * @date   2020-05-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DBGPAIR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DBGPAIR_HPP__

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
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * DbgPair class prototype.
 *
 * @author zer0
 * @date   2020-05-20
 */
template <typename T>
struct DbgPair
{
    using Val = T;
    using File = std::string;
    using Line = std::size_t;
    using Msg = std::string;

    File file;
    Line line = 0;

    Err code = E_UNKNOWN;
    Msg msg;

    Val value;

    DbgPair()
    { /* EMPTY. */ }

    DbgPair(Err c) : code(c)
    { /* EMPTY. */ }
    DbgPair(Err c, Msg const & m) : code(c), msg(m)
    { /* EMPTY. */ }
    DbgPair(Err c, Msg && m) : code(c), msg(std::move(m))
    { /* EMPTY. */ }

    DbgPair(File const & f, Line l, Err c) : file(f), line(l), code(c)
    { /* EMPTY. */ }
    DbgPair(File const & f, Line l, Err c, Msg const & m) : file(f), line(l), code(c), msg(m)
    { /* EMPTY. */ }
    DbgPair(File const & f, Line l, Err c, Msg && m) : file(f), line(l), code(c), msg(std::move(m))
    { /* EMPTY. */ }

    DbgPair(Val const & v) : code(E_SUCCESS), value(v)
    { /* EMPTY. */ }
    DbgPair(Val && v) : code(E_SUCCESS), value(std::move(v))
    { /* EMPTY. */ }

    DbgPair(File const & f, Line l, Val const & v) : file(f), line(l), code(E_SUCCESS), value(v)
    { /* EMPTY. */ }
    DbgPair(File const & f, Line l, Val && v) : file(f), line(l), code(E_SUCCESS), value(std::move(v))
    { /* EMPTY. */ }

    ~DbgPair()
    { /* EMPTY. */ }

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

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, DbgPair const & dbg)
    {
        if (dbg.file.empty()) {
            os << dbg.name();
        } else {
            os << dbg.file << ":" << dbg.line << " " << dbg.name();
        }
        if (!dbg.msg.empty()) {
            os << "('" << dbg.msg << "')";
        }
        return os;
    }
};

using DbgNull = DbgPair<std::nullptr_t>;
using DbgInfo = DbgNull;

using DbgString = DbgPair<std::string>;
using DbgStr = DbgString;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_DBG_FMT
#define TBAG_DBG_FMT(err, ...) { FILE_STRING, LINE_NUMBER, err, ::libtbag::string::fformat(__VA_ARGS__) }
#endif

#ifndef TBAG_DBG_OK
#define TBAG_DBG_OK(value) { FILE_STRING, LINE_NUMBER, value }
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DBGPAIR_HPP__

