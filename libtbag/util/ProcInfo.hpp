/**
 * @file   ProcInfo.hpp
 * @brief  ProcInfo class prototype.
 * @author zer0
 * @date   2017-09-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_PROCINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_PROCINFO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * ProcInfo structure.
 *
 * @author zer0
 * @date   2017-09-08
 *
 * @remarks
 *  Process information structure.
 */
struct ProcInfo
{
    int  pid;
    bool active;

    ProcInfo(int p = 0, bool a = false) : pid(p), active(a)
    { /* EMPTY. */ }
    ~ProcInfo()
    { /* EMPTY. */ }
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_PROCINFO_HPP__

