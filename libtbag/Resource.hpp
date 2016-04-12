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

// TinyXML2 operators.
public:
    using Document = tinyxml2::XMLDocument;
    using Element = tinyxml2::XMLElement;
    using Node = tinyxml2::XMLNode;

public:
    static Map read(Document const & doc, Str const & tag) {
        Element const * root = doc.FirstChildElement(ROOT_TAG);
        if (root == nullptr) {
            return Map();
        }

        Element const * cursor = root->FirstChildElement(tag.c_str());
        Map map;

        while (cursor != nullptr) {
            map.insert(std::make_pair(Str(cursor->Attribute(NAME_ATTRIBUTE))
                                    , Str(cursor->GetText())));
            cursor = cursor->NextSiblingElement(tag.c_str());
        }
        return map;
    }

public:
    static Map readFromString(Str const & xml, Str const & tag) {
        Document doc;
        if (doc.Parse(xml.c_str()) == tinyxml2::XML_NO_ERROR) {
            return Resource::read(doc, tag);
        }
        return Map();
    }

    static Map read(Str const & path, Str const & tag) {
        Document doc;
        if (doc.LoadFile(path.c_str()) == tinyxml2::XML_NO_ERROR) {
            return Resource::read(doc, tag);
        }
        return Map();
    }

public:
    static bool save(Str const & path, Str const & tag, Map const & map) {
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

