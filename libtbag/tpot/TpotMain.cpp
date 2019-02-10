/**
 * @file   TpotMain.cpp
 * @brief  TpotMain class implementation.
 * @author zer0
 * @date   2019-01-28
 */

#include <libtbag/tpot/TpotMain.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/app/ex/ServiceApp.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/util/Version.hpp>

#include <cstdlib>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const SYNOPSIS_TEXT =
        "TpoT is a command-line utility for the tbag library.\n"
        "Usage: tpot [options] [application] [args] ...\n";
TBAG_CONSTEXPR static char const * const CONFIG_NAME = "config.xml";
TBAG_CONSTEXPR static char const * const SERVICE_NAME = "tpot";

TpotMain::TpotMain() : TpotMain(0, nullptr)
{
    // EMPTY.
}

TpotMain::TpotMain(int argc, char ** argv) : TpotMain(0, nullptr, nullptr)
{
    // EMPTY.
}

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
{
    using namespace std::placeholders;
    _params.argc = argc;
    _params.argv = argv;
    _params.envs = envs;
    _params.init_tbag = true;
    _params.install_config = true;
    _params.install_help = true;
    _params.install_verbose = true;
    _params.install_synopsis = true;
    _params.synopsis = SYNOPSIS_TEXT;
    _params.install_remarks = true;
    _params.install_create_config = false;
    _params.config_name = CONFIG_NAME;
    _params.install_service = true;
    _params.service_name = SERVICE_NAME;
    _params.version = libtbag::util::getTbagVersion();
    _params.scope = DefaultApp::ConfigScope::GLOBAL;
    _params.options_cb = std::bind(&TpotMain::onOptions, this, _1);
    _params.properties_cb = std::bind(&TpotMain::onInfo, this, _1);
    _params.std_signal = std::bind(&TpotMain::onTerminateSignal, this, _1);

    _manager.registerDefaultPots();

    COMMENT("Update Remarks") {
        std::stringstream ss;
        ss << std::endl << _manager.getRemarks()
           << libtbag::app::ex::ServiceApp::getDefaultRemarks();
        _params.remarks = ss.str();
    }
}

TpotMain::~TpotMain()
{
    // EMPTY.
}

void TpotMain::createDefaultLogger()
{
    if (isWindowsPlatform()) {
        libtbag::log::createDefaultStdoutLogger();
    } else {
        libtbag::log::createDefaultColorStdoutLogger();
    }
    libtbag::log::setDefaultSeverity(libtbag::log::DEBUG_SEVERITY);
}

int TpotMain::run()
{
    using namespace std::placeholders;
    return _app.run(std::bind(&TpotMain::onRun, this, _1), _params);
}

void TpotMain::onTerminateSignal(int signum)
{
}

void TpotMain::onOptions(HelpCommander & commander)
{
    using namespace libtbag::string;
    commander.insert("tlog", [&](Arguments const & args){
        createDefaultLogger();
    }, "Enable tbag log");
}

void TpotMain::onInfo(Element const & element)
{
}

int TpotMain::onRun(RunnerParams const & params)
{
    return _manager.run(params);
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

