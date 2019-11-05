/**
 * @file   Preferences.hpp
 * @brief  Preferences class prototype.
 * @author zer0
 * @date   2019-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_PREFERENCES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_PREFERENCES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/Type.hpp>

#include <string>
#include <map>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

/**
 * Preferences class prototype.
 *
 * All frequently used settings can be passed as parameters.
 *
 * Default node format:
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
 *   <extra>
 *     <!-- insert your custom xml -->
 *     <!-- ... -->
 *   </extra>
 *
 *   <storage root='directory_path'>
 *     <!-- ... -->
 *   </storage>
 * </application>
 * @endcode
 *
 * @author zer0
 * @date   2019-11-03
 */
class TBAG_API Preferences
{
public:
    TBAG_CONSTEXPR static char const * const TAG_APPLICATION = "application";
    TBAG_CONSTEXPR static char const * const TAG_VALUES      = "values";
    TBAG_CONSTEXPR static char const * const TAG_VALUE       = "value";
    TBAG_CONSTEXPR static char const * const TAG_EXTRA       = "extra";

public:
    /**
     * NodeInterface class prototype.
     *
     * @author zer0
     * @date   2017-04-21
     */
    struct NodeInterface : public libtbag::dom::xml::XmlHelper
    {
        NodeInterface() { /* EMPTY. */ }
        virtual ~NodeInterface() { /* EMPTY. */ }

        virtual std::string name() const = 0;

        virtual bool init() { return true; }
        virtual void clear() { /* EMPTY. */ }
        virtual void load(Element const & element) { /* EMPTY. */ }
        virtual void save(Element & element) const { /* EMPTY. */ }

        bool readFromXmlFile(std::string const & path)
        {
            return XmlHelper::readFromXmlFile(path, name(), [this](Element const & element){
                load(element);
            });
        }

        bool readFromXmlText(std::string const & xml)
        {
            return XmlHelper::readFromXmlText(xml, name(), [this](Element const & element){
                load(element);
            });
        }

        bool writeToXmlFile(std::string const & path, bool compact = false) const
        {
            return XmlHelper::writeToXmlFile(path, name(), compact, [this](Element & element){
                save(element);
            });
        }

        bool writeToXmlText(std::string & xml, bool compact = false) const
        {
            return XmlHelper::writeToXmlText(xml, name(), compact, [this](Element & element){
                save(element);
            });
        }
    };

public:
    using SharedNode = std::shared_ptr<NodeInterface>;
    using WeakNode   = std::weak_ptr<NodeInterface>;
    using NodeMap    = std::map<std::string, SharedNode>;
    using NodePair   = NodeMap::value_type;

private:
    NodeMap _nodes;

public:
    Preferences();
    Preferences(std::string const & root);
    Preferences(Preferences const & obj);
    Preferences(Preferences && obj) TBAG_NOEXCEPT;
    ~Preferences();

public:
    Preferences & operator =(Preferences const & obj);
    Preferences & operator =(Preferences && obj) TBAG_NOEXCEPT;

public:
    void copy(Preferences const & obj);
    void swap(Preferences & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Preferences & lh, Preferences & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_nodes.size())
    { return _nodes.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_nodes.empty())
    { return _nodes.empty(); }
    inline void clear() TBAG_NOEXCEPT_SP_OP(_nodes.clear())
    { _nodes.clear(); }

public:
    bool add(NodeInterface * node);
    bool add(SharedNode node);
    bool remove(std::string const & name);

public:
    WeakNode get(std::string const & name);

public:
    /** Create(new) & add node. */
    template <typename Up, typename ... Args>
    inline std::shared_ptr<typename remove_cr<Up>::type> newAdd(Args && ... args)
    {
        typedef typename remove_cr<Up>::type ResultObjectType;
        SharedNode shared(new (std::nothrow) ResultObjectType(std::forward<Args>(args) ...));
        if (shared != nullptr && add(shared)) {
            return std::static_pointer_cast<ResultObjectType>(shared);
        }
        return std::shared_ptr<ResultObjectType>();
    }

    template <typename Up>
    Up * getPointer()
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(NodeInterface, Up);
        if (auto shared = get(Up().name()).lock()) {
            return static_cast<Up*>(shared.get());
        }
        return nullptr;
    }

    template <typename Up>
    std::weak_ptr<Up> getWeak()
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(NodeInterface, Up);
        if (auto shared = get(Up().name()).lock()) {
            return std::weak_ptr<Up>(std::static_pointer_cast<Up>(shared));
        }
        return std::weak_ptr<Up>();
    }

public:
    std::size_t init();

public:
    Err load(std::string const & path);
    Err save(std::string const & path) const;

public:
    Err loadOrDefaultSave(std::string const & config_path, bool create_dir = false);
};

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_PREFERENCES_HPP__

