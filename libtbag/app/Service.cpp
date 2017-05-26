/**
 * @file   Service.cpp
 * @brief  Service class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/app/Service.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

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

int Service::runService()
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

