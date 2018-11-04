/**
 * @file   Resource.cpp
 * @brief  Resource class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/3rd/tinyxml2/tinyxml2.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

// ----------------------------
/* inline */ namespace __impl {
// ----------------------------

// TinyXML2 operators.
using Document = tinyxml2::XMLDocument;
using Printer  = tinyxml2::XMLPrinter;
using Element  = tinyxml2::XMLElement;
using Node     = tinyxml2::XMLNode;

static Resource::Map readFromXmlDocument(Document const & doc,
                                         std::string const & root_name,
                                         std::string const & tag,
                                         std::string const & attribute)
{
    Element const * root = doc.FirstChildElement(root_name.c_str());
    if (root == nullptr) {
        return Resource::Map();
    }

    Element const * cursor = root->FirstChildElement(tag.c_str());
    Resource::Map map;

    while (cursor != nullptr) {
        map.insert(std::make_pair(std::string(cursor->Attribute(attribute.c_str()))
                , std::string(cursor->GetText())));
        cursor = cursor->NextSiblingElement(tag.c_str());
    }

    return map;
}

// ------------------
} // namespace __impl
// ------------------

// ------------------------
// Resource implementation.
// ------------------------

Resource::Resource() : Resource(getRootTagName())
{
    // EMPTY.
}

Resource::Resource(std::string const & root)
        : Resource(root, getPropertyTagName())
{
    // EMPTY.
}

Resource::Resource(std::string const & root, std::string const & tag)
        : Resource(root, tag, getAttributeName())
{
    // EMPTY.
}

Resource::Resource(std::string const & root, std::string const & tag, std::string const & attr)
        : _root(root), _tag(tag), _map(), _attr(attr)
{
    // EMPTY.
}

Resource::Resource(Resource const & obj) : Resource()
{
    copy(obj);
}

Resource::Resource(Resource && obj) TBAG_NOEXCEPT
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

Resource & Resource::operator =(Resource && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

Resource & Resource::copy(Resource const & obj)
{
    if (this != &obj) {
        _root = obj._root;
        _tag  = obj._tag;
        _attr = obj._attr;
        _map  = obj._map;
    }
    return *this;
}

void Resource::swap(Resource & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _root.swap(obj._root);
        _tag.swap(obj._tag);
        _attr.swap(obj._attr);
        _map.swap(obj._map);
    }
}

void Resource::clear()
{
    _map.clear();
}

std::size_t Resource::size() const
{
    return _map.size();
}

std::vector<std::string> Resource::keys() const
{
    std::vector<std::string> result;
    for (auto & cursor : _map) {
        result.push_back(cursor.first);
    }
    return result;
}

bool Resource::readFile(std::string const & path)
{
    _map = readMapFromXmlFile(path, _root, _tag, _attr);
    return (_map.empty() == false);
}

bool Resource::readString(std::string const & xml)
{
    _map = readMapFromXmlString(xml, _root, _tag, _attr);
    return (_map.empty() == false);
}

bool Resource::saveFile(std::string const & path) const
{
    return saveToXmlFile(path, _root, _tag, _attr, _map);
}

std::string Resource::getXmlString() const
{
    return getXmlString(_root, _tag, _attr, _map);
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

std::string Resource::get(std::string const & key, std::string const & default_value) const
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

Resource::Map Resource::readMapFromXmlString(std::string const & xml,
                                             std::string const & root,
                                             std::string const & tag,
                                             std::string const & attr)
{
    __impl::Document doc;
    if (doc.Parse(xml.c_str()) == tinyxml2::XML_NO_ERROR) {
        return __impl::readFromXmlDocument(doc, root, tag, attr);
    }
    return Map();
}

Resource::Map Resource::readMapFromXmlFile(std::string const & path,
                                           std::string const & root,
                                           std::string const & tag,
                                           std::string const & attr)
{
    __impl::Document doc;
    if (doc.LoadFile(path.c_str()) == tinyxml2::XML_NO_ERROR) {
        return __impl::readFromXmlDocument(doc, root, tag, attr);
    }
    return Map();
}

bool Resource::saveToXmlFile(std::string const & path,
                             std::string const & root,
                             std::string const & tag,
                             std::string const & attr,
                             Map const & map)
{
    __impl::Document doc;
    __impl::Node * node = doc.InsertFirstChild(doc.NewElement(root.c_str()));

    for (auto & cursor : map) {
        __impl::Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(attr.c_str(), cursor.first.c_str());
        element->SetText(cursor.second.c_str());
        node->InsertEndChild(element);
    }

    return (doc.SaveFile(path.c_str(), isCompactXmlFile()) == tinyxml2::XML_NO_ERROR);
}

std::string Resource::getXmlString(std::string const & root,
                                   std::string const & tag,
                                   std::string const & attr,
                                   Map const & map)
{
    __impl::Document doc;
    __impl::Node * node = doc.InsertFirstChild(doc.NewElement(root.c_str()));

    for (auto & cursor : map) {
        __impl::Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(attr.c_str(), cursor.first.c_str());
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

Resource Resource::createFromXmlString(std::string const & xml, std::string const & root,
                                       std::string const & tag, std::string const & attr)
{
    std::string root_tag;
    if (root.empty()) {
        root_tag = getRootTagName();
    } else {
        root_tag = root;
    }

    std::string property_tag;
    if (tag.empty()) {
        property_tag = getPropertyTagName();
    } else {
        property_tag = tag;
    }

    std::string name_attribute;
    if (attr.empty()) {
        name_attribute = getAttributeName();
    } else {
        name_attribute = attr;
    }

    Resource res(root, tag, attr);
    res.readString(xml);
    return res;
}

Resource Resource::createFromXmlFile(std::string const & path, std::string const & root,
                                     std::string const & tag, std::string const & attr)
{
    libtbag::filesystem::Path XML_FILE_PATH(path);
    if (!XML_FILE_PATH.isRegularFile() || !XML_FILE_PATH.isReadable()) {
        return Resource();
    }

    std::string xml_string;
    if (isFailure(libtbag::filesystem::readFile(path, xml_string))) {
        return Resource();
    }

    return createFromXmlString(xml_string, root, tag, attr);
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

