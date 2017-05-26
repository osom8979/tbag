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
    Service(int argc, char ** argv, char ** envs);
    Service(int argc, char ** argv);
    Service();
    virtual ~Service();

public:
    int runService();
};

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_SERVICE_HPP__

