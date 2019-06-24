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
#include <algorithm>
#include <utility>
#include <iostream>

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

TpotMain::TpotMain(std::string const & service_name,
                   std::string const & config_name,
                   std::string const & synopsis,
                   int argc, char ** argv, char ** envs)
        : TpotMain(service_name, config_name, synopsis, libtbag::util::getTbagVersion(),
                   DefaultApp::ConfigScope::EXE, std::string(), std::string(), std::string(),
                   Pots(), std::string(), argc, argv, envs)
{
    // EMPTY.
}

TpotMain::TpotMain(std::string const & service_name,
                   std::string const & config_name,
                   std::string const & synopsis,
                   Version const & version,
                   DefaultApp::ConfigScope scope,
                   std::string const & local_scope_config_path,
                   std::string const & home_scope_config_path,
                   std::string const & global_scope_config_path,
                   Pots const & pots,
                   std::string const & default_app,
                   int argc, char ** argv, char ** envs)
        : _default_app(default_app), _help_app()
{
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
    _params.local_scope_config_path = local_scope_config_path;
    _params.home_scope_config_path = home_scope_config_path;
    _params.global_scope_config_path = global_scope_config_path;
    updateDefaultCallbacks();

    if (pots.empty()) {
        _manager.registerDefaultPots();
    } else {
        _manager.pots() = pots;
    }

    _params.remarks.clear();
    appendManagerRemarks();
}

TpotMain::TpotMain(Params const & params, std::string const & default_app,
                   bool append_manager_remarks, bool update_default_callbacks)
        : TpotMain(params, Pots(), default_app, append_manager_remarks)
{
    // EMPTY.
}

TpotMain::TpotMain(Params const & params, Pots const & pots, std::string const & default_app,
                   bool append_manager_remarks, bool update_default_callbacks)
        : _default_app(default_app), _help_app(), _params(params)
{
    if (pots.empty()) {
        _manager.registerDefaultPots();
    } else {
        _manager.pots() = pots;
    }
    if (append_manager_remarks) {
        appendManagerRemarks();
    }
    if (update_default_callbacks) {
        updateDefaultCallbacks();
    }
}

TpotMain::~TpotMain()
{
    // EMPTY.
}

void TpotMain::appendManagerRemarks()
{
    appendManagerRemarks(_params);
}

void TpotMain::appendManagerRemarks(Params & params)
{
    std::stringstream ss;
    ss << std::endl << _manager.getRemarks()
       << libtbag::app::ex::ServiceApp::getDefaultRemarks();
    params.remarks += ss.str();
}

void TpotMain::updateDefaultCallbacks()
{
    using namespace std::placeholders;
    if (!_params.options_cb) {
        _params.options_cb = std::bind(&TpotMain::onOptions, this, _1);
    }
    if (!_params.properties_cb) {
        _params.properties_cb = std::bind(&TpotMain::onInfo, this, _1);
    }
    if (!_params.std_signal) {
        _params.std_signal = std::bind(&TpotMain::onTerminateSignal, this, _1);
    }
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
    commander.insertDefault("help-app", &_help_app, std::string(),
                            "Print a help message for the application.", "{app}");
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
    if (STORAGE_INFO.empty()) {
        return;
    }
    std::cout << STORAGE_INFO << std::endl;
}

int TpotMain::onRun(RunnerParams const & params)
{
    if (params.verbose) {
        printParamsInfo(params);
    }

    if (!_help_app.empty()) {
        return _manager.help(_help_app, params.verbose);
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

