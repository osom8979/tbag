/**
 * @file   ServiceApp.hpp
 * @brief  ServiceApp class prototype.
 * @author zer0
 * @date   2017-06-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_SERVICEAPP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_SERVICEAPP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/app/Service.hpp>
#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/dom/DefaultXmlModel.hpp>
#include <libtbag/dom/node/ServerXmlNode.hpp>
#include <libtbag/log/node/DefaultLogXmlNode.hpp>

#include <memory>
#include <vector>
#include <string>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {
namespace ex  {

/**
 * ServiceApp class prototype.
 *
 * @author zer0
 * @date   2017-06-02
 */
class TBAG_API ServiceApp : private Service
{
public:
    using Version = util::Version;

    using HelpCommander     = string::HelpCommander;
    using DefaultXmlModel   = dom::DefaultXmlModel;
    using DefaultLogXmlNode = log::node::DefaultLogXmlNode;
    using ServerXmlNode     = dom::node::ServerXmlNode;

    using WeakModel      = std::weak_ptr<DefaultXmlModel>;
    using WeakLogNode    = std::weak_ptr<DefaultLogXmlNode>;
    using WeakServerNode = std::weak_ptr<ServerXmlNode>;

    using StringVector = std::vector<std::string>;

public:
    enum class RunningMode
    {
        RUN_UNKNOWN,
        RUN_APPLICATION,
        RUN_INSTALL,
        RUN_UNINSTALL,
        RUN_START,
        RUN_STOP,
    };

public:
    TBAG_CONSTEXPR static char const * const GLOBAL_MODEL_OBJECT_KEY
            = "libtbag.app.ex.ServiceApp.DefaultXmlModel";

protected:
    HelpCommander _options;
    HelpCommander _commander;

    std::string _config_path;
    Version     _version;

protected:
    bool _enable_help;
    bool _enable_verbose;
    bool _enable_unknown;
    bool _enable_version;

protected:
    int _exit_code;

public:
    ServiceApp(std::string const & config_name, int argc, char ** argv, char ** envs);
    virtual ~ServiceApp();

public:
    inline HelpCommander       & atOptions()       TBAG_NOEXCEPT { return _options; }
    inline HelpCommander const & atOptions() const TBAG_NOEXCEPT { return _options; }

    inline HelpCommander       & atCommander()       TBAG_NOEXCEPT { return _commander; }
    inline HelpCommander const & atCommander() const TBAG_NOEXCEPT { return _commander; }

protected:
    void installDefaultSynopsis();
    void installDefaultSynopsis(std::string const & synopsis);

    void installDefaultRemarks();
    void installDefaultRemarks(std::string const & remarks);

    void installDefaultCommand();
    void installDefaultOptions();
    void installDefaultVersion(int major, int minor = 0, int patch = 0);

    void installDefaultLogNode();
    void installDefaultLogNode(std::string const & logger_name);
    void installDefaultLogNode(std::string const & logger_name, std::string const & file_name);

    void installDefaultServerNode();
    void installDefaultServerNode(std::string const & var, std::string const & ip, int port);

private:
    bool loadOrDefaultSaveConfig();
    bool loadOrDefaultSaveConfig(std::string const & path);

protected:
    bool createLoggers();
    void registerDefaultSignalHandler();
    void registerDefaultSignalHandler(std::string const & logger);

public:
    int run();

// Static methods.
public:
    static WeakModel      getModel();
    static WeakLogNode    getLogNode();
    static WeakServerNode getServerNode();

public:
    virtual bool onLoadConfig() { return true; }
    virtual int onDefaultCommand(StringVector const & args) { return 0; }
};

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_SERVICEAPP_HPP__

