/**
 * @file   Application.cpp
 * @brief  Application class implementation.
 * @author zer0
 * @date   2017-05-13
 */

#include <libtbag/app/Application.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/string/Commander.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <libtbag/libtbag.h>

#include <cstdlib>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

Application::Application(int argc, char ** argv, char ** envs)
{
    tbInitialize();
    _property = Global::getInstance()->insertNewObject<Property>(PROPERTY_NAME);
    onCreate();
}

Application::Application(int argc, char ** argv) : Application(argc, argv, nullptr)
{
    // EMPTY.
}

Application::Application() : Application(0, nullptr)
{
    // EMPTY.
}

Application::~Application()
{
    onDestroy();
    _property.reset();
    tbRelease();
}

Application::WeakProperty Application::getProperty()
{
    return Global::getInstance()->find<Property>(PROPERTY_NAME);
}

int Application::getArgc()
{
    if (auto shared = getProperty().lock()) {
        return shared->argc;
    }
    assert(false && "Expired properties.");
    return 0;
}

char ** Application::getArgv()
{
    if (auto shared = getProperty().lock()) {
        return shared->argv;
    }
    assert(false && "Expired properties.");
    return nullptr;
}

char ** Application::getEnvs()
{
    if (auto shared = getProperty().lock()) {
        return shared->envs;
    }
    assert(false && "Expired properties.");
    return nullptr;
}

void Application::setExitCode(int code)
{
    if (auto shared = getProperty().lock()) {
        shared->exit_code = code;
    }
    assert(false && "Expired properties.");
}

int Application::getExitCode()
{
    if (auto shared = getProperty().lock()) {
        return shared->exit_code;
    }
    assert(false && "Expired properties.");
    return EXIT_FAILURE;
}

int Application::run()
{
    onRunning();
    assert(static_cast<bool>(_property));
    return _property->exit_code;
}

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

