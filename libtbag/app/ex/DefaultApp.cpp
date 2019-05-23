/**
 * @file   DefaultApp.cpp
 * @brief  DefaultApp class implementation.
 * @author zer0
 * @date   2018-11-02
 */

#include <libtbag/app/ex/DefaultApp.hpp>
#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/app/ex/ServiceApp.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>
#include <libtbag/res/node/StorageNode.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {
namespace ex  {

/**
 * DefaultApp::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-02
 */
struct DefaultApp::Impl : public libtbag::app::ex::ServiceApp
{
public:
    using Base          = libtbag::app::ex::ServiceApp;
    using Resource      = libtbag::dom::xml::Resource;
    using NodeInterface = libtbag::dom::xml::XmlModel::NodeInterface;
    using LogXmlNode    = libtbag::log::node::LogXmlNode;
    using StorageNode   = libtbag::res::node::StorageNode;

public:
    /**
     * DefaultApp::Impl::Loggers class implementation.
     *
     * @author zer0
     * @date   2018-11-02
     */
    struct Loggers : public LogXmlNode
    {
        Loggers() : LogXmlNode()
        { /* EMPTY. */ }
        virtual ~Loggers()
        { /* EMPTY. */ }

        virtual std::string name() const override
        { return std::string(LOGGERS_TAG); }
    };

    /**
     * DefaultApp::Impl::Values class implementation.
     *
     * @author zer0
     * @date   2018-11-02
     */
    struct Values : public NodeInterface
    {
        Impl * impl = nullptr;
        Resource resource;

        Values(Impl * parent) : NodeInterface(), impl(parent), resource(VALUES_TAG, VALUE_TAG)
        { /* EMPTY. */ }
        virtual ~Values()
        { /* EMPTY. */ }

        virtual void setup() override
        { resource.clear(); }
        virtual void teardown() override
        { resource.clear(); }

        virtual std::string name() const override
        { return std::string(VALUES_TAG); }

        virtual void load(Element const & element) override
        {
            if (impl->isEnableVerbose()) {
                std::cout << "Load <values> XML" << std::endl;
            }
            resource.readElement(element);
        }
    };

    /**
     * DefaultApp::Impl::Properties class implementation.
     *
     * @author zer0
     * @date   2018-11-02
     */
    struct Properties : public NodeInterface
    {
        Impl * impl = nullptr;

        Properties(Impl * parent) : NodeInterface(), impl(parent)
        { /* EMPTY. */ }
        virtual ~Properties()
        { /* EMPTY. */ }

        virtual std::string name() const override
        { return std::string(PROPERTIES_TAG); }

        virtual void load(Element const & element) override
        {
            if (impl->isEnableVerbose()) {
                std::cout << "Load <properties> XML" << std::endl;
            }
            assert(impl != nullptr);
            if (impl->_params.properties_cb) {
                impl->_params.properties_cb(element);
            }
        }
    };

    /**
     * DefaultApp::Impl::Storage class implementation.
     *
     * @author zer0
     * @date   2018-11-06
     */
    struct Storages : public StorageNode
    {
        Storages(char ** envs) : StorageNode(envs)
        { /* EMPTY. */ }
        virtual ~Storages()
        { /* EMPTY. */ }

        virtual std::string name() const override
        { return std::string(STORAGE_TAG); }
    };

public:
    using SharedLoggers    = std::shared_ptr<Loggers>;
    using SharedValues     = std::shared_ptr<Values>;
    using SharedProperties = std::shared_ptr<Properties>;
    using SharedStorages   = std::shared_ptr<Storages>;

private:
    Runner  _runner;
    Params  _params;

private:
    SharedLoggers    _loggers;
    SharedValues     _values;
    SharedProperties _properties;
    SharedStorages   _storages;

public:
    Impl(Runner const & cb) : Impl(cb, Params{})
    {
        // EMPTY.
    }

