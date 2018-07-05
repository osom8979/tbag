/**
 * @file   Resource.cpp
 * @brief  Resource class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/3rd/tinyxml2/tinyxml2.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

/* inline */ namespace __impl {

// TinyXML2 operators.
using Document = tinyxml2::XMLDocument;
using Printer  = tinyxml2::XMLPrinter;
using Element  = tinyxml2::XMLElement;
using Node     = tinyxml2::XMLNode;

static Resource::Map readFromXmlDocument(Document const & doc, std::string const & tag)
{
    Element const * root = doc.FirstChildElement(Resource::getRootTagName());
    if (root == nullptr) {
        return Resource::Map();
    }

    Element const * cursor = root->FirstChildElement(tag.c_str());
    Resource::Map map;

    while (cursor != nullptr) {
        map.insert(std::make_pair(std::string(cursor->Attribute(Resource::getAttributeName()))
                , std::string(cursor->GetText())));
        cursor = cursor->NextSiblingElement(tag.c_str());
    }

    return map;
}

} // namespace __impl

// ------------------------
// Resource implementation.
// ------------------------

Resource::Resource()
{
    // EMPTY.
}

Resource::Resource(Resource const & obj)
{
    copy(obj);
}

Resource::Resource(Resource && obj)
{
    swap(obj);
}

Resource::~Resource()
{
    // EMPTY.
}

Resource & Resource::operator =(Resource const & obj)
{
    return copy(obj);
}

Resource & Resource::operator =(Resource && obj)
{
    swap(obj);
    return *this;
}

Resource & Resource::copy(Resource const & obj)
{
    if (this != &obj) {
        _tag = obj._tag;
        _map = obj._map;
    }
    return *this;
}

void Resource::swap(Resource & obj)
{
    if (this != &obj) {
        _tag.swap(obj._tag);
        _map.swap(obj._map);
    }
}

void Resource::clear()
{
    _tag.clear();
    _map.clear();
}

std::size_t Resource::size() const
{
    return _map.size();
}

bool Resource::readFile(std::string const & path, std::string const & tag)
{
    _tag = tag;
    _map = readFromXmlFile(path, tag);
    return (_map.empty() == false);
}

bool Resource::readString(std::string const & xml, std::string const & tag)
{
    _tag = tag;
    _map = readFromXmlString(xml, tag);
    return (_map.empty() == false);
}

bool Resource::save(std::string const & path) const
{
    return save(path, _tag, _map);
}

std::string Resource::getXmlString() const
{
    return getXmlString(_tag, _map);
}

bool Resource::getString(std::string const & key, std::string * result) const
{
    auto find_value = _map.find(key);
    if (find_value == _map.end()) {
        return false;
    }

    if (result != nullptr) {
        *result = find_value->second;
    }

    return true;
}

std::string Resource::getString(std::string const & key, std::string default_value) const
{
    std::string result;
    if (getString(key, &result)) {
        return result;
    }
    return default_value;
}

std::string Resource::getString(std::string const & key) const
{
    return getString(key, std::string());
}

auto Resource::get(std::string const & key
                 , std::string default_value) const -> decltype(default_value)
{
    return getString(key, default_value);
}

void Resource::set(std::string const & key, std::string const & value)
{
    auto find_value = _map.find(key);
    if (find_value == _map.end()) {
        _map.insert(std::make_pair(key, value));
    } else {
        find_value->second = value;
    }
}

std::string & Resource::at(std::string const & key)
{
    return _map.at(key);
}

std::string const & Resource::at(std::string const & key) const
{
    return _map.at(key);
}

// ---------------
// static methods.
// ---------------

Resource::Map Resource::readFromXmlString(std::string const & xml, std::string const & tag)
{
    __impl::Document doc;
    if (doc.Parse(xml.c_str()) == tinyxml2::XML_NO_ERROR) {
        return __impl::readFromXmlDocument(doc, tag);
    }

    return Map();
}

Resource::Map Resource::readFromXmlFile(std::string const & path, std::string const & tag)
{
    __impl::Document doc;
    if (doc.LoadFile(path.c_str()) == tinyxml2::XML_NO_ERROR) {
        return __impl::readFromXmlDocument(doc, tag);
    }

    return Map();
}

bool Resource::save(std::string const & path, std::string const & tag, Map const & map)
{
    __impl::Document doc;
    __impl::Node * node = doc.InsertFirstChild(doc.NewElement(getRootTagName()));

    for (auto & cursor : map) {
        __impl::Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(getAttributeName(), cursor.first.c_str());
        element->SetText(cursor.second.c_str());
        node->InsertEndChild(element);
    }

    return (doc.SaveFile(path.c_str(), isCompactXmlFile()) == tinyxml2::XML_NO_ERROR);
}

std::string Resource::getXmlString(std::string const & tag, Map const & map)
{
    __impl::Document doc;
    __impl::Node * node = doc.InsertFirstChild(doc.NewElement(getRootTagName()));

    for (auto & cursor : map) {
        __impl::Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(getAttributeName(), cursor.first.c_str());
        element->SetText(cursor.second.c_str());
        node->InsertEndChild(element);
    }

    __impl::Printer printer;
    if (doc.Accept(&printer)) {
        return std::string(printer.CStr(), printer.CStr() + printer.CStrSize());
    } else {
        return std::string();
    }
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

