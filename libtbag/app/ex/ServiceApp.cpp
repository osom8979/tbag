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
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <iostream>

#define SERVICE_APP_COMMAND_APP       "app"
#define SERVICE_APP_COMMAND_INSTALL   "install"
#define SERVICE_APP_COMMAND_UNINSTALL "uninstall"
#define SERVICE_APP_COMMAND_START     "start"
#define SERVICE_APP_COMMAND_STOP      "stop"
#define SERVICE_APP_COMMAND_REQUEST   "request"

#define SERVICE_APP_OPTIONS_PREFIX    "--"
#define SERVICE_APP_OPTIONS_DELIMITER "="

#define SERVICE_APP_COMMAND_PREFIX    ""
#define SERVICE_APP_COMMAND_DELIMITER "/"

#define SERVICE_APP_OPTIONS_GLOBAL    "global"
#define SERVICE_APP_OPTIONS_HOME      "home"
#define SERVICE_APP_OPTIONS_LOCAL     "local"
#define SERVICE_APP_OPTIONS_CONFIG    "config"
#define SERVICE_APP_OPTIONS_VERBOSE   "verbose"
#define SERVICE_APP_OPTIONS_VERSION   "version"

#define SERVICE_APP_ENVIRONMENT_TITLE "TITLE"
#define SERVICE_APP_COMMAND_HELP_HEAD "Command list:"

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
          _commander(SERVICE_APP_COMMAND_PREFIX, SERVICE_APP_COMMAND_DELIMITER),
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

void ServiceApp::installDefaultSynopsis()
{
    libtbag::string::Environments env;
    env.push(SERVICE_APP_ENVIRONMENT_TITLE, filesystem::Path::getExePath().getName());
    installDefaultSynopsis(env.convert(std::string(SERVICE_APP_MAIN_SYNOPSIS)));
}

void ServiceApp::installDefaultSynopsis(std::string const & synopsis)
{
    _options.setSynopsis(synopsis);
}

void ServiceApp::installDefaultRemarks()
{
    installDefaultRemarks(std::string(SERVICE_APP_MAIN_REMARKS));
}

void ServiceApp::installDefaultRemarks(std::string const & remarks)
{
    _commander.setRemarks(remarks);
}

