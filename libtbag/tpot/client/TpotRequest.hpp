/**
 * @file   TpotRequest.hpp
 * @brief  TpotRequest class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/structure/TpotProtocol.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

// ------------
// Entry-point.
// ------------

TBAG_API int runTpotRequest(std::string const & uri, structure::Exec const & exec);
TBAG_API int runTpotRequest(std::string const & uri,
                            std::string const & file,
                            std::string const & cwd,
                            std::vector<std::string> const & args,
                            std::vector<std::string> const & envs,
                            uint32_t uid = 0,
                            uint32_t gid = 0);
TBAG_API int runTpotRequest();
TBAG_API int runTpotRequestWithTest();

} // namespace client
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__

