/**
 * @file   FakeService.hpp
 * @brief  FakeService class prototype.
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_DETAILS_FAKESERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_DETAILS_FAKESERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/service/details/ServiceInterface.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace service {
namespace details {

/**
 * FakeService class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */
class TBAG_API FakeService : public ServiceCommon
{
public:
    FakeService(std::string const & name);
    virtual ~FakeService();

public:
    Err install() override;
    Err uninstall() override;
    Err start() override;
    Err stop() override;
};

} // namespace details
} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_DETAILS_FAKESERVICE_HPP__

