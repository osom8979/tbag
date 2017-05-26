/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2017-05-13
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const TPOT_MAIN_SYNOPSIS = ""
        "Tpot is the process manager.\n\n"
        " Usage: tpot {options} {command}\n"
        ""/* -- END -- */;

TBAG_CONSTEXPR static char const * const TPOT_MAIN_REMARKS = "\n"
        "Command list:\n"
        "app      Normal application mode. [DEFAULT]\n"
        "service  Service(daemon) mode.\n"
        "\n"
        "Configuration file:\n"
        "  When reading, the values are read from the system,\n"
        "  global and repository local configuration files by default,\n"
        "  and options --global, --home, --local and --config={filename}\n"
        "  can be used to tell the command to read from only that location\n"
        ""/* -- END -- */;

TBAG_CONSTEXPR static char const * const TPOT_MAIN_COMMAND_APP = "app";
TBAG_CONSTEXPR static char const * const TPOT_MAIN_COMMAND_SERVICE = "service";

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::Service(argc, argv, envs), _mode(RunningMode::APPLICATION),
          _help(false), _verbose(false), _unknown(false)
{
    initCommander(argc, argv);
}

TpotMain::~TpotMain()
{
    // EMPTY.
}

void TpotMain::initCommander(int argc, char ** argv)
{
    using namespace libtbag::string;

    _commander.setSynopsis(TPOT_MAIN_SYNOPSIS);
    _commander.setRemarks(TPOT_MAIN_REMARKS);

    bool is_call_once = false;
    _commander.setDefaultCallback([&](Arguments const & args){
        if (is_call_once) {
            _unknown = true;
            return;
        }
        std::string const cmd = libtbag::string::lower(args.get(0));
        if (cmd == std::string(TPOT_MAIN_COMMAND_APP)) {
            _mode = RunningMode::APPLICATION;
        } else if (cmd == std::string(TPOT_MAIN_COMMAND_SERVICE)) {
            _mode = RunningMode::SERVICE;
        } else {
            _unknown = true;
        }
        is_call_once = true;
    });

    _commander.insertHelpCommand([&](Arguments const & args){
        _help = true;
    });

    _commander.insert("global", [&](Arguments const & args){
        // TODO: Implement this section.
    }, "Use the global resource. The path is /etc/tpot/config.xml");
    _commander.insert("home", [&](Arguments const & args){
        // TODO: Implement this section.
    }, "Use the home resource. The path is ~/.tpot/config.xml");
    _commander.insert("local", [&](Arguments const & args){
        // TODO: Implement this section.
    }, "Use the local resource. The path is ${EXE_PATH}/config.xml [DEFAULT]");
    _commander.insert("config", [&](Arguments const & args){
        // TODO: Implement this section.
    }, "Use the given config file.", "{filename}");

    _commander.insert("verbose", [&](Arguments const & args){
        _verbose = true;
    }, "Be more verbose/talkative during the operation.");

    bool const IGNORE_FIRST = true;
    _commander.request(argc, argv, IGNORE_FIRST);
}

void TpotMain::onCreate()
{
    // EMPTY.
}

void TpotMain::onRunning()
{
    // EMPTY.
}

void TpotMain::onDestroy()
{
    // EMPTY.
}

int TpotMain::autoRun()
{
    if (_help || _unknown) {
        std::cout << _commander.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (_mode == RunningMode::SERVICE) {
        if (_verbose) {
            std::cout << "Run service mode.\n";
        }
        return runService();
    }

    assert(_mode == RunningMode::APPLICATION);
    if (_verbose) {
        std::cout << "Run application mode.\n";
    }
    return run();
}

// ------------
// Entry-point.
// ------------

int runTpotMain(int argc, char ** argv, char ** envs)
{
    return TpotMain(argc, argv, envs).autoRun();
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

