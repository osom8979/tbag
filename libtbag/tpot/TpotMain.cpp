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
#include <libtbag/tpot/TpotRunner.hpp>

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
          _help(false), _verbose(false), _unknown(false), _version(false)
{
    _config_path = (res::TpotAsset::get_local_config() / res::TPOT_CONFIG_XML_FILE_NAME).getString();

    using namespace libtbag::signal;
    registerDefaultStdTerminateHandler();
    registerDefaultHandler();

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
            _mode = RunningMode::APPLICATION;
        } else if (COMMAND == std::string(TPOT_MAIN_COMMAND_SERVICE)) {
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
    using namespace libtbag::container;
    using namespace libtbag::tpot::res;
    auto config = Global::getInstance()->insertNewObject<TpotConfig>(TPOT_CONFIG_GLOBAL_NAME);
    assert(static_cast<bool>(config));

    config->add(TpotConfig::SharedNode(new TpotNode()));
    config->add(TpotConfig::SharedNode(new TpotLog()));
    config->loadOrDefaultSave(filesystem::Path(_config_path));
}

void TpotMain::onCreate()
{
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
    }
}

int TpotMain::onRunning()
{
    TpotParams params;
    params.enable_tty = (_mode != RunningMode::SERVICE);
    return TpotRunner(params).run();
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

    if (_version) {
        using namespace libtbag::util;
        std::cout << getTbagVersion().toString() << std::endl;
        return EXIT_SUCCESS;
    }

    if (_mode == RunningMode::SERVICE) {
        if (_verbose) {
            std::cout << "Run service mode.\n";
        }
        return runService(std::string(TPOT_SERVICE_NAME));
    }

    assert(_mode == RunningMode::APPLICATION);
    if (_verbose) {
        std::cout << "Run application mode.\n";
    }
    return run();
}

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

