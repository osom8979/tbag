/**
 * @file   WindowsService.cpp
 * @brief  WindowsService class implementation.
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */

#include <libtbag/service/details/WindowsService.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace service {
namespace details {

WindowsService::WindowsService(std::string const & name) : ServiceCommon(name)
{
    // EMPTY.
}

WindowsService::~WindowsService()
{
    // EMPTY.
}

Err WindowsService::install()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return E_ENOSYS;
#else
    return E_ENOSYS;
#endif
}

Err WindowsService::uninstall()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return E_ENOSYS;
#else
    return E_ENOSYS;
#endif
}

Err WindowsService::start()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return E_ENOSYS;
#else
    return E_ENOSYS;
#endif
}

Err WindowsService::stop()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return E_ENOSYS;
#else
    return E_ENOSYS;
#endif
}

} // namespace details
} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

