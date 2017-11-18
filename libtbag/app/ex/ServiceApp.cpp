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
#include <libtbag/string/Environments.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <iostream>
#include <sstream>

#define SERVICE_APP_OPTIONS_PREFIX    "--"
#define SERVICE_APP_OPTIONS_DELIMITER "="

#define SERVICE_APP_OPTIONS_GLOBAL    "global"
#define SERVICE_APP_OPTIONS_HOME      "home"
#define SERVICE_APP_OPTIONS_LOCAL     "local"
#define SERVICE_APP_OPTIONS_CONFIG    "config"
#define SERVICE_APP_OPTIONS_VERBOSE   "verbose"
#define SERVICE_APP_OPTIONS_VERSION   "version"

#define SERVICE_APP_ENVIRONMENT_TITLE "TITLE"

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

ServiceApp::ServiceApp(std::string const & config_name, int argc, char ** argv, char ** envs)
        : app::Service(argc, argv, envs),
          _options(SERVICE_APP_OPTIONS_PREFIX, SERVICE_APP_OPTIONS_DELIMITER),
          _enable_help(false), _enable_verbose(false), _enable_version(false)
{
    using namespace libtbag::container;
    auto config = newGlobalObject<DefaultXmlModel>(GLOBAL_MODEL_OBJECT_KEY, config_name);
    assert(static_cast<bool>(config));
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
    using namespace libtbag::string;
    auto config = getConfig().lock();
    assert(static_cast<bool>(config));

    std::string const GLOBAL_PATH = config->getFilePath(DefaultXmlModel::Scope::GLOBAL).getString();
    std::string const   HOME_PATH = config->getFilePath(DefaultXmlModel::Scope::HOME  ).getString();
    std::string const  LOCAL_PATH = config->getFilePath(DefaultXmlModel::Scope::EXE   ).getString();

    std::string const DEFAULT_DECORATE = " [DEFAULT]";
    std::string global_help = "Use the global resource. The path is ";
    std::string   home_help = "Use the home resource. The path is ";
    std::string  local_help = "Use the local resource. The path is ";
    std::string config_help = "Use the given config file.";

    global_help += GLOBAL_PATH;
      home_help +=   HOME_PATH;
     local_help +=  LOCAL_PATH;

    // @formatter:on
    switch (scope) {
    case ConfigScope::WORK:   config_help += DEFAULT_DECORATE; break;
    case ConfigScope::EXE:     local_help += DEFAULT_DECORATE; _config_path =  LOCAL_PATH; break;
    case ConfigScope::HOME:     home_help += DEFAULT_DECORATE; _config_path =   HOME_PATH; break;
    case ConfigScope::GLOBAL: global_help += DEFAULT_DECORATE; _config_path = GLOBAL_PATH; break;
    }
    // @formatter:off

    _options.insertDefault(SERVICE_APP_OPTIONS_GLOBAL, &_config_path,   GLOBAL_PATH, global_help);
    _options.insertDefault(SERVICE_APP_OPTIONS_HOME  , &_config_path,     HOME_PATH,   home_help);
    _options.insertDefault(SERVICE_APP_OPTIONS_LOCAL , &_config_path,    LOCAL_PATH,  local_help);
    _options.insertDefault(SERVICE_APP_OPTIONS_CONFIG, &_config_path, std::string(), config_help, "{filename}");
}

void ServiceApp::installVersionOptions(Version const & version)
{
    installVersionOptions(version.getMajor(), version.getMinor(), version.getPatch());
}

void ServiceApp::installVersionOptions(int major, int minor, int patch)
{
    using namespace libtbag::string;
    _version.set(static_cast<uint32_t>(major), static_cast<uint32_t>(minor), static_cast<uint32_t>(patch));
    _options.insert(SERVICE_APP_OPTIONS_VERSION, [&](Arguments const & args){
        _enable_version = true;
    }, "print the version number and exit.");
}

bool ServiceApp::loadOrDefaultSaveConfig()
{
    bool const CREATE_PARENT_DIR = true;
    return loadOrDefaultSaveConfig(_config_path, CREATE_PARENT_DIR);
}

bool ServiceApp::loadOrDefaultSaveConfig(std::string const & path, bool create_parent_dir)
{
    auto config = getConfig().lock();
    assert(static_cast<bool>(config));

    auto const PATH = filesystem::Path(path);
    return config->loadOrDefaultSave(PATH, create_parent_dir);
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

    bool enable_unknown = false;
    std::vector<std::string> cmds;

    _options.setDefaultCallback([&](Arguments const & args){
        if (args.getName().empty() == false) {
            // This block comes when an unknown option is hit.
            enable_unknown = true;
            return;
        }

        if (args.empty() == false) {
            // Command arguments.
            cmds.push_back(args.getOriginalArgumentString());
        }
    });

    if (_enable_verbose) {
        std::cout << "Request options.\n";
    }
    bool const IGNORE_FIRST = true;
    _options.request(getArgc(), getArgv(), IGNORE_FIRST);

    // --------------
    // CHECK OPTIONS.
    // --------------

    if (enable_unknown) {
        std::cerr << "Found the unknown commands." << std::endl;
        return EXIT_FAILURE;
    }

    if (_enable_help) {
        std::cout << _options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (_enable_version) {
        std::cout << _version.toString() << std::endl;
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
        if (loadOrDefaultSaveConfig() == false) {
            std::cerr << "Load or save failed: " << _config_path << std::endl;
            return EXIT_FAILURE;
        }

        if (_enable_verbose) {
            std::cout << "Load or save config file: " << _config_path << std::endl;
            std::cout << "Command Line Arguments: " << getCommandLineArgumentsString() << std::endl;
        }

        auto config = getConfig().lock();
        if (onLoadConfig(*config) == false) {
            std::cerr << "onLoad event failed.\n";
            return EXIT_FAILURE;
        }
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

