/**
 * @file   FakeService.cpp
 * @brief  FakeService class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/app/details/FakeService.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app     {
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
    return Err::E_SUCCESS;
}

Err FakeService::uninstall()
{
    return Err::E_SUCCESS;
}

Err FakeService::start()
{
    return Err::E_SUCCESS;
}

Err FakeService::stop()
{
    return Err::E_SUCCESS;
}

} // namespace details
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
