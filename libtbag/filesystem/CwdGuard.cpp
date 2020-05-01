/**
 * @file   CwdGuard.cpp
 * @brief  CwdGuard class implementation.
 * @author zer0
 * @date   2020-05-01
 */

#include <libtbag/filesystem/CwdGuard.hpp>
#include <libtbag/system/SysInfo.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

CwdGuard::CwdGuard(Path const & change_dir) : PREV_DIR(Path::getWorkDir())
{
    auto const code = libtbag::system::changeDirectory(change_dir);
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

CwdGuard::~CwdGuard()
{
    libtbag::system::changeDirectory(PREV_DIR);
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

