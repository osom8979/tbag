/**
 * @file   CwdGuard.hpp
 * @brief  CwdGuard class prototype.
 * @author zer0
 * @date   2020-05-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_CWDGUARD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_CWDGUARD_HPP__

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
 * CwdGuard class prototype.
 *
 * @author zer0
 * @date   2020-05-01
 */
class TBAG_API CwdGuard TBAG_FINAL : private Noncopyable
{
private:
    Path const PREV_DIR;

public:
    CwdGuard(Path const & change_dir);
    ~CwdGuard();
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_CWDGUARD_HPP__

