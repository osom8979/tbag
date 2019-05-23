/**
 * @file   ProcessId.hpp
 * @brief  ProcessId class prototype.
 * @author zer0
 * @date   2019-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_PROCESSID_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_PROCESSID_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

TBAG_API long getProcessId();

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_PROCESSID_HPP__

