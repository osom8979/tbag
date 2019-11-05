/**
 * @file   ServiceInterface.hpp
 * @brief  ServiceInterface class prototype.
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_DETAILS_SERVICEINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_DETAILS_SERVICEINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace service {
namespace details {

/**
 * ServiceInterface class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */
struct ServiceInterface
{
    ServiceInterface() { /* EMPTY.*/ }
    virtual ~ServiceInterface() { /* EMPTY.*/ }

    virtual Err install() = 0;
    virtual Err uninstall() = 0;
    virtual Err start() = 0;
    virtual Err stop() = 0;
};

/**
 * ServiceCommon class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 * @date   2019-11-05 (Move package: libtbag/app/details -> libtbag/service/details)
 */
class ServiceCommon : public ServiceInterface
{
private:
    std::string _name;

public:
    ServiceCommon(std::string const & name) : _name(name)
    { /* EMPTY. */ }
    virtual ~ServiceCommon()
    { /* EMPTY. */ }

public:
    virtual std::string getName() const
    { return _name; }
};

} // namespace details
} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_DETAILS_SERVICEINTERFACE_HPP__

