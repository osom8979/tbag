/**
 * @file   Service.hpp
 * @brief  Service class prototype.
 * @author zer0
 * @date   2017-05-25
 * @date   2019-11-05 (Move package: libtbag/app -> libtbag/service)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_SERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_SERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/service/details/ServiceInterface.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace service {

/**
 * Service class prototype.
 *
 * @author zer0
 * @date   2017-05-25
 * @date   2019-11-05 (Move package: libtbag/app -> libtbag/service)
 */
class TBAG_API Service : private Noncopyable
{
public:
    using ServiceInterface = details::ServiceInterface;
    using UniqueService    = std::unique_ptr<ServiceInterface>;

private:
    UniqueService _service;

private:
    bool _is_start;

public:
    Service();
    virtual ~Service();

public:
    Err createService(std::string const & name);

public:
    Err install();
    Err uninstall();
    Err start();
    Err stop();
};

} // namespace service

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SERVICE_SERVICE_HPP__

