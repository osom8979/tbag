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
#include <libtbag/dom/xml/XmlUtils.hpp>


#include <string>
#include <vector>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

/**
 * Resource class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class TBAG_API Resource
{
public:
    TBAG_CONSTEXPR static char const * const ROOT_TAG_NAME = "resource";
    TBAG_CONSTEXPR static char const * const PROPERTY_TAG_NAME = "property";
    TBAG_CONSTEXPR static char const * const ATTRIBUTE_NAME = "name";

public:
    using Document = tinyxml2::XMLDocument;
    using Printer = tinyxml2::XMLPrinter;
    using Element = tinyxml2::XMLElement;
    using Node = tinyxml2::XMLNode;

public:
    using Map = std::unordered_map<std::string, std::string>;

public:
    std::string root;
    std::string tag;
    std::string attr;

public:
    Map map;

public:
    Resource(std::string const & r = ROOT_TAG_NAME,
             std::string const & t = PROPERTY_TAG_NAME,
             std::string const & a = ATTRIBUTE_NAME);
    Resource(Resource const & obj);
    Resource(Resource && obj) TBAG_NOEXCEPT;
    virtual ~Resource();

public:
    Resource & operator =(Resource const & obj);
    Resource & operator =(Resource && obj) TBAG_NOEXCEPT;

public:
    void copyFrom(Resource const & obj);
    void copyTo(Resource & obj) const;

public:
    void swap(Resource & obj) TBAG_NOEXCEPT;
    inline friend void swap(Resource & lh, Resource & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(map.empty())
    { return map.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(map.size())
    { return map.size(); }

public:
    void clear()
    { map.clear(); }

public:
    bool exists(std::string const & key) const
    { return map.find(key) != map.end(); }

public:
    std::string & at(std::string const & key)
    { return map.at(key); }
    std::string const & at(std::string const & key) const
    { return map.at(key); }

public:
    std::vector<std::string> keys() const;
    std::vector<std::string> values() const;

public:
    bool readFromXmlString(std::string const & xml);
    bool readFromXmlFile(std::string const & path);
    bool readFromXmlDocument(Document const & document);
    bool readFromXmlElement(Element const & element);

public:
    bool saveToXmlString(std::string & xml) const;
    bool saveToXmlFile(std::string const & path, bool compact = false) const;

public:
    bool get(std::string const & key, std::string & result) const;
    std::string opt(std::string const & key, std::string const & default_value) const;

public:
    template <typename Type, typename Converter>
    bool getValue(std::string const & key, Type * result, Converter func) const
    {
        auto itr = map.find(key);
        if (itr == map.end()) {
            return false;
        }

        Type convert = 0;
        try {
            convert = func(itr->second);
        } catch (...) {
            return false;
        }

        if (result != nullptr) {
            *result = convert;
        }
        return true;
    }

#ifndef __TBAG_RESOURCE_ACCESSOR_IMPLEMENT
#define __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(name, type, func, value)             \
public:                                                                         \
    bool get##name(std::string const & key, type * result) const                \
    {                                                                           \
        return getValue(key, result, [](std::string const & str) -> type {      \
            return func(str);                                                   \
        });                                                                     \
    }                                                                           \
    type opt##name(std::string const & key, type default_value = value) const   \
    {                                                                           \
        type result = 0;                                                        \
        if (get##name(key, &result)) {                                          \
            return result;                                                      \
        }                                                                       \
        return default_value;                                                   \
    }                                                                           \
    type opt(std::string const & key, type default_value = value) const         \
    {                                                                           \
        return this->opt##name(key, default_value);                             \
    }
#endif
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(Integer,    int,                std::stoi,   0);
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(UnInteger,  unsigned int,       std::stoul,  0);
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(LongLong,   long long,          std::stoll,  0);
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(UnLongLong, unsigned long long, std::stoull, 0);
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(Float,      float,       std::stof,  0.0);
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(Double,     double,      std::stod,  0.0);
    __TBAG_RESOURCE_ACCESSOR_IMPLEMENT(LongDouble, long double, std::stold, 0.0);
#undef __TBAG_RESOURCE_ACCESSOR_IMPLEMENT

public:
    void set(std::string const & key, std::string const & value);

#ifndef __TBAG_RESOURCE_MUTATOR_IMPLEMENT
#define __TBAG_RESOURCE_MUTATOR_IMPLEMENT(type)             \
public:                                                     \
    void set(std::string const & key, type const & value)   \
    {                                                       \
        this->set(key, std::to_string(value));              \
    }
#endif
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(int);
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(unsigned int);
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(long long);
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(unsigned long long);
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(float);
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(double);
    __TBAG_RESOURCE_MUTATOR_IMPLEMENT(long double);
#undef __TBAG_RESOURCE_MUTATOR_IMPLEMENT

public:
    static Resource createFromXmlString(std::string const & xml,
                                        std::string const & root = ROOT_TAG_NAME,
                                        std::string const & tag = PROPERTY_TAG_NAME,
                                        std::string const & attr = ATTRIBUTE_NAME);
    static Resource createFromXmlFile(std::string const & path,
                                      std::string const & root = ROOT_TAG_NAME,
                                      std::string const & tag = PROPERTY_TAG_NAME,
                                      std::string const & attr = ATTRIBUTE_NAME);
};

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_RESOURCE_HPP__

