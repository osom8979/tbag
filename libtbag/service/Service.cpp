/**
 * @file   Service.cpp
 * @brief  Service class implementation.
 * @author zer0
 * @date   2017-05-25
 * @date   2019-11-05 (Move package: libtbag/app -> libtbag/service)
 */

#include <libtbag/service/Service.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/service/details/UnixService.hpp>
#include <libtbag/service/details/WindowsService.hpp>
#include <libtbag/service/details/FakeService.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace service {

#if defined(TBAG_PLATFORM_WINDOWS)
using RealService = libtbag::service::details::WindowsService;
#elif defined(TBAG_PLATFORM_UNIX_LIKE)
using RealService = libtbag::service::details::UnixService;
#else
using RealService = libtbag::service::details::FakeService;
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
    if (_service) {
        tDLogW("Service::createService() Already service object.");
        return E_ALREADY;
    }

    _service.reset(new (std::nothrow) RealService(name));
    if (!_service) {
        tDLogE("Service::createService() Bad allocation error.");
        return E_BADALLOC;
    }
    return E_SUCCESS;
}

Err Service::install()
{
    if (!_service) {
        tDLogE("Service::install() The service is not ready.");
        return E_ILLSTATE;
    }
    return _service->install();
}

Err Service::uninstall()
{
    if (!_service) {
        tDLogE("Service::uninstall() The service is not ready.");
        return E_ILLSTATE;
    }
    return _service->uninstall();
}

Err Service::start()
{
    if (_is_start) {
        tDLogE("Service::start() Already started state.");
        return E_ALREADY;
    }

    if (!_service) {
        tDLogE("Service::start() The service is not ready.");
        return E_ILLSTATE;
    }

    auto const code = _service->start();
    if (isSuccess(code)) {
        tDLogI("Service::start() Start service.");
        _is_start = true;
    } else {
        tDLogE("Service::start() Start {} error.", code);
    }
    return code;
}

Err Service::stop()
{
    if (!_is_start) {
        tDLogE("Service::stop() Already stopped state.");
        return E_ALREADY;
    }

    if (!_service) {
        tDLogE("Service::stop() The service is not ready.");
        return E_ILLSTATE;
    }

    auto const code = _service->stop();
    if (isSuccess(code)) {
        tDLogI("Service::stop() Stop service.");
        _is_start = false;
    } else {
        tDLogE("Service::stop() Stop {} error.", code);
    }
    return code;
}

} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

