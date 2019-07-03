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
#include <libtbag/app/Application.hpp>

#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/dom/xml/DefaultXmlModel.hpp>
#include <libtbag/dom/xml/node/ServerXmlNode.hpp>

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
class TBAG_API ServiceApp : public Application, public Service
{
public:
    using Version = util::Version;

    using HelpCommander   = libtbag::string::HelpCommander;
    using Environments    = libtbag::string::Environments;
    using DefaultXmlModel = libtbag::dom::xml::DefaultXmlModel;
    using ConfigScope     = DefaultXmlModel::Scope;
    using SharedModel     = std::shared_ptr<DefaultXmlModel>;
    using WeakModel       = std::weak_ptr<DefaultXmlModel>;

public:
    TBAG_CONSTEXPR static char const * const GLOBAL_MODEL_OBJECT_KEY
            = "libtbag.app.ex.ServiceApp.DefaultXmlModel";

protected:
    HelpCommander _options;
    Environments  _envs;
    Version       _version;
    SharedModel   _config;
    std::string   _config_path;
    std::string   _service_name;

protected:
    bool _enable_help;
    bool _enable_verbose;
    bool _enable_version;
    bool _enable_create_config;
    bool _enable_service;

public:
    ServiceApp(std::string const & config_name, int argc, char ** argv, char ** envs, bool init_tbag = false);
    virtual ~ServiceApp();

public:
    inline HelpCommander       & atOptions()       TBAG_NOEXCEPT { return _options; }
    inline HelpCommander const & atOptions() const TBAG_NOEXCEPT { return _options; }

    inline Environments       & envs()       TBAG_NOEXCEPT { return _envs; }
    inline Environments const & envs() const TBAG_NOEXCEPT { return _envs; }

    inline SharedModel       & config()       TBAG_NOEXCEPT { return _config; }
    inline SharedModel const & config() const TBAG_NOEXCEPT { return _config; }

    inline std::string getConfigPath() const TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_copy_assignable<std::string>::value)
    { return _config_path; }
    inline Version getVersion() const TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_copy_assignable<Version>::value)
    { return _version; }

    inline bool isEnableHelp   () const TBAG_NOEXCEPT { return _enable_help;    }
    inline bool isEnableVerbose() const TBAG_NOEXCEPT { return _enable_verbose; }
    inline bool isEnableVersion() const TBAG_NOEXCEPT { return _enable_version; }

public:
    static std::string getDefaultSynopsis();
    static std::string getDefaultRemarks();

protected:

    /**
     * @defgroup __DOXYGEN_GROUP__SERVICE_APP_CREATE_HELPER_METHODS__ List of onCreate() helpers.
     * @remarks
     *  This protected groups can be called from the onCreate() function.
     * @{
     */

    void installDefaultSynopsis();
    void installDefaultSynopsis(std::string const & synopsis);

    void installDefaultRemarks();
    void installDefaultRemarks(std::string const & remarks);

    void installHelpOptions();
    void installVerboseOptions();
    void installConfigOptions(ConfigScope scope = ConfigScope::EXE);
    void installConfigOptions(ConfigScope scope, std::string const & global_path);
    void installConfigOptions(ConfigScope scope, std::string const & home_path, std::string const & global_path);
    void installConfigOptions(ConfigScope scope,
                              std::string const & exe_path,
                              std::string const & home_path,
                              std::string const & global_path);

    void installVersionOptions(Version const & version);
    void installVersionOptions(int major, int minor = 0, int patch = 0, std::string const & tweak = std::string());

    void installCreateConfig();

    void installServiceOptions(std::string const & service_name);

    /**
     * @}
     */

private:
    bool updateConfig();

public:
    virtual int run() override;

public:
    /**
     * After reading the config file, Got to the chance to add work to be done.
     */
    virtual bool onLoadConfig(DefaultXmlModel & config);

// Static methods.
public:
    static WeakModel getConfig();
};

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_SERVICEAPP_HPP__

