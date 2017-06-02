/**
 * @file   TpotCommon.hpp
 * @brief  TpotCommon class prototype.
 * @author zer0
 * @date   2017-06-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR char const * const TPOT_DEFAULT_LOGGER_NAME = log::TBAG_DEFAULT_LOGGER_NAME;
TBAG_CONSTEXPR char const * const TPOT_DEFAULT_LOGGER_FILE_PREFIX = "tpot";

TBAG_CONSTEXPR char const * const TPOT_CONFIG_GLOBAL_OBJECT_KEY = "tpot-config";
TBAG_CONSTEXPR char const * const TPOT_DEFAULT_CONFIG_ROOT_NAME = "tpot";

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTCOMMON_HPP__

