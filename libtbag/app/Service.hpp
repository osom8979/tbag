/**
 * @file   Service.hpp
 * @brief  Service class prototype.
 * @author zer0
 * @date   2017-05-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_SERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_SERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/app/Application.hpp>
#include <libtbag/app/details/ServiceInterface.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

/**
 * Service class prototype.
 *
 * @author zer0
 * @date   2017-05-25
 */
class TBAG_API Service : public Application
{
public:
    using ServiceInterface = details::ServiceInterface;
    using UniqueService    = std::unique_ptr<ServiceInterface>;

private:
    UniqueService _service;

public:
    Service(int argc, char ** argv, char ** envs);
    Service(int argc, char ** argv);
    Service();
    virtual ~Service();

public:
    Err createService(std::string const & name);
    void registerTerminateHandler();

public:
    Err install();
    Err uninstall();
    Err start();
    Err stop();

public:
    virtual void onTerminate();
};

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_SERVICE_HPP__

