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
        " Usage: ${" SERVICE_APP_ENVIRONMENT_TITLE "} {options} {command}\n"
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
          _enable_help(false), _enable_verbose(false), _enable_unknown(false), _enable_version(false)
{
    using namespace libtbag::container;
    using namespace libtbag::string;

    auto config = Global::getInstance()->insertNewObject<DefaultXmlModel>(GLOBAL_MODEL_OBJECT_KEY, config_name);
    assert(static_cast<bool>(config));
    _config_path = config->getFilePath(DefaultXmlModel::Scope::EXE).getString();
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
    _options.insert(SERVICE_APP_OPTIONS_VERBOSE, [&](Arguments const & args){
        _enable_verbose = true;
    }, "Be more verbose/talkative during the operation.");
}

void ServiceApp::installConfigOptions()
{
    using namespace libtbag::string;
    auto config = getConfig().lock();
    assert(static_cast<bool>(config));

    std::string const GLOBAL_PATH = config->getFilePath(DefaultXmlModel::Scope::GLOBAL).getString();
    std::string const HOME_PATH   = config->getFilePath(DefaultXmlModel::Scope::HOME  ).getString();
    std::string const LOCAL_PATH  = config->getFilePath(DefaultXmlModel::Scope::EXE   ).getString();

    _options.insert(SERVICE_APP_OPTIONS_GLOBAL, [&, GLOBAL_PATH](Arguments const & args){
        _config_path = GLOBAL_PATH;
    }, std::string("Use the global resource. The path is ") + GLOBAL_PATH);
    _options.insert(SERVICE_APP_OPTIONS_HOME, [&, HOME_PATH](Arguments const & args){
        _config_path = HOME_PATH;
    }, std::string("Use the home resource. The path is ") + HOME_PATH);
    _options.insert(SERVICE_APP_OPTIONS_LOCAL, [&, LOCAL_PATH](Arguments const & args){
        _config_path = LOCAL_PATH;
    }, std::string("Use the local resource. The path is ") + LOCAL_PATH + " [DEFAULT]");
    _options.insert(SERVICE_APP_OPTIONS_CONFIG, [&](Arguments const & args){
        if (args.empty() == false) {
            _config_path = args.get(0);
        } else {
            _enable_unknown = true;
        }
    }, "Use the given config file.", "{filename}");
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

    StringVector cmds;
    _options.setDefaultCallback([&](Arguments const & args){
        if (args.getName().empty() == false) {
            // This block comes when an unknown option is hit.
            _enable_unknown = true;
            return;
        }

        if (args.empty() == false) {
            // Command arguments.
            cmds.push_back(args.get(0));
        }
    });

    bool const IGNORE_FIRST = true;
    _options.request(getArgc(), getArgv(), IGNORE_FIRST);

    // --------------
    // CHECK OPTIONS.
    // --------------

    if (_enable_unknown) {
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

    return onDefaultCommand(cmds);
}

bool ServiceApp::onLoadConfig(DefaultXmlModel & config)
{
    return true;
}

int ServiceApp::onDefaultCommand(StringVector const & args)
{
    return EXIT_FAILURE;
}

// ---------------
// Static methods.
// ---------------

ServiceApp::WeakModel ServiceApp::getConfig()
{
    using namespace libtbag::container;
    return Global::getInstance()->find<DefaultXmlModel>(GLOBAL_MODEL_OBJECT_KEY);
}

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

