/**
 * @file   Preferences.hpp
 * @brief  Preferences class prototype.
 * @author zer0
 * @date   2019-10-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_PREFERENCES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_PREFERENCES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/dom/xml/DefaultXmlModel.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>
#include <libtbag/res/node/StorageNode.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * Preferences class prototype.
 *
 * All frequently used settings can be passed as parameters.
 *
 * <code>config.xml</code> format:
 * @code{.xml}
 * <?xml version='1.0' encoding='utf-8' ?>
 * <application>
 *   <loggers>
 *     <!-- insert your logger -->
 *     <logger>
 *       <name>test-logger-file</name>
 *       <sink>stdout</sink>
 *     </logger>
 *     <!-- ... -->
 *   </loggers>
 *
 *   <values>
 *     <!-- insert your properties -->
 *     <value name='key1'>value1</value>
 *     <value name='key2'>value2</value>
 *     <value name='key3'>value3</value>
 *     <!-- ... -->
 *   </values>
 *
 *   <properties>
 *     <!-- insert your custom xml -->
 *     <!-- ... -->
 *   </properties>
 *
 *   <storage root='directory_path'>
 *     <!-- ... -->
 *   </storage>
 * </application>
 * @endcode
 *
 * @author zer0
 * @date   2019-10-22
 */
class TBAG_API Preferences
{
public:
    using DefaultXmlModel = libtbag::dom::xml::DefaultXmlModel;
    using InitParams      = DefaultXmlModel::InitParams;
    using NodeInterface   = DefaultXmlModel::NodeInterface;
    using Resource        = libtbag::dom::xml::Resource;
    using LogXmlNode      = libtbag::log::node::LogXmlNode;
    using StorageNode     = libtbag::res::node::StorageNode;

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_CONFIG_XML = "config.xml";

    TBAG_CONSTEXPR static char const * const TAG_APPLICATION = "application";
    TBAG_CONSTEXPR static char const * const TAG_LOGGERS     = "loggers";
    TBAG_CONSTEXPR static char const * const TAG_VALUES      = "values";
    TBAG_CONSTEXPR static char const * const TAG_VALUE       = "value";
    TBAG_CONSTEXPR static char const * const TAG_PROPERTIES  = "properties";
    TBAG_CONSTEXPR static char const * const TAG_STORAGE     = "storage";

public:
    struct Loggers : public LogXmlNode
    {
        Loggers()
        { /* EMPTY. */ }
        virtual ~Loggers()
        { /* EMPTY. */ }

        std::string name() const override
        { return TAG_LOGGERS; }
    };

    struct Values : public NodeInterface
    {
        Resource resource;

        Values() : resource(TAG_VALUES, TAG_VALUE)
        { /* EMPTY. */ }
        virtual ~Values()
        { /* EMPTY. */ }

        void setup() override
        { resource.clear(); }
        void teardown() override
        { resource.clear(); }

        std::string name() const override
        { return TAG_VALUES; }
        void load(Element const & element) override
        { resource.readFromXmlElement(element); }
    };

    struct Properties : public NodeInterface
    {
        Properties()
        { /* EMPTY. */ }
        virtual ~Properties()
        { /* EMPTY. */ }

        std::string name() const override
        { return TAG_PROPERTIES; }
        void load(Element const & element) override
        { /* EMPTY. */ }
    };

    struct Storage : public StorageNode
    {
        Storage()
        { /* EMPTY. */ }
        virtual ~Storage()
        { /* EMPTY. */ }

        std::string name() const override
        { return TAG_STORAGE; }
    };

public:
    using SharedXmlModel   = std::shared_ptr<DefaultXmlModel>;
    using SharedLoggers    = std::shared_ptr<Loggers>;
    using SharedValues     = std::shared_ptr<Values>;
    using SharedProperties = std::shared_ptr<Properties>;
    using SharedStorage    = std::shared_ptr<Storage>;

private:
    SharedXmlModel _model;

public:
    Preferences();
    Preferences(std::nullptr_t) TBAG_NOEXCEPT;
    Preferences(Preferences const & obj) TBAG_NOEXCEPT;
    Preferences(Preferences && obj) TBAG_NOEXCEPT;
    ~Preferences();

public:
    Preferences & operator =(Preferences const & obj) TBAG_NOEXCEPT;
    Preferences & operator =(Preferences && obj) TBAG_NOEXCEPT;

public:
    void copy(Preferences const & obj) TBAG_NOEXCEPT;
    void swap(Preferences & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Preferences & lh, Preferences & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_model); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline DefaultXmlModel       * get()       TBAG_NOEXCEPT { return _model.get(); }
    inline DefaultXmlModel const * get() const TBAG_NOEXCEPT { return _model.get(); }

    inline DefaultXmlModel       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline DefaultXmlModel const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline DefaultXmlModel       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline DefaultXmlModel const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline DefaultXmlModel       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline DefaultXmlModel const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    friend inline bool operator <(Preferences const & x, Preferences const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(Preferences const & x, Preferences const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(Preferences const & x, Preferences const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(Preferences const & x, Preferences const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(Preferences const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(Preferences const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_PREFERENCES_HPP__

