/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2017-09-07
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/tpot/TpotServer.hpp>
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

TBAG_CONSTEXPR static char const * const TPOT_NAME = "tpot";

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::ex::ServiceApp(TPOT_NAME, argc, argv, envs),
          _bind(), _port(0)
{
    // EMPTY.
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
    installDefaultSynopsis();
    installDefaultRemarks();

    installConfigOptions();
    installHelpOptions();
    installVerboseOptions();
    installVersionOptions(util::getTbagVersion());

    using namespace libtbag::string;
    atOptions().insert("bind", [&](Arguments const & args){
        if (args.optString(0, &_bind) == false) {
            _bind.clear();
        }
    }, "Assign bind address directly. (If not, refer to the config file)");
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
        tDLogI("TpotMain::initConfig() Config (BIND: {}, PORT: {})", server->getIp(), server->getPort());
    } else {
        tDLogW("TpotMain::initConfig() Config is nullptr.");
    }
    return true;
}

int TpotMain::onDefaultCommand(StringVector const & args)
{
    tDLogD("TpotMain::onDefaultCommand() BEGIN");

    auto node = getServerNode().lock();
    assert(static_cast<bool>(node));

    TpotServer::Param param;
    param.verbose = isEnableVerbose();
    if (_bind.empty() == false) {
        param.bind = _bind;
    } else if (node->getEnable()) {
        param.bind = node->getIp();
    } else {
        param.bind = "0.0.0.0";
    }
    if (_port == 0) {
        param.port = _port;
    } else if (node->getEnable()) {
        param.port = node->getPort();
    } else {
        param.port = 0;
    }

    int const EXIT_CODE = TpotServer(param).run();
    tDLogD("TpotMain::onDefaultCommand() END");
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

