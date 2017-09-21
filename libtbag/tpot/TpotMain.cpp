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

TBAG_CONSTEXPR static char const * const TPOT_NAME    = "tpot";
TBAG_CONSTEXPR static char const * const TPOT_VAR_EXT = ".conf";
TBAG_CONSTEXPR static int const DEFAULT_TIMEOUT_MILLISEC = 10 * 1000;

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::ex::ServiceApp(TPOT_NAME, argc, argv, envs),
          _commands(), _ip(), _port(0), _timeout(0)
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
    atOptions().insertDefault("ip", &_ip, std::string(), "Assign ip address directly. (If not, refer to the config file)");
    atOptions().insertDefault("port", &_port, 0, "Assign port number directly. (If not, refer to the config file)");
    atOptions().insertDefault("timeout", &_timeout, DEFAULT_TIMEOUT_MILLISEC, "Write(request/response) packet timeout.");

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

    using namespace libtbag::filesystem;
    auto var = node->getVar();
    if (var.empty()) {
        Path const CONF_PATH(getConfigPath());
        Path const CONF_DIR = CONF_PATH.getParent() / (std::string(TPOT_NAME) + std::string(TPOT_VAR_EXT));
        var = CONF_DIR.toString();
    }

    int exit_code = EXIT_FAILURE;
    if (args[0] == TPOT_COMMAND_SERVER) {
        TpotServer::Param param;
        param.verbose = isEnableVerbose();
        param.bind = ip;
        param.port = port;
        param.var  = var;
        param.timeout = static_cast<uint64_t>(_timeout);
        param.type = TpotServer::StreamType::TCP;
        exit_code = runTpotServer(param);

    } else if (args[0] == TPOT_COMMAND_REQUEST) {
        TpotClient::Param param;
        param.verbose = isEnableVerbose();
        param.ip = ip;
        param.port = port;
        param.timeout = static_cast<uint64_t>(_timeout);
        param.type = TpotServer::StreamType::TCP;
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

