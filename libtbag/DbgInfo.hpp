/**
 * @file   DbgInfo.hpp
 * @brief  DbgInfo class prototype.
 * @author zer0
 * @date   2020-05-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DBGINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DBGINFO_HPP__

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
 * DbgInfo class prototype.
 *
 * @author zer0
 * @date   2020-05-20
 */
struct DbgInfo
{
    using File = std::string;
    using Line = std::size_t;
    using Msg = std::string;

    File file;
    Line line = 0;

    Err code = E_UNKNOWN;
    Msg msg;

    DbgInfo()
    { /* EMPTY. */ }
    DbgInfo(Err c) : code(c)
    { /* EMPTY. */ }
    DbgInfo(Err c, Msg const & m) : code(c), msg(m)
    { /* EMPTY. */ }
    DbgInfo(Err c, Msg && m) : code(c), msg(std::move(m))
    { /* EMPTY. */ }

    DbgInfo(File const & f, Line l)
            : file(f), line(l)
    { /* EMPTY. */ }
    DbgInfo(File const & f, Line l, Err c)
            : file(f), line(l), code(c)
    { /* EMPTY. */ }
    DbgInfo(File const & f, Line l, Err c, Msg const & m)
            : file(f), line(l), code(c), msg(m)
    { /* EMPTY. */ }
    DbgInfo(File const & f, Line l, Err c, Msg && m)
            : file(f), line(l), code(c), msg(std::move(m))
    { /* EMPTY. */ }

    DbgInfo(DbgInfo const & obj)
            : file(obj.file),
              line(obj.line),
              code(obj.code),
              msg(obj.msg)
    { /* EMPTY. */ }
    DbgInfo(DbgInfo && obj) TBAG_NOEXCEPT
            : file(std::move(obj.file)),
              line(obj.line),
              code(obj.code),
              msg(std::move(obj.msg))
    { /* EMPTY. */ }

    ~DbgInfo()
    { /* EMPTY. */ }

    DbgInfo & operator =(DbgInfo const & obj)
    {
        if (this != &obj) {
            file = obj.file;
            line = obj.line;
            code = obj.code;
            msg = obj.msg;
        }
        return *this;
    }

    DbgInfo & operator =(DbgInfo && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            file = std::move(obj.file);
            line = obj.line;
            code = obj.code;
            msg = std::move(obj.msg);
        }
        return *this;
    }

    void swap(DbgInfo & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            file.swap(obj.file);
            std::swap(line, obj.line);
            std::swap(code, obj.code);
            msg.swap(obj.msg);
        }
    }

    friend void swap(DbgInfo & lh, DbgInfo & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
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

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, DbgInfo const & info)
    {
        if (info.file.empty()) {
            os << info.name();
        } else {
            os << info.file << ":" << info.line << " " << info.name();
        }
        if (!info.msg.empty()) {
            os << "(" << info.msg << ")";
        }
        return os;
    }

    template <typename ... Args>
    void format(std::string const & f, Args && ... args)
    {
        msg = libtbag::string::fformat(f, std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    static DbgInfo createFmt(File const & file, Line line, Err code, std::string const & f, Args && ... args)
    {
        DbgInfo result(file, line, code);
        result.format(f, std::forward<Args>(args) ...);
        return result;
    }

    template <typename ... Args>
    static DbgInfo createFmt(Err code, std::string const & f, Args && ... args)
    {
        DbgInfo result(code);
        result.format(f, std::forward<Args>(args) ...);
        return result;
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_DBG_FMT
#define TBAG_DBG_FMT(err, ...) ::libtbag::DbgInfo::createFmt(FILE_STRING, LINE_NUMBER, err, __VA_ARGS__)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DBGINFO_HPP__

