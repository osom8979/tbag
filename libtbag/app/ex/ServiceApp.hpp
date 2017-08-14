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
#include <libtbag/dom/xml/DefaultXmlModel.hpp>
#include <libtbag/dom/xml/node/ServerXmlNode.hpp>
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
class TBAG_API ServiceApp : public Service
{
public:
    using Version = util::Version;

    using HelpCommander   = string::HelpCommander;
    using DefaultXmlModel = dom::xml::DefaultXmlModel;
    using WeakModel       = std::weak_ptr<DefaultXmlModel>;

    using StringVector = std::vector<std::string>;

public:
    TBAG_CONSTEXPR static char const * const GLOBAL_MODEL_OBJECT_KEY
            = "libtbag.app.ex.ServiceApp.DefaultXmlModel";

protected:
    HelpCommander _options;
    std::string   _config_path;
    Version       _version;

protected:
    bool _enable_help;
    bool _enable_verbose;
    bool _enable_unknown;
    bool _enable_version;

public:
    ServiceApp(std::string const & config_name, int argc, char ** argv, char ** envs);
    virtual ~ServiceApp();

public:
    inline HelpCommander       & atOptions()       TBAG_NOEXCEPT { return _options; }
    inline HelpCommander const & atOptions() const TBAG_NOEXCEPT { return _options; }

    inline std::string getConfigPath() const TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_copy_assignable<std::string>::value)
    { return _config_path; }
    inline Version getVersion() const TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_copy_assignable<Version>::value)
    { return _version; }

    inline bool isEnableHelp   () const TBAG_NOEXCEPT { return _enable_help;    }
    inline bool isEnableVerbose() const TBAG_NOEXCEPT { return _enable_verbose; }
    inline bool isEnableUnknown() const TBAG_NOEXCEPT { return _enable_unknown; }
    inline bool isEnableVersion() const TBAG_NOEXCEPT { return _enable_version; }

protected:

    /**
     * @defgroup __DOXYGEN_GROUP__SERVICE_APP_CREATE_HELPER_METHODS__ List of onCreate() helpers.
     * @remarks
     *  This protected groups can be called from the onCreate() function.
     * @{
     */

    static std::string getDefaultSynopsis();
    static std::string getDefaultRemarks();

    void installDefaultSynopsis();
    void installDefaultSynopsis(std::string const & synopsis);

    void installDefaultRemarks();
    void installDefaultRemarks(std::string const & remarks);

    void installHelpOptions();
    void installVerboseOptions();
    void installConfigOptions();

    void installVersionOptions(Version const & version);
    void installVersionOptions(int major, int minor = 0, int patch = 0);

    /**
     * @}
     */

private:
    bool loadOrDefaultSaveConfig();
    bool loadOrDefaultSaveConfig(std::string const & path, bool create_parent_dir = false);

public:
    virtual int run() override;

private:
    /**
     * Disable onRunning() method.
     * Instead, use the onDefaultCommand() method.
     */
    virtual int onRunning() override
    {
        return EXIT_FAILURE;
    }

public:
    virtual bool onLoadConfig(DefaultXmlModel & config);
    virtual int onDefaultCommand(StringVector const & args);

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

