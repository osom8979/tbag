/**
 * @file   RotatePath.hpp
 * @brief  RotatePath class prototype.
 * @author zer0
 * @date   2017-07-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 * RotatePath class prototype.
 *
 * @author zer0
 * @date   2017-07-31
 *
 * @remarks
 *  - absolute time.
 *  - file size.
 */
class TBAG_API RotatePath : private Noncopyable
{
public:
    using TimeSpec  = details::TimeSpec;
    using FileState = details::FileState;

public:
    RotatePath();
    virtual ~RotatePath();
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