    Impl(Runner const & cb, Params const & p)
            : Base(p.config_name, p.argc, p.argv, p.envs, p.init_tbag),
              _runner(cb), _params(p)
    {
        auto & model = config();
        assert(static_cast<bool>(model));

        using namespace libtbag::filesystem;
        auto const EXE_NAME = Path::getExePath().getName();
        auto params = DefaultXmlModel::getDefaultParams(APPLICATION_TAG, EXE_NAME, DEFAULT_CONFIG_XML);
        if (!_params.config_name.empty()) {
            params.file_name = _params.config_name;
        }
        model->setParams(params); // Reset model parameters.
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    virtual bool onCreate() override
    {
        if (_params.install_synopsis) {
            if (_params.synopsis.empty()) {
                installDefaultSynopsis();
            } else {
                installDefaultSynopsis(_params.synopsis);
            }
        }

        if (_params.install_remarks) {
            if (_params.remarks.empty()) {
                installDefaultRemarks();
            } else {
                installDefaultRemarks(_params.remarks);
            }
        }

        if (_params.install_config) {
            installConfigOptions(_params.scope,
                                 _params.local_scope_config_path,
                                 _params.home_scope_config_path,
                                 _params.global_scope_config_path);
        }

        if (_params.install_help) {
            installHelpOptions();
        }

        if (_params.install_verbose) {
            installVerboseOptions();
        }

        if (_params.version.getMajor() != 0 || _params.version.getMinor() != 0) {
            installVersionOptions(_params.version);
        }

        if (_params.install_create_config) {
            installCreateConfig();
        }

        if (_params.install_service) {
            installServiceOptions(_params.service_name);
        }

        if (_params.options_cb) {
            _params.options_cb(atOptions());
        }

        auto config = getConfig().lock();
        assert(static_cast<bool>(config));
        _loggers    = config->newAdd<Loggers>();
        _values     = config->newAdd<Values>(this);
        _properties = config->newAdd<Properties>(this);
        _storages   = config->newAdd<Storages>(_params.envs);
        assert(static_cast<bool>(_loggers));
        assert(static_cast<bool>(_values));
        assert(static_cast<bool>(_properties));
        assert(static_cast<bool>(_storages));

        return true;
    }

    virtual bool onLoadConfig(DefaultXmlModel & config) override
    {
        assert(static_cast<bool>(_loggers));
        std::string preview_message;
        if (_loggers->createLoggers(preview_message) > 0) {
            if (isEnableVerbose()) {
                std::cout << preview_message << std::endl;
            }
        }

        using namespace libtbag::signal;
        if (!_params.signals.empty()) {
            for (auto & cursor : _params.signals) {
                auto const SIGNUM = cursor.first;
                if (isEnableVerbose()) {
                    std::cout << "Register " << getSignalName(SIGNUM) << "(" << SIGNUM << ") signal." << std::endl;
                }
                registerFunctionalHandler(SIGNUM, cursor.second);
            }
        }

        if (_params.std_signal) {
            if (isEnableVerbose()) {
                std::cout << "Register STD Terminate signal." << std::endl;
            }
            registerStdTerminateFunctionalHandler(_params.std_signal);
        }

        return true;
    }

    virtual int onRunning(std::vector<std::string> const & args) override
    {
        assert(static_cast<bool>(_values));
        RunnerParams params;
        params.envs    = envs();
        params.args    = args;
        params.values  = _values->resource.map();
        params.storage = _storages->storage();
        params.verbose = isEnableVerbose();
        if (_runner) {
            return _runner(params);
        }
        return EXIT_FAILURE;
    }

    virtual void onDestroy() override
    {
        // EMPTY.
    }
};

// --------------------------
// DefaultApp implementation.
// --------------------------

DefaultApp::DefaultApp()
{
    // EMPTY.
}

DefaultApp::~DefaultApp()
{
    // EMPTY.
}

int DefaultApp::run(Runner const & cb)
{
    return std::make_unique<Impl>(cb)->run();
}

int DefaultApp::run(Runner const & cb, Params const & params)
{
    return std::make_unique<Impl>(cb, params)->run();
}

} // namespace ex
} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

