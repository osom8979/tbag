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
        "Usage: tpot [options] [command] [command_sub_args] ...\n";
TBAG_CONSTEXPR static char const * const CONFIG_NAME = "tpot.xml";
TBAG_CONSTEXPR static char const * const SERVICE_NAME = "tpot";

TBAG_CONSTEXPR static bool __is_default_tpot_mode_is_luajit() TBAG_NOEXCEPT
{
#if defined(DISABLE_DEFAULT_TPOT_LUAJIT)
    return false;
#else
    return true;
#endif
}

TpotMain::TpotMain() : TpotMain(0, nullptr)
{
    // EMPTY.
}

TpotMain::TpotMain(int argc, char ** argv)
        : TpotMain(argc, argv, nullptr)
{
    // EMPTY.
}

TpotMain::TpotMain(int argc, char ** argv, char ** envs)
        : TpotMain(SERVICE_NAME, CONFIG_NAME, SYNOPSIS_TEXT, argc, argv, envs)
{
    // EMPTY.
}

TpotMain::TpotMain(std::string const & service_name, std::string const & config_name, std::string const & synopsis,
                   int argc, char ** argv, char ** envs)
        : TpotMain(service_name, config_name, synopsis, libtbag::util::getTbagVersion(),
                   DefaultApp::ConfigScope::EXE, Pots(), std::string(), argc, argv, envs)
{
    // EMPTY.
}

TpotMain::TpotMain(std::string const & service_name, std::string const & config_name, std::string const & synopsis,
                   Version const & version, DefaultApp::ConfigScope scope, Pots const & pots, std::string const & default_app,
                   int argc, char ** argv, char ** envs)
        : _default_app(default_app)
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
    _params.synopsis = synopsis;
    _params.install_remarks = true;
    _params.install_create_config = true;
    _params.config_name = config_name;
    _params.install_service = true;
    _params.service_name = service_name;
    _params.version = version;
    _params.scope = scope;
    _params.options_cb = std::bind(&TpotMain::onOptions, this, _1);
    _params.properties_cb = std::bind(&TpotMain::onInfo, this, _1);
    _params.std_signal = std::bind(&TpotMain::onTerminateSignal, this, _1);

    if (pots.empty()) {
        _manager.registerDefaultPots();
    } else {
        _manager.pots() = pots;
    }

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
    // EMPTY.
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
    // EMPTY.
}

void TpotMain::printParamsInfo(RunnerParams const & params) const
{
    auto const & STORAGE_INFO = params.storage.getInfo();
    tDLogIfD(params.verbose && !STORAGE_INFO.empty(), "TpotMain::printParamsInfo()\n{}", STORAGE_INFO);
}

int TpotMain::onRun(RunnerParams const & params)
{
    if (params.verbose) {
        printParamsInfo(params);
    }

    if (!_default_app.empty()) {
        return _manager.run(params, _default_app);
    } else if (__is_default_tpot_mode_is_luajit()) {
        return _manager.runOrLutjit(params);
    } else {
        return _manager.run(params);
    }
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

