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
#include <libtbag/uvpp/Process.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

/**
 * TpotRequest class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API TpotRequest
{
public:
    using Process = uvpp::Process;
    using Options = Process::Options;
//        using String  = std::string;
//        using Strings = std::vector<String>;
//        using Stdios  = std::vector<StdioContainer>;
//
//        String file; ///< Executable file path.
//        String cwd;  ///< Working directory.
//
//        Strings args; ///< Arguments.
//        Strings envs; ///< Environment variables (e.g. VAR=VALUE).
//
//        Stdios stdios;
//
//        // Changing the UID/GID is only supported on Unix.
//        uuser  uid;
//        ugroup gid;
//
//        bool setuid; ///< Set the child process' user id.
//        bool setgid; ///< Set the child process' group id.
//        bool detached;
//        bool verbatim_args;
//        bool hide;

public:
public:
    TpotRequest();
    TpotRequest(TpotRequest const & obj);
    TpotRequest(TpotRequest && obj);
    virtual ~TpotRequest();

public:
    TpotRequest & operator =(TpotRequest const & obj);
    TpotRequest & operator =(TpotRequest && obj);
};

// ------------
// Entry-point.
// ------------

TBAG_API int runTpotRequestWithInteractiveMode();

} // namespace client
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__

