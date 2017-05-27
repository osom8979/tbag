/**
 * @file   UnixService.cpp
 * @brief  UnixService class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/app/details/UnixService.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app     {
namespace details {

UnixService::UnixService(std::string const & name) : ServiceCommon(name)
{
    // EMPTY.
}

UnixService::~UnixService()
{
    // EMPTY.
}

Err UnixService::install()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

Err UnixService::uninstall()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

Err UnixService::start()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

Err UnixService::stop()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    return Err::E_ENOSYS;
#else
    return Err::E_ENOSYS;
#endif
}

} // namespace details
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

