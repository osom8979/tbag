/**
 * @file   SimpleApp.cpp
 * @brief  SimpleApp class implementation.
 * @author zer0
 * @date   2018-10-30
 */

#include <libtbag/app/SimpleApp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/libtbag.h>

#include <cstdlib>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

SimpleApp::SimpleApp(int argc, char ** argv, char ** envs, bool init, char const * name)
        : SimpleApp(Param(argc, argv, envs, init, name))
{
    // EMPTY.
}

SimpleApp::SimpleApp(Param const & p) : param(p)
{
    if (param.init) {
        tbInitialize();
    }
}

SimpleApp::~SimpleApp()
{
    if (param.init) {
        tbRelease();
    }
}

bool SimpleApp::onCreate()
{
    using namespace libtbag::filesystem;
    using namespace libtbag::signal;

    registerDefaultStdTerminateHandler();
    registerDefaultHandler();

    help.setSynopsis(std::string("Usage: ") + Path::getExePath().getName() + " {options} commands\n");
    help.setRemarks(std::string());

    help.insertHelpCommand([&](Arguments const & args){
        print_help = true;
    });

    help.insertDefault("verbose", &print_verbose, true, "Be more verbose/talkative during the operation.");
    help.insertDefault("version", &print_version, true, "print the version number and exit.");

    if (!param.name.empty()) {
        help.insertDefault("log_raw"  , &log_raw, true, "Raw log messages.");
        help.insertDefault("log_color", &log_color, true, "Coloring log messages.");
        help.insertDefault("log_level", &log_level, DEFAULT_LOG_LEVEL, "Logging level (0:off ~ 8:debug)", "{level}");
    }

    help.setDefaultCallback([&](Arguments const & args){
        if (!args.getName().empty()) {
            // This block comes when an unknown option is hit.
            print_unknown = true;
            return;
        }

        if (!args.getFull().empty()) {
            commands.push_back(args.getFull());
        } else if (!args.getOriginalArgumentString().empty()) {
            commands.push_back(args.getOriginalArgumentString());
        } else if (!args.getName().empty()) {
            commands.push_back(args.getName());
        }
    });

    return true;
}

void SimpleApp::onDestroy()
{
    if (!param.name.empty()) {
        libtbag::log::removeLogger(param.name);
    }
}

SimpleApp::Version SimpleApp::getVersion()
{
    return Version();
}

int SimpleApp::onMain(Commands const & commands)
{
    return EXIT_SUCCESS;
}

int SimpleApp::run()
{
    if (onCreate()) {
        help.request(param.argc, param.argv);
    } else {
        std::cerr << "Create failure." << std::endl;
        return EXIT_FAILURE;
    }

    if (!param.name.empty()) {
        libtbag::log::Logger * logger = nullptr;
        if (log_raw) {
            logger = libtbag::log::createRawStdoutLogger(param.name);
        } else if (log_color) {
            logger = libtbag::log::createColorStdoutLogger(param.name);
        } else {
            logger = libtbag::log::createStdoutLogger(param.name);
        }

        if (logger != nullptr) {
            libtbag::log::setLevel(param.name, log_level);
        } else {
            std::cerr << "[WARNING] Logger(" << param.name << ") creation failed." << std::endl;
        }
    }

    if (print_help) {
        std::cout << help.help() << std::endl;
        return EXIT_SUCCESS;
    }
    if (print_version) {
        std::cout << getVersion().toString() << std::endl;
        return EXIT_SUCCESS;
    }
    if (print_unknown) {
        std::cerr << "Unknown command." << std::endl;
        return EXIT_FAILURE;
    }

    int const EXIT_CODE = onMain(commands);
    if (EXIT_CODE != EXIT_SUCCESS) {
        std::cerr << "Exit code error: " << EXIT_CODE << std::endl;
    }

    onDestroy();

    return EXIT_CODE;
}

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