void ServiceApp::installDefaultCommand()
{
    using namespace libtbag::string;
    _commander.insert(SERVICE_APP_COMMAND_APP, [&](Arguments const & args){
        _exit_code = onRunning();
    }, "Normal application mode. [DEFAULT]");
    _commander.insert(SERVICE_APP_COMMAND_INSTALL, [&](Arguments const & args){
        _exit_code = (install() == Err::E_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
    }, "Install service.");
    _commander.insert(SERVICE_APP_COMMAND_UNINSTALL, [&](Arguments const & args){
        _exit_code = (uninstall() == Err::E_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
    }, "Uninstall service.");
    _commander.insert(SERVICE_APP_COMMAND_START, [&](Arguments const & args){
        _exit_code = (start() == Err::E_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
    }, "Start service.");
    _commander.insert(SERVICE_APP_COMMAND_STOP, [&](Arguments const & args){
        _exit_code = (stop() == Err::E_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
    }, "Stop service.");
}

void ServiceApp::installDefaultOptions()
{
    using namespace libtbag::string;
    auto config = getModel().lock();
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

    _options.insertHelpCommand([&](Arguments const & args){
        _enable_help = true;
    });
    _options.insert(SERVICE_APP_OPTIONS_VERBOSE, [&](Arguments const & args){
        _enable_verbose = true;
    }, "Be more verbose/talkative during the operation.");
}

void ServiceApp::installDefaultVersion(int major, int minor, int patch)
{
    using namespace libtbag::string;
    _version.set(static_cast<uint32_t>(major), static_cast<uint32_t>(minor), static_cast<uint32_t>(patch));
    _options.insert(SERVICE_APP_OPTIONS_VERSION, [&](Arguments const & args){
        _enable_version = true;
    }, "print the version number and exit.");
}

void ServiceApp::installDefaultLogNode()
{
    installDefaultLogNode(std::string(log::TBAG_DEFAULT_LOGGER_NAME));
}

void ServiceApp::installDefaultLogNode(std::string const & logger_name)
{
    using namespace libtbag::container;
    auto config = getModel().lock();
    assert(static_cast<bool>(config));
    config->newAdd<DefaultLogXmlNode>(logger_name);
}

void ServiceApp::installDefaultLogNode(std::string const & logger_name, std::string const & file_name)
{
    using namespace libtbag::container;
    auto config = getModel().lock();
    assert(static_cast<bool>(config));
    config->newAdd<DefaultLogXmlNode>(logger_name, file_name);
}

void ServiceApp::installDefaultServerNode()
{
    using namespace libtbag::container;
    auto config = getModel().lock();
    assert(static_cast<bool>(config));
    config->newAdd<ServerXmlNode>();
}

void ServiceApp::installDefaultServerNode(std::string const & var, std::string const & ip, int port)
{
    using namespace libtbag::container;
    auto config = getModel().lock();
    assert(static_cast<bool>(config));
    config->newAdd<ServerXmlNode>(var, ip, port);
}

bool ServiceApp::loadOrDefaultSaveConfig()
{
    return loadOrDefaultSaveConfig(_config_path);
}

bool ServiceApp::loadOrDefaultSaveConfig(std::string const & path)
{
    auto config = getModel().lock();
    assert(static_cast<bool>(config));

    auto const PATH = filesystem::Path(path);
    if (config->loadOrDefaultSave(PATH) == false) {
        std::cerr << "Load or save failed.\n";
        return false;
    }

    if (_enable_verbose) {
        std::cout << "Load or save config file: " << path << std::endl;
    }
    return true;
}

bool ServiceApp::createLoggers()
{
    auto log = getLogNode().lock();
    if (static_cast<bool>(log) == false) {
        return false;
    }

    if (log->createLoggers() >= 1) {
        auto const NAMES = log->getNames();
        std::size_t const NAMES_SIZE = NAMES.size();
        assert(NAMES_SIZE >= 1);

        std::stringstream ss;
        ss << NAMES[0];
        for (std::size_t i = 1; i < NAMES_SIZE; ++i) {
            ss << "," << NAMES[i];
        }

        if (_enable_verbose) {
            std::cout << "Create loggers: " << ss.str() << std::endl;
        }
    }

    return true;
}

void ServiceApp::registerDefaultSignalHandler()
{
    libtbag::signal::registerDefaultStdTerminateHandler();
    libtbag::signal::registerDefaultHandler();
}

void ServiceApp::registerDefaultSignalHandler(std::string const & logger)
{
    libtbag::signal::registerDefaultStdTerminateHandler(logger);
    libtbag::signal::registerDefaultHandler(logger);
}

int ServiceApp::run()
{
    using namespace libtbag::string;

    if (onCreate() == false) {
        std::cerr << "Create failed.\n";
        return EXIT_FAILURE;
    }

    StringVector cmds;
    _options.setDefaultCallback([&](Arguments const & args){
        if (args.getName().empty()) {
            _enable_unknown = true;
            return;
        }

        if (args.empty() == false) {
            cmds.push_back(libtbag::string::lower(args.get(0)));
        }
    });

    bool const IGNORE_FIRST = true;
    _options.request(getArgc(), getArgv(), IGNORE_FIRST);

    if (loadOrDefaultSaveConfig() && onLoadConfig()) {
        if (_enable_help || _enable_unknown) {
            std::cout << _options.help() << std::endl
                      << _commander.help(true) << std::endl;
            _exit_code = EXIT_FAILURE;
        } else if (_enable_version) {
            std::cout << _version.toString() << std::endl;
            _exit_code = EXIT_SUCCESS;
        } else {
            char const SPACE = ' ';
            std::stringstream ss;
            for (auto & cmd : cmds) {
                ss << cmd << SPACE;
            }

            _commander.setCallOnce();
            std::size_t const CALL_COUNT = _commander.request(ss.str());
            if (CALL_COUNT == 0U) {
                _exit_code = onDefaultCommand(cmds);
            }
        }
    } else {
        _exit_code = EXIT_FAILURE;
    }

    onDestroy();
    return _exit_code;
}

// ---------------
// Static methods.
// ---------------

ServiceApp::WeakModel ServiceApp::getModel()
{
    using namespace libtbag::container;
    return Global::getInstance()->find<DefaultXmlModel>(GLOBAL_MODEL_OBJECT_KEY);
}

ServiceApp::WeakLogNode ServiceApp::getLogNode()
{
    if (auto config = getModel().lock()) {
        return config->getWeak<DefaultLogXmlNode>();
    }
    return WeakLogNode();
}

ServiceApp::WeakServerNode ServiceApp::getServerNode()
{
    if (auto config = getModel().lock()) {
        return config->getWeak<ServerXmlNode>();
    }
    return WeakServerNode();
}

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

