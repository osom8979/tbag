/**
 * @file   DefaultApp.hpp
 * @brief  DefaultApp class prototype.
 * @author zer0
 * @date   2018-11-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_DEFAULTAPP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_DEFAULTAPP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/dom/xml/XmlModel.hpp>
#include <libtbag/res/Storage.hpp>
#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {
namespace ex  {

/**
 * DefaultApp class prototype.
 *
 * @author zer0
 * @date   2018-11-02
 *
 * @remarks
 *  All frequently used settings can be passed as parameters.
 *
 *  config.xml format:
 *  @code{.xml}
 *   <?xml version='1.0' encoding='utf-8' ?>
 *   <application>
 *     <loggers>
 *       <!-- insert your logger -->
 *       <logger>
 *         <name>test-logger-file</name>
 *         <sink>stdout</sink>
 *       </logger>
 *       <!-- ... -->
 *     </loggers>
 *
 *     <values>
 *       <!-- insert your properties -->
 *       <value name='key1'>value1</value>
 *       <value name='key2'>value2</value>
 *       <value name='key3'>value3</value>
 *       <!-- ... -->
 *     </values>
 *
 *     <properties>
 *       <!-- insert your custom xml -->
 *       <!-- ... -->
 *     </properties>
 *
 *     <storage root='directory_path'>
 *       <config>path</config>
 *       <!-- ... -->
 *     </storage>
 *   </application>
 *  @endcode
 */
class TBAG_API DefaultApp : private Noncopyable
{
public:
    struct Impl; ///< Pointer implementation of ServiceApp.

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_CONFIG_XML = "config.xml";
    TBAG_CONSTEXPR static char const * const APPLICATION_TAG    = "application";
    TBAG_CONSTEXPR static char const * const LOGGERS_TAG        = "loggers";
    TBAG_CONSTEXPR static char const * const VALUES_TAG         = "values";
    TBAG_CONSTEXPR static char const * const VALUE_TAG          = "value";
    TBAG_CONSTEXPR static char const * const PROPERTIES_TAG     = "properties";
    TBAG_CONSTEXPR static char const * const STORAGE_TAG        = "storage";

public:
    using SignalCallback = libtbag::signal::SignalCallback;
    using Element        = libtbag::dom::xml::XmlModel::Element;
    using ConfigScope    = libtbag::dom::xml::XmlModel::Scope;
    using HelpCommander  = libtbag::string::HelpCommander;
    using Environments   = libtbag::string::Environments;
    using Storage        = libtbag::res::Storage;
    using Version        = libtbag::util::Version;

    using Arguments = std::vector<std::string>;
    using ValueMap  = std::unordered_map<std::string, std::string>;

public:
    struct RunnerParams
    {
        Environments envs;
        Arguments    args;
        ValueMap   values;
        Storage   storage;
        bool      verbose;
    };

    using SignalMap       = std::map<int, SignalCallback>;
    using Runner          = std::function<int(RunnerParams const &)>;
    using OptionsCallback = std::function<void(HelpCommander &)>;
    using InfoCallback    = std::function<void(Element const &)>;

public:
    struct Params
    {
        int     argc = 0;       ///< The first argument of the main() function.
        char ** argv = nullptr; ///< The second argument of the main() function.
        char ** envs = nullptr; ///< The third argument of the main() function.

        /**
         * Initialize tbag library.
         */
        bool init_tbag = false;

        /**
         * Install config.xml options.
         */
        bool install_config = true;

        /**
         * Install help options.
         */
        bool install_help = true;

        /**
         * Install verbose options.
         */
        bool install_verbose = true;

        /**
         * Install synopsis options.
         */
        bool install_synopsis = true;
        std::string synopsis;

        /**
         * Install remarks options.
         */
        bool install_remarks = true;
        std::string remarks;

        /**
         * Install create_config options.
         */
        bool install_create_config = false;

        /**
         * User defined config.xml file name.
         * You can add logging information if necessary.
         */
        std::string config_name;

        /**
         * Start program on service(daemon) mode.
         */
        std::string service_name;

        /**
         * Application version.
         */
        Version version;

        /**
         * Default config scope.
         */
        ConfigScope scope = ConfigScope::EXE;

        /**
         * Edit command-line options.
         */
        OptionsCallback options_cb;

        /**
         * Load properties callback (from config.xml).
         */
        InfoCallback properties_cb;

        /**
         * Signal handling.
         */
        SignalMap signals;

        /**
         * STD Terminate Signal handling.
         */
        SignalCallback std_signal;
    };

public:
    DefaultApp();
    ~DefaultApp();

public:
    int run(Runner const & cb);
    int run(Runner const & cb, Params const & params);
};

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_EX_DEFAULTAPP_HPP__

