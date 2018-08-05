/**
 * @file   ImageOffsetDom.cpp
 * @brief  ImageOffsetDom class implementation.
 * @author zer0
 * @date   2018-08-04
 */

#include <libtbag/graphic/ImageOffsetDom.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

ImageOffsetDom::ImageOffsetDom()
{
    // EMPTY.
}

ImageOffsetDom::ImageOffsetDom(std::string const & path) : ImageOffsetDom()
{
    if (!loadFromFile(path)) {
        throw std::bad_alloc();
    }
}

ImageOffsetDom::ImageOffsetDom(std::string const & xml, load_from_memory const & UNUSED_PARAM(v))
        : ImageOffsetDom()
{
    if (!loadFromMemory(xml)) {
        throw std::bad_alloc();
    }
}

ImageOffsetDom::ImageOffsetDom(ImageOffsetDom const & obj) : ImageOffsetDom()
{
    (*this) = obj;
}

ImageOffsetDom::ImageOffsetDom(ImageOffsetDom && obj) : ImageOffsetDom()
{
    (*this) = std::move(obj);
}

ImageOffsetDom::~ImageOffsetDom()
{
    // EMPTY.
}

ImageOffsetDom & ImageOffsetDom::operator =(ImageOffsetDom const & obj)
{
    copy(obj);
    return *this;
}

ImageOffsetDom & ImageOffsetDom::operator =(ImageOffsetDom && obj)
{
    swap(obj);
    return *this;
}

void ImageOffsetDom::copy(ImageOffsetDom const & obj)
{
    if (this != &obj) {
        _items = _items;
    }
}

void ImageOffsetDom::swap(ImageOffsetDom & obj)
{
    if (this != &obj) {
        _items.swap(_items);
    }
}

bool ImageOffsetDom::find(std::string const & name, Item * result) const
{
    auto itr = _items.find(name);
    if (itr == _items.end()) {
        return false;
    }
    if (result != nullptr) {
        *result = itr->second;
    }
    return true;
}

std::string ImageOffsetDom::toXmlString() const
{
    Document doc;
    Node * node = doc.InsertFirstChild(doc.NewElement(IMAGES_KEY));

    for (auto & cursor : _items) {
        Element * element = doc.NewElement(IMAGE_KEY);
        element->SetAttribute(NAME_KEY, cursor.first.c_str());

        if (!cursor.second.file.empty()) {
            element->SetAttribute(FILE_KEY, cursor.second.file.c_str());
        }

        // @formatter:off
        element->SetAttribute(       X_KEY, cursor.second.x       );
        element->SetAttribute(       Y_KEY, cursor.second.y       );
        element->SetAttribute(   WIDTH_KEY, cursor.second.width   );
        element->SetAttribute(  HEIGHT_KEY, cursor.second.height  );
        element->SetAttribute(ORIGIN_X_KEY, cursor.second.origin_x);
        element->SetAttribute(ORIGIN_Y_KEY, cursor.second.origin_y);
        element->SetAttribute(  WEIGHT_KEY, cursor.second.weight  );
        // @formatter:on

        if (!cursor.second.body.empty()) {
            element->SetText(cursor.second.body.c_str());
        }

        node->InsertEndChild(element);
    }

    Printer printer;
    if (doc.Accept(&printer)) {
        return std::string(printer.CStr(), printer.CStr() + printer.CStrSize());
    } else {
        return std::string();
    }
}

bool ImageOffsetDom::saveToFile(std::string const & path) const
{
    return isSuccess(libtbag::filesystem::writeFile(path, toXmlString()));
}

bool ImageOffsetDom::loadFromMemory(std::string const & xml)
{
    using namespace libtbag::dom::xml;

    Document doc;
    if (doc.Parse(xml.c_str()) != tinyxml2::XML_NO_ERROR) {
        return false;
    }

    Element const * root = doc.FirstChildElement(IMAGES_KEY);
    if (root == nullptr) {
        return false;
    }

    Element const * cursor = root->FirstChildElement(IMAGE_KEY);
    while (cursor != nullptr) {
        std::string name;
        if (isSuccess(XmlHelper::optAttr(*cursor, NAME_KEY, name))) {
            Item item;
            XmlHelper::optAttr(*cursor,     FILE_KEY, item.file    );
            XmlHelper::optAttr(*cursor,        X_KEY, item.x       );
            XmlHelper::optAttr(*cursor,        Y_KEY, item.y       );
            XmlHelper::optAttr(*cursor,    WIDTH_KEY, item.width   );
            XmlHelper::optAttr(*cursor,   HEIGHT_KEY, item.height  );
            XmlHelper::optAttr(*cursor, ORIGIN_X_KEY, item.origin_x);
            XmlHelper::optAttr(*cursor, ORIGIN_Y_KEY, item.origin_y);
            XmlHelper::optAttr(*cursor,   WEIGHT_KEY, item.weight  );

            char const * body = cursor->GetText();
            if (body != nullptr) {
                item.body = libtbag::string::trim(body);
            }

            _items.insert(std::make_pair(std::move(name), std::move(item)));
        }
        cursor = cursor->NextSiblingElement();
    }

    return true;
}

bool ImageOffsetDom::loadFromFile(std::string const & path)
{
    std::string content;
    if (isSuccess(libtbag::filesystem::readFile(path, content))) {
        return loadFromMemory(content);
    }
    return false;
}

ImageOffsetDom ImageOffsetDom::readFile(std::string const & path)
{
    return ImageOffsetDom(path);
}

ImageOffsetDom ImageOffsetDom::readXml(std::string const & xml)
{
    return ImageOffsetDom(xml, ImageOffsetDom::load_from_memory{});
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

