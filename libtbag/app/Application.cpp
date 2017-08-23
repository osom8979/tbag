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
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

Application::Application(int argc, char ** argv, char ** envs)
{
    tbInitialize();
    _property = Global::getInstance()->insertNewObject<Property>(PROPERTY_NAME);
    assert(static_cast<bool>(_property));
    _property->argc = argc;
    _property->argv = argv;
    _property->envs = envs;
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

std::string Application::getCommandLineArgumentsString()
{
    int argc = getArgc();
    char ** argv = getArgv();
    std::stringstream ss;

    for (int i = 0; i < argc; ++i) {
        ss << argv[i];
        if (i + 1 < argc) {
            ss << ' ';
        }
    }

    return ss.str();
}

int Application::run()
{
    if (onCreate() == false) {
        return EXIT_FAILURE;
    }

    int const EXIT_CODE = onRunning();
    onDestroy();

    return EXIT_CODE;
}

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

