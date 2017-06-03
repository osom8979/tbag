/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2017-05-13
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/tpot/TpotCommon.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

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

TBAG_CONSTEXPR char const * const TPOT_SERVICE_NAME = "tpot";

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : app::ex::ServiceApp(TPOT_DEFAULT_CONFIG_NAME, argc, argv, envs),
          _enable_test(false)
{
    // EMPTY.
}

TpotMain::~TpotMain()
{
    // EMPTY.
}

//int TpotMain::runRequest()
//{
//    if (_test) {
//        if (_verbose) { std::cout << "Run request test mode.\n"; }
//        return client::runTpotRequestWithTest();
//    } else {
//        if (_verbose) { std::cout << "Run request mode.\n"; }
//        return client::runTpotRequest();
//    }
//}

// --------------
// Event methods.
// --------------

bool TpotMain::onCreate()
{
    tDLogD("TpotMain::onCreate()");

    installDefaultSynopsis();
    installDefaultRemarks();

    installDefaultCommand();
    installDefaultOptions();

    using namespace libtbag::string;
    atOptions().insert("test", [&](Arguments const & args){
        _enable_test = true;
    }, "Use the test mode. (Only request command)");

    installDefaultLogNode(TPOT_DEFAULT_LOGGER_NAME, TPOT_DEFAULT_LOGGER_FILE_PREFIX);
    installDefaultServerNode();

    installDefaultVersion(LIBTBAG_VERSION_MAJOR, LIBTBAG_VERSION_MINOR, LIBTBAG_VERSION_PATCH);
    return true;
}

bool TpotMain::onLoadConfig()
{
    tDLogD("TpotMain::onLoadConfig()");

    createLoggers();
    registerDefaultSignalHandler();

    auto node = getServerNode().lock();
    assert(static_cast<bool>(node));
    if (node != nullptr) {
        tDLogI("TpotMain::initConfig() Config (BIND: {}, PORT: {})", node->getIp(), node->getPort());
    } else {
        tDLogW("TpotMain::initConfig() Config is nullptr.");
    }
    return true;
}

int TpotMain::onRunning()
{
    tDLogD("TpotMain::onRunning() BEGIN");

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

int TpotMain::onDefaultCommand(StringVector const & args)
{
    tDLogD("TpotMain::onDefaultCommand()");

    for (auto & i : args) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
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

