/**
 * @file   UnixService.hpp
 * @brief  UnixService class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_UNIXSERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_UNIXSERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/app/details/ServiceInterface.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app     {
namespace details {

/**
 * UnixService class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API UnixService : public ServiceCommon
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

private:
    std::string _pid_path;

public:
    UnixService(std::string const & name);
    virtual ~UnixService();

public:
    virtual Err install() override;
    virtual Err uninstall() override;
    virtual Err start() override;
    virtual Err stop() override;
};

} // namespace details
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_UNIXSERVICE_HPP__

