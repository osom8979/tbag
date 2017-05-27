/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2017-05-13
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/tpot/res/TpotAsset.hpp>
#include <libtbag/tpot/client/TpotRequest.hpp>
#include <libtbag/tpot/TpotRunner.hpp>

#include <cassert>
#include <iostream>

#define TPOT_MAIN_COMMAND_APP       "app"
#define TPOT_MAIN_COMMAND_INSTALL   "install"
#define TPOT_MAIN_COMMAND_UNINSTALL "uninstall"
#define TPOT_MAIN_COMMAND_START     "start"
#define TPOT_MAIN_COMMAND_STOP      "stop"
#define TPOT_MAIN_COMMAND_REQUEST   "request"

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
        TPOT_MAIN_COMMAND_APP "        Normal application mode. [DEFAULT]\n"
        TPOT_MAIN_COMMAND_INSTALL "    Install service.\n"
        TPOT_MAIN_COMMAND_UNINSTALL "  Uninstall service.\n"
        TPOT_MAIN_COMMAND_START "      Start service.\n"
        TPOT_MAIN_COMMAND_STOP "       Stop service.\n"
        TPOT_MAIN_COMMAND_REQUEST "    Request mode.\n"
        "\n"
        "Configuration file:\n"
        "  When reading, the values are read from the system,\n"
        "  global and repository local configuration files by default,\n"
        "  and options --global, --home, --local and --config={filename}\n"
        "  can be used to tell the command to read from only that location\n"
        ""/* -- END -- */;

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::Service(argc, argv, envs), _mode(RunningMode::RUN_APPLICATION),
          _help(false), _verbose(false), _unknown(false), _version(false)
{
    _config_path = (res::TpotAsset::get_local_config() / res::TPOT_CONFIG_XML_FILE_NAME).getString();

    initCommander(argc, argv);
    initConfig();
}

TpotMain::~TpotMain()
{
    // EMPTY.
}

void TpotMain::initCommander(int argc, char ** argv)
{
    using namespace libtbag::string;
    using namespace libtbag::log;

    _commander.setSynopsis(TPOT_MAIN_SYNOPSIS);
    _commander.setRemarks(TPOT_MAIN_REMARKS);

    bool is_call_once = false;
    _commander.setDefaultCallback([&](Arguments const & args){
        if (is_call_once) {
            _unknown = true;
            return;
        }

        if (args.empty()) {
            // Skip if not found arguments.
            _unknown = true;
            return;
        }
        std::string const COMMAND = libtbag::string::lower(args.get(0));

        if (COMMAND == std::string(TPOT_MAIN_COMMAND_APP)) {
            _mode = RunningMode::RUN_APPLICATION;
        } else if (COMMAND == std::string(TPOT_MAIN_COMMAND_INSTALL)) {
            _mode = RunningMode::RUN_INSTALL;
        } else if (COMMAND == std::string(TPOT_MAIN_COMMAND_UNINSTALL)) {
            _mode = RunningMode::RUN_UNINSTALL;
        } else if (COMMAND == std::string(TPOT_MAIN_COMMAND_START)) {
            _mode = RunningMode::RUN_START;
        } else if (COMMAND == std::string(TPOT_MAIN_COMMAND_STOP)) {
            _mode = RunningMode::RUN_STOP;
        } else if (COMMAND == std::string(TPOT_MAIN_COMMAND_REQUEST)) {
            _mode = RunningMode::RUN_REQUEST;
        } else {
            _unknown = true;
        }
        is_call_once = true;
    });

    _commander.insert("global", [&](Arguments const & args){
        _config_path = (res::TpotAsset::get_global_config() / res::TPOT_CONFIG_XML_FILE_NAME).getString();
    }, "Use the global resource. The path is /etc/tpot/tpot.xml");
    _commander.insert("home", [&](Arguments const & args){
        _config_path = (res::TpotAsset::get_home_config() / res::TPOT_CONFIG_XML_FILE_NAME).getString();
    }, "Use the home resource. The path is ~/.tpot/tpot.xml");
    _commander.insert("local", [&](Arguments const & args){
        _config_path = (res::TpotAsset::get_local_config() / res::TPOT_CONFIG_XML_FILE_NAME).getString();
    }, "Use the local resource. The path is ${EXE_PATH}/config.xml [DEFAULT]");
    _commander.insert("config", [&](Arguments const & args){
        if (args.empty() == false) {
            _config_path = args.get(0);
        } else {
            _unknown = true;
        }
    }, "Use the given config file.", "{filename}");

    _commander.insertHelpCommand([&](Arguments const & args){
        _help = true;
    });
    _commander.insert("verbose", [&](Arguments const & args){
        _verbose = true;
    }, "Be more verbose/talkative during the operation.");
    _commander.insert("version", [&](Arguments const & args){
        _version = true;
    }, "print the tpot(tbag) version number and exit.");

    bool const IGNORE_FIRST = true;
    _commander.request(argc, argv, IGNORE_FIRST);
}

