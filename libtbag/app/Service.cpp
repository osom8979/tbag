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

Service::Service() : _is_start(false)
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

void Service::registerTerminateHandler()
{
    signal::registerFunctionalHandler(signal::TBAG_SIGNAL_TERMINATION, [this](int signal){
        this->onTerminate();
    });
}

Err Service::install()
{
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::install() The service is not ready.");
        return Err::E_ILLSTATE;
    }
    return _service->install();
}

Err Service::uninstall()
{
    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::uninstall() The service is not ready.");
        return Err::E_ILLSTATE;
    }
    return _service->uninstall();
}

Err Service::start()
{
    if (_is_start) {
        tDLogE("Service::start() Already started state.");
        return Err::E_ALREADY;
    }

    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::start() The service is not ready.");
        return Err::E_ILLSTATE;
    }

    Err const START_CODE = _service->start();
    if (isSuccess(START_CODE)) {
        tDLogI("Service::start() Start service.");
        _is_start = true;
    } else {
        tDLogE("Service::start() Start {} error.", getErrName(START_CODE));
    }
    return START_CODE;
}

Err Service::stop()
{
    if (_is_start == false) {
        tDLogE("Service::stop() Already stopped state.");
        return Err::E_ALREADY;
    }

    if (static_cast<bool>(_service) == false) {
        tDLogE("Service::stop() The service is not ready.");
        return Err::E_ILLSTATE;
    }

    Err const STOP_CODE = _service->stop();
    if (isSuccess(STOP_CODE)) {
        tDLogI("Service::stop() Stop service.");
        _is_start = false;
    } else {
        tDLogE("Service::stop() Stop {} error.", getErrName(STOP_CODE));
    }
    return STOP_CODE;
}

void Service::onTerminate()
{
    if (_is_start) {
        stop();
    }
    signal::exitForce(0);
}

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

