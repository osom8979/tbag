/**
 * @file   Resource.cpp
 * @brief  Resource class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/dom/Resource.hpp>
#include <tinyxml2.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {

inline namespace implementation {

// TinyXML2 operators.
using Document = tinyxml2::XMLDocument;
using Element = tinyxml2::XMLElement;
using Node = tinyxml2::XMLNode;

static Resource::Map readFromXmlDocument(Document const & doc, std::string const & tag)
{
    Element const * root = doc.FirstChildElement(Resource::ROOT_TAG);
    if (root == nullptr) {
        return Resource::Map();
    }

    Element const * cursor = root->FirstChildElement(tag.c_str());
    Resource::Map map;

    while (cursor != nullptr) {
        map.insert(std::make_pair(std::string(cursor->Attribute(Resource::NAME_ATTRIBUTE))
                , std::string(cursor->GetText())));
        cursor = cursor->NextSiblingElement(tag.c_str());
    }

    return map;
}

} // namespace implementation

// ------------------------
// Resource implementation.
// ------------------------

Resource::Resource()
{
    // EMPTY.
}

Resource::Resource(Resource const & obj)
{
    this->copy(obj);
}

Resource::Resource(Resource && obj)
{
    this->swap(obj);
}

Resource::~Resource()
{
    // EMPTY.
}

Resource & Resource::operator =(Resource const & obj)
{
    return this->copy(obj);
}

Resource & Resource::operator =(Resource && obj)
{
    this->swap(obj);
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

void Resource::clear() noexcept
{
    _tag.assign("");
    _map.clear();
}

std::size_t Resource::size() const noexcept
{
    return _map.size();
}

void Resource::set_tag(std::string const & tag) noexcept
{
    _tag = tag;
}

std::string Resource::get_tag() const noexcept
{
    return _tag;
}

bool Resource::readFile(std::string const & path, std::string const & tag)
{
    _tag = tag;
    _map = Resource::readFromXmlFile(path, tag);
    return (_map.empty() == false);
}

bool Resource::readString(std::string const & xml, std::string const & tag)
{
    _tag = tag;
    _map = Resource::readFromXmlString(xml, tag);
    return (_map.empty() == false);
}

bool Resource::save(std::string const & path)
{
    return Resource::save(path, _tag, _map);
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
    if (this->getString(key, &result)) {
        return result;
    }
    return default_value;
}

std::string Resource::getString(std::string const & key) const
{
    return this->getString(key, std::string(""));
}

auto Resource::get(std::string const & key
                 , std::string default_value) const -> decltype(default_value)
{
    return this->getString(key, std::string(""));
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
    Document doc;
    if (doc.Parse(xml.c_str()) == tinyxml2::XML_NO_ERROR) {
        return readFromXmlDocument(doc, tag);
    }

    return Map();
}

Resource::Map Resource::readFromXmlFile(std::string const & path, std::string const & tag)
{
    Document doc;
    if (doc.LoadFile(path.c_str()) == tinyxml2::XML_NO_ERROR) {
        return readFromXmlDocument(doc, tag);
    }

    return Map();
}

bool Resource::save(std::string const & path, std::string const & tag, Map const & map)
{
    Document doc;
    Node * node = doc.InsertFirstChild(doc.NewElement(ROOT_TAG));

    for (auto cursor : map) {
        Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(NAME_ATTRIBUTE, cursor.first.c_str());
        element->SetText(cursor.second.c_str());
        node->InsertEndChild(element);
    }

    return (doc.SaveFile(path.c_str(), isCompactXmlFile()) == tinyxml2::XML_NO_ERROR);
}


} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

