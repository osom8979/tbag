/**
 * @file   Structures.hpp
 * @brief  Common structures prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_STRUCTURES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_STRUCTURES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/id/generator/TimeId.hpp>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/geometry/Size.hpp>
#include <libtbag/geometry/Rect.hpp>

#include <cstdint>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

using LargeId    = id::LargeId;
using MediumId   = id::MediumId;
using SmallId    = id::SmallId;
using ResultCode = uint32_t;

using Point = geometry::Point;
using Size  = geometry::Size;
using Rect  = geometry::Rect;

TBAG_CONSTEXPR LargeId  const UNKNOWN_LARGE_ID  = id::UNKNOWN_LARGE_ID;
TBAG_CONSTEXPR MediumId const UNKNOWN_MEDIUM_ID = id::UNKNOWN_MEDIUM_ID;
TBAG_CONSTEXPR SmallId  const UNKNOWN_SMALL_ID  = id::UNKNOWN_SMALL_ID;

enum ResultCodeTable
{
    RCT_SUCCESS = 0,
    RCT_FAILURE,
};

inline LargeId genLargeId()
{
    return static_cast<LargeId>(libtbag::id::generator::genTimeId());
}

struct Pair
{
    std::string key;
    std::string val;

    Pair(std::string const k = std::string(), std::string const v = std::string())
            : key(k), val(v)
    { /* EMPTY. */ }
    ~Pair()
    { /* EMPTY. */ }
};

using Pairs = std::vector<Pair>;

struct Header
{
    using Id   = LargeId;
    using Code = ResultCode;

    Id    id;
    Code  code;
    Pairs infos;

    Header() : id(genLargeId()), code(RCT_SUCCESS), infos()
    { /* EMPTY. */ }
    Header(Id i, ResultCode c = RCT_SUCCESS, Pairs const & ifs = Pairs()) : id(i), code(c), infos(ifs)
    { /* EMPTY. */ }
    ~Header()
    { /* EMPTY. */ }
};

struct UserInfo
{
    using Id = MediumId;

    Id id; ///< Registered index number. (Not 'User ID')
    Pairs infos;

    UserInfo(Id i = 0, Pairs const & ifs = Pairs()) : id(i), infos(ifs)
    { /* EMPTY. */ }
    ~UserInfo()
    { /* EMPTY. */ }
};

struct ExecParam
{
    std::string file;
    std::vector<std::string> args;
    std::vector<std::string> envs;
    std::string cwd;
    std::string input;

    ExecParam(std::string const & f = std::string(),
              std::vector<std::string> const & as = std::vector<std::string>(),
              std::vector<std::string> const & es = std::vector<std::string>(),
              std::string const & c = std::string(),
              std::string const & i = std::string()) : file(f), args(as), envs(es), cwd(c), input(i)
    { /* EMPTY. */ }
    ~ExecParam()
    { /* EMPTY. */ }
};

struct ProcessInfo
{
    int  pid;
    bool active;

    Pairs infos;

    ProcessInfo(int p = 0, bool a = false, Pairs const & ifs = Pairs()) : pid(p), active(a), infos(ifs)
    { /* EMPTY. */ }
    ~ProcessInfo()
    { /* EMPTY. */ }
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_STRUCTURES_HPP__

