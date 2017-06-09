/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2017-05-13
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/tpot/TpotCommon.hpp>
#include <libtbag/tpot/client/TpotRequest.hpp>
#include <libtbag/tpot/TpotRunner.hpp>

#include <libtbag/container/Global.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const TPOT_COMMAND_APP       = "app";
TBAG_CONSTEXPR static char const * const TPOT_COMMAND_INSTALL   = "install";
TBAG_CONSTEXPR static char const * const TPOT_COMMAND_UNINSTALL = "uninstall";
TBAG_CONSTEXPR static char const * const TPOT_COMMAND_START     = "start";
TBAG_CONSTEXPR static char const * const TPOT_COMMAND_STOP      = "stop";
TBAG_CONSTEXPR static char const * const TPOT_COMMAND_REQUEST   = "request";

TBAG_CONSTEXPR static char const * const TPOT_COMMAND_HELP_HEAD = "Command list:";

TBAG_CONSTEXPR static char const * const TPOT_SERVICE_NAME = "tpot";

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::ex::ServiceApp(TPOT_DEFAULT_CONFIG_NAME, argc, argv, envs),
          _commands(), _enable_test(false)
{
    _commands.insert(HelpPair(TPOT_COMMAND_APP      , "Normal application mode."));
    _commands.insert(HelpPair(TPOT_COMMAND_INSTALL  ,         "Install service."));
    _commands.insert(HelpPair(TPOT_COMMAND_UNINSTALL,       "Uninstall service."));
    _commands.insert(HelpPair(TPOT_COMMAND_START    ,           "Start service."));
    _commands.insert(HelpPair(TPOT_COMMAND_STOP     ,            "Stop service."));
    _commands.insert(HelpPair(TPOT_COMMAND_REQUEST  ,            "Request mode."));
}

TpotMain::~TpotMain()
{
    // EMPTY.
}

// --------------
// Event methods.
// --------------

bool TpotMain::onCreate()
{
    std::stringstream remarks;
    remarks << std::endl << TPOT_COMMAND_HELP_HEAD << std::endl
            << HelpCommander::getPaddingCommandHelp(std::string(), _commands)
            << getDefaultRemarks();

    installDefaultSynopsis();
    installDefaultRemarks(remarks.str());

    installConfigOptions();
    installHelpOptions();
    installVerboseOptions();
    installVersionOptions(util::getTbagVersion());

    using namespace libtbag::string;
    atOptions().insert("test", [&](Arguments const & args){
        _enable_test = true;
    }, "Use the test mode. (Only request command)");

    auto config = getConfig().lock();
    assert(static_cast<bool>(config));
    config->newAdd<DefaultLogXmlNode>(TPOT_DEFAULT_LOGGER_NAME, TPOT_DEFAULT_LOGGER_FILE_PREFIX);
    config->newAdd<ServerXmlNode>();

    return true;
}

bool TpotMain::onLoadConfig(DefaultXmlModel & config)
{
    auto log = getLogNode().lock();
    assert(static_cast<bool>(log));
    std::string preview_message;
    if (log->createLoggers(preview_message) > 0) {
        if (isEnableVerbose()) {
            std::cout << preview_message << std::endl;
        }
    }

    using namespace libtbag::signal;
    registerDefaultStdTerminateHandler(log::TBAG_DEFAULT_LOGGER_NAME);
    registerDefaultHandler(log::TBAG_DEFAULT_LOGGER_NAME);

    auto server = getServerNode().lock();
    assert(static_cast<bool>(server));
    if (server != nullptr) {
        tDLogI("TpotMain::initConfig() Config (BIND: {}, PORT: {})", server->getIp(), server->getPort());
    } else {
        tDLogW("TpotMain::initConfig() Config is nullptr.");
    }
    return true;
}

int TpotMain::onDefaultCommand(StringVector const & args)
{
    tDLogD("TpotMain::onRunning() BEGIN");

    //if (args[0] == TPOT_COMMAND_APP) {}
    //if (args[0] == TPOT_COMMAND_INSTALL) {}
    //if (args[0] == TPOT_COMMAND_UNINSTALL) {}
    //if (args[0] == TPOT_COMMAND_START) {}
    //if (args[0] == TPOT_COMMAND_STOP) {}
    //if (args[0] == TPOT_COMMAND_REQUEST) {}

    auto node = getServerNode().lock();
    assert(static_cast<bool>(node));

    TpotParams params;
    params.enable_verbose = _enable_verbose;
    params.server_bind = node->getIp();
    params.server_port = node->getPort();
    int const EXIT_CODE = TpotRunner(params).run();

    tDLogD("TpotMain::onRunning() END");
    return EXIT_CODE;
}

void TpotMain::onDestroy()
{
    tDLogD("TpotMain::onDestroy()");
}

TpotMain::WeakLogNode TpotMain::getLogNode()
{
    if (auto config = getConfig().lock()) {
        return config->getWeak<DefaultLogXmlNode>();
    }
    return WeakLogNode();
}

TpotMain::WeakServerNode TpotMain::getServerNode()
{
    if (auto config = getConfig().lock()) {
        return config->getWeak<ServerXmlNode>();
    }
    return WeakServerNode();
}


// ------------
// Entry-point.
// ------------

int runTpotMain(int argc, char ** argv, char ** envs)
{
    return TpotMain(argc, argv, envs).run();
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

