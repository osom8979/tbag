/**
 * @file   Resource.hpp
 * @brief  Resource class prototype.
 * @author zer0
 * @date   2016-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_RESOURCE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_RESOURCE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/dom/tinyxml2/tinyxml2.h>

#include <string>
#include <vector>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

TBAG_CONSTEXPR bool isCompactXmlFile() TBAG_NOEXCEPT
{
    return false;
}

/**
 * Resource class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class TBAG_API Resource
{
public:
    TBAG_CONSTEXPR static char const * const ROOT_TAG_NAME      = "resource";
    TBAG_CONSTEXPR static char const * const PROPERTY_TAG_NAME  = "property";
    TBAG_CONSTEXPR static char const * const ATTRIBUTE_NAME     = "name";

public:
    TBAG_CONSTEXPR static char const * const getRootTagName    () TBAG_NOEXCEPT { return ROOT_TAG_NAME;     }
    TBAG_CONSTEXPR static char const * const getPropertyTagName() TBAG_NOEXCEPT { return PROPERTY_TAG_NAME; }
    TBAG_CONSTEXPR static char const * const getAttributeName  () TBAG_NOEXCEPT { return ATTRIBUTE_NAME;    }

public:
    using Document = tinyxml2::XMLDocument;
    using Printer  = tinyxml2::XMLPrinter;
    using Element  = tinyxml2::XMLElement;
    using Node     = tinyxml2::XMLNode;

public:
    using Map = std::unordered_map<std::string, std::string>;

private:
    std::string _root;
    std::string _tag;
    std::string _attr;
    Map         _map;

public:
    Resource();
    Resource(std::string const & root);
    Resource(std::string const & root, std::string const & tag);
    Resource(std::string const & root, std::string const & tag, std::string const & attr);
    Resource(Resource const & obj);
    Resource(Resource && obj) TBAG_NOEXCEPT;
    virtual ~Resource();

public:
    Resource & operator =(Resource const & obj);
    Resource & operator =(Resource && obj) TBAG_NOEXCEPT;

public:
    Resource & copy(Resource const & obj);
    void swap(Resource & obj) TBAG_NOEXCEPT;

public:
    void clear();
    std::size_t size() const;

public:
    TBAG_ATTRIBUTE_DEPRECATED inline std::string get_tag() const { return _tag; }
    TBAG_ATTRIBUTE_DEPRECATED inline void set_tag(std::string const & tag) { _tag = tag; }

public:
    inline std::string getTag() const { return _tag; }
    inline void setTag(std::string const & tag) { _tag = tag; }

    inline std::string getRoot() const { return _root; }
    inline void setRoot(std::string const & root) { _root = root; }

public:
    inline Map       & map()       TBAG_NOEXCEPT { return _map; }
    inline Map const & map() const TBAG_NOEXCEPT { return _map; }

public:
    std::vector<std::string> keys() const;

// XML.
public:
    bool readFile(std::string const & path);
    bool readString(std::string const & xml);
    bool readDocument(Document const & document);
    bool readElement(Element const & element);

public:
    bool saveFile(std::string const & path) const;

public:
    std::string getXmlString() const;

// Accessor.
public:
    template <typename Type, typename Converter>
    bool getValue(std::string const & key, Type * result, Converter func) const
    {
        auto find_value = this->_map.find(key);
        if (find_value == this->_map.end()) {
            return false;
        }

        Type convert = 0;
        try {
            convert = func(find_value->second);
        } catch (...) {
            // (std::invalid_argument & e) { e.what(); }
            return false;
        }

        if (result != nullptr) {
            *result = convert;
        }
        return true;
    }

public:
    bool getString(std::string const & key, std::string * result) const;
    std::string getString(std::string const & key, std::string default_value) const;
    std::string getString(std::string const & key) const;

public:
    std::string get(std::string const & key, std::string const & default_value) const;

#ifndef __RESOURCE_ACCESSOR_IMPLEMENT
#define __RESOURCE_ACCESSOR_IMPLEMENT(name, type, func, value)                                      \
public:                                                                                             \
    bool get##name(std::string const & key, type * result) const                                    \
    {                                                                                               \
        return getValue(key, result, [](std::string const & str) -> type {                          \
            return func(str);                                                                       \
        });                                                                                         \
    }                                                                                               \
    type get##name(std::string const & key, type default_value = value) const                       \
    {                                                                                               \
        type result = 0;                                                                            \
        if (get##name(key, &result)) {                                                              \
            return result;                                                                          \
        }                                                                                           \
        return default_value;                                                                       \
    }                                                                                               \
    auto get(std::string const & key, type default_value = value) const -> decltype(default_value)  \
    {                                                                                               \
        return this->get##name(key, default_value);                                                 \
    }
#endif

public:
    __RESOURCE_ACCESSOR_IMPLEMENT(Integer,    int,                std::stoi,   0);
    __RESOURCE_ACCESSOR_IMPLEMENT(UnInteger,  unsigned int,       std::stoul,  0);
    __RESOURCE_ACCESSOR_IMPLEMENT(LongLong,   long long,          std::stoll,  0);
    __RESOURCE_ACCESSOR_IMPLEMENT(UnLongLong, unsigned long long, std::stoull, 0);

    __RESOURCE_ACCESSOR_IMPLEMENT(Float,      float,       std::stof,  0.0);
    __RESOURCE_ACCESSOR_IMPLEMENT(Double,     double,      std::stod,  0.0);
    __RESOURCE_ACCESSOR_IMPLEMENT(LongDouble, long double, std::stold, 0.0);

// Mutator.
public:
    void set(std::string const & key, std::string const & value);

#ifndef __RESOURCE_MUTATOR_IMPLEMENT
#define __RESOURCE_MUTATOR_IMPLEMENT(type)                  \
public:                                                     \
    void set(std::string const & key, type const & value)   \
    {                                                       \
        this->set(key, std::to_string(value));              \
    }
#endif

public:
    __RESOURCE_MUTATOR_IMPLEMENT(int);
    __RESOURCE_MUTATOR_IMPLEMENT(unsigned int);
    __RESOURCE_MUTATOR_IMPLEMENT(long long);
    __RESOURCE_MUTATOR_IMPLEMENT(unsigned long long);

    __RESOURCE_MUTATOR_IMPLEMENT(float);
    __RESOURCE_MUTATOR_IMPLEMENT(double);
    __RESOURCE_MUTATOR_IMPLEMENT(long double);

public:
    bool exists(std::string const & key) const;

public:
    std::string       & at(std::string const & key);
    std::string const & at(std::string const & key) const;

public:
    static Map readMapFromXmlString(std::string const & xml, std::string const & root,
                                    std::string const & tag, std::string const & attr);
    static Map readMapFromXmlFile(std::string const & path, std::string const & root,
                                  std::string const & tag, std::string const & attr);
    static Map readFromXmlDocument(Document const & doc, std::string const & root,
                                   std::string const & tag, std::string const & attr);
    static Map readFromXmlElement(Element const & elem, std::string const & tag,
                                  std::string const & attr);
    static bool saveToXmlFile(std::string const & path, std::string const & root,
                              std::string const & tag, std::string const & attr, Map const & map);
    static std::string getXmlString(std::string const & root, std::string const & tag,
                                    std::string const & attr, Map const & map);

public:
    static Resource createFromXmlString(std::string const & xml,
                                        std::string const & root = getRootTagName(),
                                        std::string const & tag = getPropertyTagName(),
                                        std::string const & attr = getAttributeName());
    static Resource createFromXmlFile(std::string const & path,
                                      std::string const & root = getRootTagName(),
                                      std::string const & tag = getPropertyTagName(),
                                      std::string const & attr = getAttributeName());
};

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_RESOURCE_HPP__

