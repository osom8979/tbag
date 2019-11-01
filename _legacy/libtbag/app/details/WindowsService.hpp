/**
 * @file   WindowsService.hpp
 * @brief  WindowsService class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_WINDOWSSERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_WINDOWSSERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/app/details/ServiceInterface.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app     {
namespace details {

/**
 * WindowsService class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API WindowsService : public ServiceCommon
{
public:
    WindowsService(std::string const & name);
    virtual ~WindowsService();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_WINDOWSSERVICE_HPP__