void TpotMain::initConfig()
{
    if (_verbose) {
        std::cout << "Config file path: " << _config_path << std::endl;
    }

    using namespace libtbag::container;
    using namespace libtbag::tpot::res;
    auto config = Global::getInstance()->insertNewObject<TpotConfig>(TPOT_CONFIG_GLOBAL_NAME);
    assert(static_cast<bool>(config));

    config->add(TpotConfig::SharedNode(new TpotNode()));
    config->add(TpotConfig::SharedNode(new TpotLog()));

    bool const RESULT = config->loadOrDefaultSave(filesystem::Path(_config_path));
    if (RESULT == false) {
        std::cout << "config.loadOrDefaultSave() failure.\n";
    }
}

int TpotMain::autoRun()
{
    if (_help || _unknown) {
        std::cout << _commander.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (_version) {
        using namespace libtbag::util;
        std::cout << getTbagVersion().toString() << std::endl;
        return EXIT_SUCCESS;
    }

    // @formatter:off
    switch (_mode) {
    case RunningMode::RUN_REQUEST:      return runRequest();
    case RunningMode::RUN_APPLICATION:  return runApplication();
    case RunningMode::RUN_INSTALL:      return runServiceInstall();
    case RunningMode::RUN_UNINSTALL:    return runServiceUninstall();
    case RunningMode::RUN_START:        return runServiceStart();
    case RunningMode::RUN_STOP:         return runServiceStop();
    }
    // @formatter:on

    assert(false && "Unknown mode.");
    return EXIT_FAILURE;
}

int TpotMain::runRequest()
{
    if (_verbose) {
        std::cout << "Run request mode.\n";
    }
    return client::runTpotRequestWithInteractiveMode();
}

int TpotMain::runApplication()
{
    if (_verbose) {
        std::cout << "Run application mode.\n";
    }
    return run();
}

int TpotMain::runServiceInstall()
{
    if (_verbose) {
        std::cout << "Run service install mode.\n";
    }
    return EXIT_FAILURE;
}

int TpotMain::runServiceUninstall()
{
    if (_verbose) {
        std::cout << "Run service uninstall mode.\n";
    }
    return EXIT_FAILURE;
}

int TpotMain::runServiceStart()
{
    if (_verbose) {
        std::cout << "Run service start mode.\n";
    }
    return EXIT_FAILURE;
}

int TpotMain::runServiceStop()
{
    if (_verbose) {
        std::cout << "Run service stop mode.\n";
    }
    return EXIT_FAILURE;
}

// ---------------
// Static methods.
// ---------------

TpotMain::TpotLog * TpotMain::getTpotLogPointer()
{
    using namespace libtbag::container;
    using namespace libtbag::tpot::res;
    auto config = Global::getInstance()->find<TpotConfig>(TPOT_CONFIG_GLOBAL_NAME);
    if (auto shared = config.lock()) {
        return shared->getPointer<TpotLog>();
    }
    return nullptr;
}

TpotMain::TpotNode * TpotMain::getTpotNodePointer()
{
    using namespace libtbag::container;
    using namespace libtbag::tpot::res;
    auto config = Global::getInstance()->find<TpotConfig>(TPOT_CONFIG_GLOBAL_NAME);
    if (auto shared = config.lock()) {
        return shared->getPointer<TpotNode>();
    }
    return nullptr;
}

// --------------
// Event methods.
// --------------

bool TpotMain::onCreate()
{
    tDLogD("TpotMain::onCreate()");

    TpotLog * log = getTpotLogPointer();
    assert(log != nullptr);

    if (log->createLoggers() >= 1) {
        auto const NAMES = log->getNames();
        std::size_t const NAMES_SIZE = NAMES.size();
        assert(NAMES_SIZE >= 1);

        std::stringstream ss;
        ss << NAMES[0];
        for (std::size_t i = 1; i < NAMES_SIZE; ++i) {
            ss << "," << NAMES[i];
        }
        tDLogI("TpotMain::initConfig() log->createLoggers() success ({}).", ss.str());
    }

    TpotNode * node = getTpotNodePointer();
    assert(node != nullptr);

    if (node != nullptr) {
        tDLogI("App::initConfig() Config (BIND: {}, PORT: {})", node->getBind(), node->getPort());
    } else {
        tDLogE("App::initConfig() Config is nullptr.");
        return false;
    }

    if (_mode == RunningMode::RUN_APPLICATION) {
        libtbag::signal::registerDefaultStdTerminateHandler();
        libtbag::signal::registerDefaultHandler();
    }

    return true;
}

int TpotMain::onRunning()
{
    tDLogD("TpotMain::onRunning() BEGIN");
    TpotParams params;
    params.enable_service = (_mode != RunningMode::RUN_APPLICATION);
    params.enable_verbose = _verbose;
    params.server_bind = getTpotNodePointer()->getBind();
    params.server_port = getTpotNodePointer()->getPort();
    int const EXIT_CODE = TpotRunner(params).run();

    tDLogD("TpotMain::onRunning() END");
    return EXIT_CODE;
}

void TpotMain::onDestroy()
{
    tDLogD("TpotMain::onDestroy()");
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

