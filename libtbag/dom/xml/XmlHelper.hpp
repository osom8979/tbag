/**
 * @file   XmlHelper.hpp
 * @brief  XmlHelper class prototype.
 * @author zer0
 * @date   2017-06-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLHELPER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLHELPER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/xml/XmlUtils.hpp>

#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

/**
 * XmlHelper class prototype.
 *
 * @author zer0
 * @date   2017-06-02
 */
struct TBAG_API XmlHelper
{
    using Document = libtbag::dom::xml::Document;
    using Element = libtbag::dom::xml::Element;
    using Node = libtbag::dom::xml::Node;

    static std::string text(Element const & element);
    static void text(Element & element, std::string const & value);
    static void text(Element & element, char const * value);
    static void text(Element & element, bool value);
    static void text(Element & element, int value);
    static void text(Element & element, unsigned int value);
    static void text(Element & element, float value);
    static void text(Element & element, double value);

    static bool existsChildElement(Element const & element, std::string const & tag);
    static bool existsAttribute(Element const & element, std::string const & key);

    static Err opt(Element const & element, std::string const & key, std::string & result, std::string const & default_value = std::string());
    static Err opt(Element const & element, std::string const & key, bool & result, bool default_value = false);
    static Err opt(Element const & element, std::string const & key, int & result, int default_value = 0);
    static Err opt(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value = 0);
    static Err opt(Element const & element, std::string const & key, float & result, float default_value = 0.0);
    static Err opt(Element const & element, std::string const & key, double & result, double default_value = 0.0);

    template <typename T>
    static T get(Element const & element, std::string const & key, T const & default_value = T())
    {
        T result;
        if (isSuccess(opt(element, key, result, default_value))) {
            return result;
        }
        return default_value;
    }

    static Element & set(Element & element, std::string const & key, std::string const & value);
    static Element & set(Element & element, std::string const & key, char const * value);
    static Element & set(Element & element, std::string const & key, bool value);
    static Element & set(Element & element, std::string const & key, int value);
    static Element & set(Element & element, std::string const & key, unsigned int value);
    static Element & set(Element & element, std::string const & key, float value);
    static Element & set(Element & element, std::string const & key, double value);

    static Err optAttr(Element const & element, std::string const & key, std::string & result, std::string const & default_value = std::string());
    static Err optAttr(Element const & element, std::string const & key, bool & result, bool default_value = false);
    static Err optAttr(Element const & element, std::string const & key, int & result, int default_value = 0);
    static Err optAttr(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value = 0);
    static Err optAttr(Element const & element, std::string const & key, std::int64_t & result, std::int64_t default_value = 0);
    static Err optAttr(Element const & element, std::string const & key, float & result, float default_value = 0.0);
    static Err optAttr(Element const & element, std::string const & key, double & result, double default_value = 0.0);

    static Element & setAttr(Element & element, std::string const & key, std::string const & value);
    static Element & setAttr(Element & element, std::string const & key, char const * value);
    static Element & setAttr(Element & element, std::string const & key, bool value);
    static Element & setAttr(Element & element, std::string const & key, int value);
    static Element & setAttr(Element & element, std::string const & key, unsigned int value);
    static Element & setAttr(Element & element, std::string const & key, std::int64_t value);
    static Element & setAttr(Element & element, std::string const & key, float value);
    static Element & setAttr(Element & element, std::string const & key, double value);

    static Element * newElement(Document & doc, std::string const & tag);
    static Element * newElement(Element & element, std::string const & tag);
    static Node * insertElement(Document & doc, Node * node);
    static Node * insertElement(Element & element, Node * node);

    template <typename NodeT, typename Predicated>
    static Node * newElement(NodeT & v, std::string const & tag, Predicated predicated)
    {
        static_assert(std::is_same<NodeT, Document>::value || std::is_same<NodeT, Element>::value,
                      "NodeT must be of type Document or Element.");
        Element * child = newElement(v, tag);
        assert(child != nullptr);
        predicated(*child);
        return insertElement(v, child);
    }

    template <typename Predicated>
    static std::size_t foreachElement(Element const & element, std::string const & tag, Predicated predicated)
    {
        std::size_t count = 0;
        Element const * cursor = element.FirstChildElement(tag.c_str());
        while (cursor != nullptr) {
            predicated(*cursor);
            cursor = cursor->NextSiblingElement();
        }
        return count;
    }

    static Err readFromXmlText(Document & doc, std::string const & xml);
    static Err writeToXmlText(Document const & doc, std::string & xml, bool compact = false, int depth = 0);

    template <typename Predicated>
    static bool readFromXmlFile(std::string const & path, std::string const & tag, Predicated predicated)
    {
        Document doc;
        if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
            return false;
        }
        Element const * root = doc.FirstChildElement(tag.c_str());
        if (root == nullptr) {
            return false;
        }
        predicated(*root);
        return true;
    }

    template <typename Predicated>
    static bool readFromXmlText(std::string const & xml, std::string const & tag, Predicated predicated)
    {
        Document doc;
        if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
            return false;
        }
        Element const * root = doc.FirstChildElement(tag.c_str());
        if (root == nullptr) {
            return false;
        }
        predicated(*root);
        return true;
    }

    template <typename Predicated>
    static bool writeToXmlFile(std::string const & path, std::string const & tag, bool compact, Predicated predicated)
    {
        Document doc;
        auto * element = newElement(doc, tag);
        assert(element != nullptr);
        predicated(*element);
        doc.InsertFirstChild(element);
        return doc.SaveFile(path.c_str(), compact) == tinyxml2::XML_SUCCESS;
    }

    template <typename Predicated>
    static bool writeToXmlText(std::string & xml, std::string const & tag, bool compact, Predicated predicated)
    {
        Document doc;
        auto * element = newElement(doc, tag);
        assert(element != nullptr);
        predicated(*element);
        doc.InsertFirstChild(element);
        return writeToXmlText(doc, xml, compact) == E_SUCCESS;
    }
};

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLHELPER_HPP__

