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

void Application::setExitCode(int code)
{
    assert(static_cast<bool>(_property));
    _property->exit_code = code;
}

int Application::getExitCode()
{
    assert(static_cast<bool>(_property));
    return _property->exit_code;
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

