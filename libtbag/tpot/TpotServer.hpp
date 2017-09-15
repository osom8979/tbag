/**
 * @file   TpotServer.hpp
 * @brief  TpotServer class prototype.
 * @author zer0
 * @date   2017-09-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotServer class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TpotServer : private Noncopyable
{
public:
    struct Param
    {
        bool verbose;
        std::string bind;
        int port;
    };

public:
    struct Internal;
    friend struct Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
//    UniqueInternal _internal;

public:
    TpotServer(Param const & param);
    virtual ~TpotServer();

public:
    int run();
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

