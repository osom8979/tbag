/**
 * @file   FakeService.cpp
 * @brief  FakeService class implementation.
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */

#include <libtbag/service/details/FakeService.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace service {
namespace details {

FakeService::FakeService(std::string const & name) : ServiceCommon(name)
{
    // EMPTY.
}

FakeService::~FakeService()
{
    // EMPTY.
}

Err FakeService::install()
{
    return E_SUCCESS;
}

Err FakeService::uninstall()
{
    return E_SUCCESS;
}

Err FakeService::start()
{
    return E_SUCCESS;
}

Err FakeService::stop()
{
    return E_SUCCESS;
}

} // namespace details
} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

