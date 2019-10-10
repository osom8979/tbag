/**
 * @file   Resource.cpp
 * @brief  Resource class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

Resource::Resource(std::string const & r, std::string const & t, std::string const & a)
        : root(r.empty() ? ROOT_TAG_NAME : r),
          tag(t.empty() ? PROPERTY_TAG_NAME : t),
          attr(a.empty() ? ATTRIBUTE_NAME : a),
          map()
{
    // EMPTY.
}

Resource::Resource(Resource const & obj) : Resource()
{
    copyFrom(obj);
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
    copyFrom(obj);
    return *this;
}

Resource & Resource::operator =(Resource && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Resource::copyFrom(Resource const & obj)
{
    if (this != &obj) {
        root = obj.root;
        tag = obj.tag;
        attr = obj.attr;
        map = obj.map;
    }
}

void Resource::copyTo(Resource & obj) const
{
    obj.copyFrom(*this);
}

void Resource::swap(Resource & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        root.swap(obj.root);
        tag.swap(obj.tag);
        attr.swap(obj.attr);
        map.swap(obj.map);
    }
}

std::vector<std::string> Resource::keys() const
{
    std::vector<std::string> result;
    for (auto const & cursor : map) {
        result.emplace_back(cursor.first);
    }
    return result;
}

std::vector<std::string> Resource::values() const
{
    std::vector<std::string> result;
    for (auto const & cursor : map) {
        result.emplace_back(cursor.second);
    }
    return result;
}

bool Resource::readFromXmlString(std::string const & xml)
{
    Document doc;
    if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }
    return readFromXmlDocument(doc);
}

bool Resource::readFromXmlFile(std::string const & path)
{
    Document doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }
    return readFromXmlDocument(doc);
}

bool Resource::readFromXmlDocument(Document const & document)
{
    auto const * elem = document.FirstChildElement(root.c_str());
    if (elem == nullptr) {
        return false;
    }
    return readFromXmlElement(*elem);
}

bool Resource::readFromXmlElement(Element const & element)
{
    auto const * child_elem = element.FirstChildElement(tag.c_str());
    if (child_elem == nullptr) {
        return false;
    }

    while (child_elem != nullptr) {
        auto const * name = child_elem->Attribute(attr.c_str());
        auto const * text = child_elem->GetText();
        if (name == nullptr || text == nullptr) {
            break;
        }
        map.emplace(std::string(name), std::string(text));
        child_elem = child_elem->NextSiblingElement(tag.c_str());
    }
    return true;
}

bool Resource::saveToXmlString(std::string & xml) const
{
    Document doc;
    Node * node = doc.InsertFirstChild(doc.NewElement(root.c_str()));
    if (node == nullptr) {
        return false;
    }

    for (auto const & cursor : map) {
        Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(attr.c_str(), cursor.first.c_str());
        element->SetText(cursor.second.c_str());
        node->InsertEndChild(element);
    }

    Printer printer;
    if (!doc.Accept(&printer)) {
        return false;
    }

    xml.assign(printer.CStr(), printer.CStr() + printer.CStrSize());
    return true;
}

bool Resource::saveToXmlFile(std::string const & path, bool compact) const
{
    Document doc;
    Node * node = doc.InsertFirstChild(doc.NewElement(root.c_str()));
    if (node == nullptr) {
        return false;
    }
    for (auto const & cursor : map) {
        Element * element = doc.NewElement(tag.c_str());
        element->SetAttribute(attr.c_str(), cursor.first.c_str());
        element->SetText(cursor.second.c_str());
        node->InsertEndChild(element);
    }
    return doc.SaveFile(path.c_str(), compact) == tinyxml2::XML_SUCCESS;
}

bool Resource::get(std::string const & key, std::string & value) const
{
    auto const itr = map.find(key);
    if (itr == map.end()) {
        return false;
    }
    value = itr->second;
    return true;
}

std::string Resource::opt(std::string const & key, std::string const & default_value) const
{
    std::string value;
    if (get(key, value)) {
        return value;
    }
    return default_value;
}

void Resource::set(std::string const & key, std::string const & value)
{
    auto itr = map.find(key);
    if (itr == map.end()) {
        map.emplace(key, value);
    } else {
        itr->second = value;
    }
}

Resource Resource::createFromXmlString(std::string const & xml, std::string const & root,
                                       std::string const & tag, std::string const & attr)
{
    Resource res(root, tag, attr);
    res.readFromXmlString(xml);
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

