/**
 * @file   Service.cpp
 * @brief  Service class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/app/Service.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/app/details/UnixService.hpp>
#include <libtbag/app/details/WindowsService.hpp>
#include <libtbag/app/details/FakeService.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

#if defined(TBAG_PLATFORM_WINDOWS)
using RealService = details::WindowsService;
#elif defined(TBAG_PLATFORM_UNIX_LIKE)
using RealService = details::UnixService;
#else
using RealService = details::FakeService;
#endif

Service::Service(int argc, char ** argv, char ** envs) : Application(argc, argv, envs)
{
    // EMPTY.
}

Service::Service(int argc, char ** argv) : Application(argc, argv)
{
    // EMPTY.
}

Service::Service() : Application()
{
    // EMPTY.
}

Service::~Service()
{
    // EMPTY.
}

Err Service::createService(std::string const & name)
{
    if (static_cast<bool>(_service)) {
        tDLogW("Service::createService() Already service object.");
        return Err::E_ALREADY;
    }

    _service.reset(new (std::nothrow) RealService(name));
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::createService() Bad allocation error.");
        return Err::E_BADALLOC;
    }
    return Err::E_SUCCESS;
}

Err Service::install()
{
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::install() The service is not ready.");
        return Err::E_ISTATE;
    }
    return _service->install();
}

Err Service::uninstall()
{
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::uninstall() The service is not ready.");
        return Err::E_ISTATE;
    }
    return _service->uninstall();
}

Err Service::start()
{
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::start() The service is not ready.");
        return Err::E_ISTATE;
    }
    return _service->start();
}

Err Service::stop()
{
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::stop() The service is not ready.");
        return Err::E_ISTATE;
    }
    return _service->stop();
}

int Service::runService(std::string const & name)
{
    onCreate();
    onRunning();
    onDestroy();

    return getExitCode();
}

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

