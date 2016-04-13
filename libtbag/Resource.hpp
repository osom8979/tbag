/**
 * @file   Resource.hpp
 * @brief  Resource class prototype.
 * @author zer0
 * @date   2016-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RESOURCE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RESOURCE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

#include <string>
#include <unordered_map>

#include <tinyxml2.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

constexpr bool IsCompactXmlFile() noexcept
{
    return false;
}

/**
 * Resource class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class Resource
{
public:
    static constexpr char const * const ROOT_TAG = "resource";
    static constexpr char const * const NAME_ATTRIBUTE = "name";

public:
    using Str = std::string;
    using Map = std::unordered_map<Str, Str>;

private:
    Str _tag;
    Map _map;

public:
    Resource() {
        __EMPTY_BLOCK__
    }

    Resource(Resource const & obj) {
        this->copy(obj);
    }

    Resource(Resource && obj) {
        this->swap(obj);
    }

    virtual ~Resource() {
        __EMPTY_BLOCK__
    }

public:
    Resource & operator =(Resource const & obj) {
        return this->copy(obj);
    }

    Resource & operator =(Resource && obj) {
        this->swap(obj);
        return *this;
    }

public:
    Resource & copy(Resource const & obj) {
        if (this != &obj) {
            this->_tag = obj._tag;
            this->_map = obj._map;
        }
        return *this;
    }

    void swap(Resource & obj) {
        if (this != &obj) {
            this->_tag.swap(obj._tag);
            this->_map.swap(obj._map);
        }
    }

public:
    void clear() noexcept {
        this->_tag.assign("");
        this->_map.clear();
    }

    std::size_t size() const noexcept {
        return this->_map.size();
    }

    std::string get_tag() const noexcept {
        return this->_tag;
    }

// XML.
public:
    bool readFile(std::string const & path, std::string const & tag) {
        this->_tag = tag;
        this->_map = Resource::readFromXmlFile(path, tag);
        return !this->_map.empty();
    }

    bool readString(std::string const & xml, std::string const & tag) {
        this->_tag = tag;
        this->_map = Resource::readFromXmlString(xml, tag);
        return !this->_map.empty();
    }

    bool save(std::string const & path) {
        return Resource::save(path, this->_tag, this->_map);
    }

// Accessor.
public:
    template <typename Type, typename Converter>
    bool getValue(std::string const & key, Type * result, Converter func) const {
        auto find_value = this->_map.find(key);
        if (find_value == this->_map.end()) {
            return false;
        }

        Type convert = 0;
        try {
            convert = func(find_value->second);
        } catch (std::invalid_argument & e) {
            // e.what();
            return false;
        }

        if (result != nullptr) {
            *result = convert;
        }
        return true;
    }

public:
    bool getString(std::string const & key, std::string * result) const {
        auto find_value = this->_map.find(key);
        if (find_value == this->_map.end()) {
            return false;
        }

        if (result != nullptr) {
            *result = find_value->second;
        }

        return true;
    }

    std::string getString(std::string const & key, std::string default_value) const {
        std::string result;
        if (getString(key, &result)) {
            return result;
        }
        return default_value;
    }

    std::string getString(std::string const & key) const {
        return getString(key, std::string(""));
    }

    auto get(std::string const & key
           , std::string default_value) const
           -> decltype(default_value) {
        return getString(key, std::string(""));
    }

#ifndef __RESOURCE_ACCESSOR_IMPLEMENT
#define __RESOURCE_ACCESSOR_IMPLEMENT(name, type, func, value)      \
public:                                                             \
    bool get##name(std::string const & key, type * result) const {  \
        return getValue(key, result                                 \
                , [](std::string const & str) -> type {             \
                    return func(str);                               \
                });                                                 \
    }                                                               \
    type get##name(std::string const & key                          \
                 , type default_value = value) const {              \
        type result = 0;                                            \
        if (get##name(key, &result)) {                              \
            return result;                                          \
        }                                                           \
        return default_value;                                       \
    }                                                               \
    auto get(std::string const & key                                \
           , type default_value = value) const                      \
           -> decltype(default_value) {                             \
        return this->get##name(key, default_value);                 \
    }
#endif

public:
    __RESOURCE_ACCESSOR_IMPLEMENT(Integer,    int,                   std::stoi,   0);
    __RESOURCE_ACCESSOR_IMPLEMENT(UnInteger,  unsigned int,          std::stoul,  0);
    __RESOURCE_ACCESSOR_IMPLEMENT(LongLong,   long long,             std::stoll,  0);
    __RESOURCE_ACCESSOR_IMPLEMENT(UnLongLong, unsigned long long,    std::stoull, 0);

    __RESOURCE_ACCESSOR_IMPLEMENT(Float,      float,       std::stof,  0.0);
    __RESOURCE_ACCESSOR_IMPLEMENT(Double,     double,      std::stod,  0.0);
    __RESOURCE_ACCESSOR_IMPLEMENT(LongDouble, long double, std::stold, 0.0);

// Mutator.
public:
    void set(std::string const & key, std::string const & value) {
        auto find_value = this->_map.find(key);
        if (find_value == this->_map.end()) {
            this->_map.insert(std::make_pair(key, value));
        } else {
            find_value->second = value;
        }
    }

#ifndef __RESOURCE_MUTATOR_IMPLEMENT
#define __RESOURCE_MUTATOR_IMPLEMENT(name, type)                    \
public:                                                             \
    void set##name(std::string const & key, type const & value) {   \
        this->set(key, std::to_string(value));                      \
    }
#endif

public:
    __RESOURCE_MUTATOR_IMPLEMENT(Integer,    int);
    __RESOURCE_MUTATOR_IMPLEMENT(UnInteger,  unsigned int);
    __RESOURCE_MUTATOR_IMPLEMENT(LongLong,   long long);
    __RESOURCE_MUTATOR_IMPLEMENT(UnLongLong, unsigned long long);

    __RESOURCE_MUTATOR_IMPLEMENT(Float,      float);
    __RESOURCE_MUTATOR_IMPLEMENT(Double,     double);
    __RESOURCE_MUTATOR_IMPLEMENT(LongDouble, long double);

public:
    std::string & at(std::string const & key) {
        return this->_map.at(key);
    }

    std::string const & at(std::string const & key) const {
        return this->_map.at(key);
    }

// TinyXML2 operators.
public:
    using Document = tinyxml2::XMLDocument;
    using Element = tinyxml2::XMLElement;
    using Node = tinyxml2::XMLNode;

public:
    static Map readFromXmlDocument(Document const & doc, std::string const & tag) {
        Element const * root = doc.FirstChildElement(ROOT_TAG);
        if (root == nullptr) {
            return Map();
        }

        Element const * cursor = root->FirstChildElement(tag.c_str());
        Map map;

        while (cursor != nullptr) {
            map.insert(std::make_pair(std::string(cursor->Attribute(NAME_ATTRIBUTE))
                                    , std::string(cursor->GetText())));
            cursor = cursor->NextSiblingElement(tag.c_str());
        }
        return map;
    }

public:
    static Map readFromXmlString(std::string const & xml, std::string const & tag) {
        Document doc;
        if (doc.Parse(xml.c_str()) == tinyxml2::XML_NO_ERROR) {
            return Resource::readFromXmlDocument(doc, tag);
        }
        return Map();
    }

    static Map readFromXmlFile(std::string const & path, std::string const & tag) {
        Document doc;
        if (doc.LoadFile(path.c_str()) == tinyxml2::XML_NO_ERROR) {
            return Resource::readFromXmlDocument(doc, tag);
        }
        return Map();
    }

public:
    static bool save(std::string const & path, std::string const & tag, Map const & map) {
        Document doc;
        Node * node = doc.InsertFirstChild(doc.NewElement(ROOT_TAG));
        for (auto cursor : map) {
            Element * element = doc.NewElement(tag.c_str());
            element->SetAttribute(NAME_ATTRIBUTE, cursor.first.c_str());
            element->SetText(cursor.second.c_str());
            node->InsertEndChild(element);
        }
        if (doc.SaveFile(path.c_str()) == tinyxml2::XML_NO_ERROR) {
            return true;
        }
        return false;
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RESOURCE_HPP__

