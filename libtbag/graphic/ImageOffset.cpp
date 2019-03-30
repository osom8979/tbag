/**
 * @file   ImageOffset.cpp
 * @brief  ImageOffset class implementation.
 * @author zer0
 * @date   2018-08-04
 * @date   2018-11-06 (Rename: ImageOffsetDom -> ImageOffset)
 */

#include <libtbag/graphic/ImageOffset.hpp>
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

ImageOffset::ImageOffset()
{
    // EMPTY.
}

ImageOffset::ImageOffset(std::string const & path) : ImageOffset()
{
    if (!loadFromFile(path)) {
        throw std::bad_alloc();
    }
}

ImageOffset::ImageOffset(std::string const & xml, load_from_memory const & UNUSED_PARAM(v))
        : ImageOffset()
{
    if (!loadFromMemory(xml)) {
        throw std::bad_alloc();
    }
}

ImageOffset::ImageOffset(ImageOffset const & obj) : ImageOffset()
{
    (*this) = obj;
}

ImageOffset::ImageOffset(ImageOffset && obj) TBAG_NOEXCEPT : ImageOffset()
{
    (*this) = std::move(obj);
}

ImageOffset::~ImageOffset()
{
    // EMPTY.
}

ImageOffset & ImageOffset::operator =(ImageOffset const & obj)
{
    copy(obj);
    return *this;
}

ImageOffset & ImageOffset::operator =(ImageOffset && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ImageOffset::copy(ImageOffset const & obj)
{
    if (this != &obj) {
        _items = obj._items;
    }
}

void ImageOffset::swap(ImageOffset & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _items.swap(_items);
    }
}

bool ImageOffset::find(std::string const & name, Item * result) const
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

std::string ImageOffset::toXmlString() const
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

bool ImageOffset::saveToFile(std::string const & path) const
{
    return isSuccess(libtbag::filesystem::writeFile(path, toXmlString()));
}

bool ImageOffset::loadFromMemory(std::string const & xml)
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

bool ImageOffset::loadFromFile(std::string const & path)
{
    std::string content;
    if (isSuccess(libtbag::filesystem::readFile(path, content))) {
        return loadFromMemory(content);
    }
    return false;
}

ImageOffset ImageOffset::readFile(std::string const & path)
{
    return ImageOffset(path);
}

ImageOffset ImageOffset::readXml(std::string const & xml)
{
    return ImageOffset(xml, ImageOffset::load_from_memory{});
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

