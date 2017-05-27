/**
 * @file   FakeService.hpp
 * @brief  FakeService class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_FAKESERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_FAKESERVICE_HPP__

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
 * FakeService class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API FakeService : public ServiceCommon
{
public:
    FakeService(std::string const & name);
    virtual ~FakeService();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_DETAILS_FAKESERVICE_HPP__

