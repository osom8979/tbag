/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2017-09-07
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/tpot/TpotServer.hpp>
#include <libtbag/tpot/TpotClient.hpp>

#include <libtbag/container/Global.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/util/Version.hpp>

#include <cassert>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const TPOT_COMMAND_SERVER  = "server";
TBAG_CONSTEXPR static char const * const TPOT_COMMAND_REQUEST = "request";

TBAG_CONSTEXPR static char const * const TPOT_NAME = "tpot";

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::ex::ServiceApp(TPOT_NAME, argc, argv, envs),
          _commands(), _ip(), _port(0)
{
    _commands.insert(HelpPair(TPOT_COMMAND_SERVER , "TpoT server mode."));
    _commands.insert(HelpPair(TPOT_COMMAND_REQUEST, "TpoT request mode."));
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
    remarks << std::endl << "Command list:" << std::endl
            << HelpCommander::getPaddingCommandHelp(std::string(), _commands)
            << getDefaultRemarks();

    installDefaultSynopsis();
    installDefaultRemarks(remarks.str());

    installConfigOptions();
    installHelpOptions();
    installVerboseOptions();
    installVersionOptions(util::getTbagVersion());

    using namespace libtbag::string;
    atOptions().insert("ip", [&](Arguments const & args){
        if (args.optString(0, &_ip) == false) {
            _ip.clear();
        }
    }, "Assign ip address directly. (If not, refer to the config file)");
    atOptions().insert("port", [&](Arguments const & args){
        if (args.optInteger(0, &_port) == false) {
            _port = 0;
        }
    }, "Assign port number directly. (If not, refer to the config file)");

    auto config = getConfig().lock();
    assert(static_cast<bool>(config));
    config->newAdd<DefaultLogXmlNode>(log::TBAG_DEFAULT_LOGGER_NAME, TPOT_NAME);
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
        tDLogIfI(isEnableVerbose(), "TpotMain::initConfig() Config (BIND: {}, PORT: {})", server->getIp(), server->getPort());
    } else {
        tDLogW("TpotMain::initConfig() Config is nullptr.");
    }
    return true;
}

int TpotMain::onDefaultCommand(StringVector const & args)
{
    if (args.empty()) {
        std::cerr << "Not found command.\n";
        return EXIT_FAILURE;
    }

    tDLogIfD(isEnableVerbose(), "TpotMain::onDefaultCommand() BEGIN");

    auto node = getServerNode().lock();
    assert(static_cast<bool>(node));

    std::string ip;
    int port;

    if (_ip.empty() == false) {
        ip = _ip;
    } else if (node->getEnable()) {
        ip = node->getIp();
    } else {
        ip = "0.0.0.0";
    }

    if (_port != 0) {
        port = _port;
    } else if (node->getEnable()) {
        port = node->getPort();
    } else {
        port = 0;
    }

    int exit_code = EXIT_FAILURE;
    if (args[0] == TPOT_COMMAND_SERVER) {
        TpotServer::Param param;
        param.verbose = isEnableVerbose();
        param.bind = ip;
        param.port = port;
        exit_code = TpotServer(param).run();

    } else if (args[0] == TPOT_COMMAND_REQUEST) {
        TpotClient::Param param;
        param.verbose = isEnableVerbose();
        param.ip = ip;
        param.port = port;
        exit_code = requestTpotClient(param, std::vector<std::string>(args.begin() + 1, args.end()));

    } else {
        std::cerr << "Unknown command: " << args[0] << std::endl;
    }

    tDLogIfD(isEnableVerbose(), "TpotMain::onDefaultCommand() END ({})", exit_code);
    return exit_code;
}

void TpotMain::onDestroy()
{
    tDLogIfD(isEnableVerbose(), "TpotMain::onDestroy()");
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

