/**
 * @file   ServiceApp.cpp
 * @brief  ServiceApp class implementation.
 * @author zer0
 * @date   2017-06-02
 */

#include <libtbag/app/ex/ServiceApp.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <iostream>
#include <sstream>

#define SERVICE_APP_OPTIONS_PREFIX         "--"
#define SERVICE_APP_OPTIONS_DELIMITER      "="

#define SERVICE_APP_OPTIONS_GLOBAL         "global"
#define SERVICE_APP_OPTIONS_HOME           "home"
#define SERVICE_APP_OPTIONS_LOCAL          "local"
#define SERVICE_APP_OPTIONS_CONFIG         "config"
#define SERVICE_APP_OPTIONS_VERBOSE        "verbose"
#define SERVICE_APP_OPTIONS_VERSION        "version"
#define SERVICE_APP_OPTIONS_CREATE_CONFIG  "create_config"
#define SERVICE_APP_OPTIONS_SERVICE        "service"

#define SERVICE_APP_ENVIRONMENT_TITLE      "TITLE"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {
namespace ex  {

TBAG_CONSTEXPR static char const * const SERVICE_APP_MAIN_SYNOPSIS = ""
        " Usage: ${" SERVICE_APP_ENVIRONMENT_TITLE "} [flags] cmd1 [cmd2] ...\n"
        ""/* -- END -- */;

TBAG_CONSTEXPR static char const * const SERVICE_APP_MAIN_REMARKS = "\n"
        "Configuration file:\n"
        "  When reading, the values are read from the system,\n"
        "  global and repository local configuration files by default,\n"
        "  and options "
        SERVICE_APP_OPTIONS_PREFIX SERVICE_APP_OPTIONS_GLOBAL ", "
        SERVICE_APP_OPTIONS_PREFIX SERVICE_APP_OPTIONS_HOME   ", "
        SERVICE_APP_OPTIONS_PREFIX SERVICE_APP_OPTIONS_LOCAL  " and "
        SERVICE_APP_OPTIONS_PREFIX SERVICE_APP_OPTIONS_CONFIG SERVICE_APP_OPTIONS_DELIMITER "{filename}\n"
        "  can be used to tell the command to read from only that location"
        ""/* -- END -- */;

ServiceApp::ServiceApp(std::string const & config_name, int argc, char ** argv, char ** envs, bool init_tbag)
        : libtbag::app::Application(argc, argv, envs, init_tbag),
          _options(SERVICE_APP_OPTIONS_PREFIX, SERVICE_APP_OPTIONS_DELIMITER),
          _envs(envs), _version(), _config(), _config_path(),
          _enable_help(false), _enable_verbose(false), _enable_version(false),
          _enable_create_config(false), _enable_service(false)
{
    using namespace libtbag::container;
    _config = newGlobalObject<DefaultXmlModel>(GLOBAL_MODEL_OBJECT_KEY, config_name);
    assert(static_cast<bool>(_config));
}

ServiceApp::~ServiceApp()
{
    // EMPTY.
}

std::string ServiceApp::getDefaultSynopsis()
{
    libtbag::string::Environments env;
    env.push(SERVICE_APP_ENVIRONMENT_TITLE, filesystem::Path::getExePath().getName());
    return env.convert(std::string(SERVICE_APP_MAIN_SYNOPSIS));
}

std::string ServiceApp::getDefaultRemarks()
{
    return std::string(SERVICE_APP_MAIN_REMARKS);
}

void ServiceApp::installDefaultSynopsis()
{
    installDefaultSynopsis(getDefaultSynopsis());
}

void ServiceApp::installDefaultSynopsis(std::string const & synopsis)
{
    _options.setSynopsis(synopsis);
}

void ServiceApp::installDefaultRemarks()
{
    installDefaultRemarks(getDefaultRemarks());
}

void ServiceApp::installDefaultRemarks(std::string const & remarks)
{
    _options.setRemarks(remarks);
}

void ServiceApp::installHelpOptions()
{
    using namespace libtbag::string;
    _options.insertHelpCommand([&](Arguments const & args){
        _enable_help = true;
    });
}

void ServiceApp::installVerboseOptions()
{
    using namespace libtbag::string;
    std::string const HELP = "Be more verbose/talkative during the operation.";
    _options.insertDefault(SERVICE_APP_OPTIONS_VERBOSE, &_enable_verbose, true, HELP);
}

void ServiceApp::installConfigOptions(ConfigScope scope)
{
    installConfigOptions(scope, std::string());
}

void ServiceApp::installConfigOptions(ConfigScope scope, std::string const & global_path)
{
    installConfigOptions(scope, std::string(), global_path);
}

void ServiceApp::installConfigOptions(ConfigScope scope, std::string const & home_path, std::string const & global_path)
{
    installConfigOptions(scope, std::string(), home_path, global_path);
}

void ServiceApp::installConfigOptions(ConfigScope scope,
                                      std::string const & exe_path,
                                      std::string const & home_path,
                                      std::string const & global_path)
{
    using namespace libtbag::string;
    auto config = getConfig().lock();
    assert(static_cast<bool>(config));

    auto const GLOBAL = config->getFilePath(DefaultXmlModel::Scope::GLOBAL).getString();
    auto const HOME   = config->getFilePath(DefaultXmlModel::Scope::HOME).getString();
    auto const LOCAL  = config->getFilePath(DefaultXmlModel::Scope::EXE).getString();

    std::string const GLOBAL_PATH = global_path.empty() ? GLOBAL : global_path;
    std::string const HOME_PATH = home_path.empty() ? HOME : home_path;
    std::string const LOCAL_PATH = exe_path.empty() ? LOCAL : exe_path;

    std::string const DEFAULT_DECORATE = " [DEFAULT]";
    std::string global_help = "Use the global resource. The path is ";
    std::string   home_help = "Use the home resource. The path is ";
    std::string  local_help = "Use the local resource. The path is ";
    std::string config_help = "Use the given config file.";

    global_help += GLOBAL_PATH;
      home_help +=   HOME_PATH;
     local_help +=  LOCAL_PATH;

    // clang-format on
    switch (scope) {
    case ConfigScope::WORK:   config_help += DEFAULT_DECORATE; break;
    case ConfigScope::EXE:     local_help += DEFAULT_DECORATE; _config_path =  LOCAL_PATH; break;
    case ConfigScope::HOME:     home_help += DEFAULT_DECORATE; _config_path =   HOME_PATH; break;
    case ConfigScope::GLOBAL: global_help += DEFAULT_DECORATE; _config_path = GLOBAL_PATH; break;
    }
    // clang-format off

    _options.insertDefault(SERVICE_APP_OPTIONS_GLOBAL, &_config_path,   GLOBAL_PATH, global_help);
    _options.insertDefault(SERVICE_APP_OPTIONS_HOME  , &_config_path,     HOME_PATH,   home_help);
    _options.insertDefault(SERVICE_APP_OPTIONS_LOCAL , &_config_path,    LOCAL_PATH,  local_help);
    _options.insertDefault(SERVICE_APP_OPTIONS_CONFIG, &_config_path, std::string(), config_help, "{filename}");
}

void ServiceApp::installVersionOptions(Version const & version)
{
    installVersionOptions(version.getMajor(), version.getMinor(), version.getPatch(), version.getTweak());
}

void ServiceApp::installVersionOptions(int major, int minor, int patch, std::string const & tweak)
{
    using namespace libtbag::string;
    _version.set(static_cast<uint32_t>(major), static_cast<uint32_t>(minor), static_cast<uint32_t>(patch));
    _version.setTweak(tweak);
    _options.insert(SERVICE_APP_OPTIONS_VERSION, [&](Arguments const & args){
        _enable_version = true;
    }, "print the version number and exit.");
}

void ServiceApp::installCreateConfig()
{
    _options.insertDefault(SERVICE_APP_OPTIONS_CREATE_CONFIG, &_enable_create_config, true,
                           "If the config file does not exist, create it.");
}

void ServiceApp::installServiceOptions(std::string const & service_name)
{
    _service_name = service_name;
    _options.insertDefault(SERVICE_APP_OPTIONS_SERVICE, &_enable_service, true,
                           "Start program on service mode.");
}

bool ServiceApp::updateConfig()
{
    using namespace libtbag::filesystem;
    auto config = getConfig().lock();
    assert(static_cast<bool>(config));
    if (_enable_create_config) {
        return config->loadOrDefaultSave(Path(_config_path), true/*Create parent dir*/);
    } else {
        return config->load(Path(_config_path));
    }
}

int ServiceApp::run()
{
    using namespace libtbag::string;

    ApplicationGuard<ServiceApp> const APP_GUARD(*this);
    if (APP_GUARD.isCreateSuccess() == false) {
        std::cerr << "Create failed.\n";
        return EXIT_FAILURE;
    }

    // ----------------
    // REQUEST OPTIONS.
    // ----------------

    std::string unknown_command;
    std::vector<std::string> cmds;

    _options.setDefaultCallback([&](Arguments const & args){
        if (!_options.isSkipCommands() && !args.getName().empty()) {
            // This block comes when an unknown option is hit.
            unknown_command = args.getName();
            return;
        }

        if (!args.getFull().empty()) {
            cmds.push_back(args.getFull());
        } else if (!args.getOriginalArgumentString().empty()) {
            cmds.push_back(args.getOriginalArgumentString());
        } else if (!args.getName().empty()) {
            cmds.push_back(args.getName());
        }
        _options.setSkipCommands();
    });

    if (_enable_verbose) {
        std::cout << "Request options.\n";
    }
    bool const IGNORE_FIRST = true;
    _options.request(getArgc(), getArgv(), IGNORE_FIRST);

    // --------------
    // CHECK OPTIONS.
    // --------------

    if (!unknown_command.empty()) {
        std::cerr << "Found the unknown commands: " << unknown_command << std::endl;
        return EXIT_FAILURE;
    }

    if (_enable_help) {
        std::cout << _options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (_enable_version) {
        if (_enable_verbose) {
            std::cout << _version.toLongString() << std::endl;
        } else {
            std::cout << _version.toString() << std::endl;
        }
        return EXIT_FAILURE;
    }

    // ------------
    // CONFIG LOAD.
    // ------------

    if (_config_path.empty()) {
        if (_enable_verbose) {
            std::cout << "Empty config path (Disable onLoad event).\n";
        }
    } else {
        bool const UPDATE_CONFIG_RESULT = updateConfig();
        if (_enable_verbose) {
            std::cout << "Update config " << (UPDATE_CONFIG_RESULT ? "success" : "failed")
                      << ": " << _config_path << std::endl;
            std::cout << "Command Line Arguments: " << getCommandLineArgumentsString() << std::endl;
        }

        auto config = getConfig().lock();
        if (onLoadConfig(*config) == false) {
            std::cerr << "onLoad event failed.\n";
            return EXIT_FAILURE;
        }
    }

    if (_enable_service) {
        if (_service_name.empty()) {
            std::cerr << "Undefined service name.\n";
            return EXIT_FAILURE;
        }

        createService(_service_name);
        std::cout << "Enable service mode: " << _service_name << std::endl;

        auto const START_CODE = start();
        if (isFailure(START_CODE)) {
            std::cerr << "Service start " << START_CODE << " error" << std::endl;
            return EXIT_FAILURE;
        }
        registerTerminateHandler();
    }

    return onRunning(cmds);
}

bool ServiceApp::onLoadConfig(DefaultXmlModel & config)
{
    return true;
}

// ---------------
// Static methods.
// ---------------

ServiceApp::WeakModel ServiceApp::getConfig()
{
    using namespace libtbag::container;
    return findGlobalObject<DefaultXmlModel>(GLOBAL_MODEL_OBJECT_KEY);
}

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

