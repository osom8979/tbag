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

Application::Application(int argc, char ** argv, char ** envs, bool init_tbag)
        : _initialize_tbag(init_tbag)
{
    if (init_tbag) {
        tbInitialize();
    }
    _main_arguments = Global::getInstance()->insertNewObject<MainArguments>(MAIN_ARGUMENTS_NAME);
    assert(static_cast<bool>(_main_arguments));
    _main_arguments->argc = argc;
    _main_arguments->argv = argv;
    _main_arguments->envs = envs;
}

Application::Application(int argc, char ** argv, bool init_tbag)
        : Application(argc, argv, nullptr, init_tbag)
{
    // EMPTY.
}

Application::Application(bool init_tbag)
        : Application(0, nullptr, init_tbag)
{
    // EMPTY.
}

Application::~Application()
{
    _main_arguments.reset();
    if (_initialize_tbag) {
        tbRelease();
    }
}

Application::WeakMainArguments Application::getMainArguments()
{
    return Global::getInstance()->find<MainArguments>(MAIN_ARGUMENTS_NAME);
}

int Application::getArgc()
{
    if (auto shared = getMainArguments().lock()) {
        return shared->argc;
    }
    assert(false && "Expired properties.");
    return 0;
}

char ** Application::getArgv()
{
    if (auto shared = getMainArguments().lock()) {
        return shared->argv;
    }
    assert(false && "Expired properties.");
    return nullptr;
}

char ** Application::getEnvs()
{
    if (auto shared = getMainArguments().lock()) {
        return shared->envs;
    }
    assert(false && "Expired properties.");
    return nullptr;
}

std::vector<std::string> Application::getCommandLineArguments()
{
    int const SIZE = getArgc();
    char ** argv = getArgv();

    std::vector<std::string> result;
    for (int i = 0; i < SIZE; ++i) {
        result.emplace_back(argv[i]);
    }
    return result;
}

std::string Application::getCommandLineArgumentsString()
{
    int const SIZE = getArgc();
    char ** argv = getArgv();
    assert(SIZE >= 1);

    char const SP = ' ';
    std::stringstream ss;
    ss << argv[0];

    for (int i = 1; i < SIZE; ++i) {
        ss << SP << argv[i];
    }
    return ss.str();
}

int Application::run()
{
    if (onCreate() == false) {
        return EXIT_FAILURE;
    }
    int const EXIT_CODE = onRunning(getCommandLineArguments());
    onDestroy();
    return EXIT_CODE;
}

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

