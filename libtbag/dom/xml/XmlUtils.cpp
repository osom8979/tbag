/**
 * @file   XmlUtils.cpp
 * @brief  XmlUtils class implementation.
 * @author zer0
 * @date   2019-08-05
 */

#include <libtbag/dom/xml/XmlUtils.hpp>

#include <cassert>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

template <typename BaseType, typename Predicated>
static Err __query_text(Element const * node,
                        std::string const & key,
                        BaseType & result,
                        BaseType default_value,
                        Predicated predicated)
{
    BaseType temp = 0;
    if (predicated(&temp) != tinyxml2::XML_SUCCESS) {
        result = default_value;
        return E_QUERY;
    }
    result = temp;
    return E_SUCCESS;
}

template <typename BaseType, typename Fp>
static Err __query_text2(Element const & element,
                         std::string const & key,
                         BaseType & result,
                         BaseType default_value,
                         Fp f)
{
    auto * node = element.FirstChildElement(key.c_str());
    if (node == nullptr) {
        result = default_value;
        return E_NFOUND;
    }
    auto query = std::bind(std::forward<Fp>(f), node, std::placeholders::_1);
    return __query_text(node, key, result, default_value, query);
}

template <typename BaseType>
static Element & __new_child(Element & element, std::string const & key, BaseType value)
{
    auto * node = element.GetDocument()->NewElement(key.c_str());
    assert(node != nullptr);
    node->SetText(value);
    element.InsertEndChild(node);
    return *node;
}

template <typename BaseType>
static Err __query_attribute(Element const & element,
                             std::string const & key,
                             BaseType & result,
                             BaseType default_value)
{
    BaseType temp;
    if (element.QueryAttribute(key.c_str(), &temp) != tinyxml2::XML_SUCCESS) {
        result = default_value;
        return E_QUERY;
    }
    result = temp;
    return E_SUCCESS;
}

std::string text(Element const & element)
{
    if (element.GetText() != nullptr) {
        return std::string(element.GetText());
    }
    return std::string();
}

void text(Element & element, std::string const & value)
{
    element.SetText(value.c_str());
}

void text(Element & element, char const * value)
{
    element.SetText(value);
}

void text(Element & element, bool value)
{
    element.SetText(value);
}

void text(Element & element, int value)
{
    element.SetText(value);
}

void text(Element & element, unsigned int value)
{
    element.SetText(value);
}

void text(Element & element, float value)
{
    element.SetText(value);
}

void text(Element & element, double value)
{
    element.SetText(value);
}

bool existsChildElement(Element const & element, std::string const & tag)
{
    return element.FirstChildElement(tag.c_str()) != nullptr;
}

bool existsAttribute(Element const & element, std::string const & key)
{
    return element.Attribute(key.c_str()) != nullptr;
}

Err opt(Element const & element, std::string const & key, std::string & result, std::string const & default_value)
{
    auto * node = element.FirstChildElement(key.c_str());
    if (node != nullptr && node->GetText() != nullptr) {
        result.assign(node->GetText());
        return E_SUCCESS;
    }
    result = default_value;
    return E_PARSING;
}

// clang-format off
Err opt(Element const & element, std::string const & key, bool & result, bool default_value)
{ return __query_text2(element, key, result, default_value, &Element::QueryBoolText); }
Err opt(Element const & element, std::string const & key, int & result, int default_value)
{ return __query_text2(element, key, result, default_value, &Element::QueryIntText); }
Err opt(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value)
{ return __query_text2(element, key, result, default_value, &Element::QueryUnsignedText); }
Err opt(Element const & element, std::string const & key, float & result, float default_value)
{ return __query_text2(element, key, result, default_value, &Element::QueryFloatText); }
Err opt(Element const & element, std::string const & key, double & result, double default_value)
{ return __query_text2(element, key, result, default_value, &Element::QueryDoubleText); }
// clang-format on

// clang-format off
Element & set(Element & element, std::string const & key, std::string const & value)
{ return __new_child(element, key, value.c_str()); }
Element & set(Element & element, std::string const & key, char const * value)
{ return __new_child(element, key, value); }
Element & set(Element & element, std::string const & key, bool value)
{ return __new_child(element, key, value); }
Element & set(Element & element, std::string const & key, int value)
{ return __new_child(element, key, value); }
Element & set(Element & element, std::string const & key, unsigned int value)
{ return __new_child(element, key, value); }
Element & set(Element & element, std::string const & key, float value)
{ return __new_child(element, key, value); }
Element & set(Element & element, std::string const & key, double value)
{ return __new_child(element, key, value); }
// clang-format on

Err optAttr(Element const & element, std::string const & key, std::string & result, std::string const & default_value)
{
    char const * value = element.Attribute(key.c_str());
    if (value == nullptr) {
        result = default_value;
        return E_QUERY;
    }
    result.assign(value);
    return E_SUCCESS;
}

// clang-format off
Err optAttr(Element const & element, std::string const & key, bool & result, bool default_value)
{ return __query_attribute(element, key, result, default_value); }
Err optAttr(Element const & element, std::string const & key, int & result, int default_value)
{ return __query_attribute(element, key, result, default_value); }
Err optAttr(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value)
{ return __query_attribute(element, key, result, default_value); }
Err optAttr(Element const & element, std::string const & key, std::int64_t & result, std::int64_t default_value)
{ return __query_attribute(element, key, result, default_value); }
Err optAttr(Element const & element, std::string const & key, float & result, float default_value)
{ return __query_attribute(element, key, result, default_value); }
Err optAttr(Element const & element, std::string const & key, double & result, double default_value)
{ return __query_attribute(element, key, result, default_value); }
// clang-format on

// clang-format off
Element & setAttr(Element & element, std::string const & key, std::string const & value)
{ element.SetAttribute(key.c_str(), value.c_str()); return element; }
Element & setAttr(Element & element, std::string const & key, char const * value)
{ element.SetAttribute(key.c_str(), value); return element; }
Element & setAttr(Element & element, std::string const & key, bool value)
{ element.SetAttribute(key.c_str(), value); return element; }
Element & setAttr(Element & element, std::string const & key, int value)
{ element.SetAttribute(key.c_str(), value); return element; }
Element & setAttr(Element & element, std::string const & key, unsigned int value)
{ element.SetAttribute(key.c_str(), value); return element; }
Element & setAttr(Element & element, std::string const & key, std::int64_t value)
{ element.SetAttribute(key.c_str(), value); return element; }
Element & setAttr(Element & element, std::string const & key, float value)
{ element.SetAttribute(key.c_str(), value); return element; }
Element & setAttr(Element & element, std::string const & key, double value)
{ element.SetAttribute(key.c_str(), value); return element; }
// clang-format on

Element * newElement(Document & doc, std::string const & tag)
{
    return doc.NewElement(tag.c_str());
}

Element * newElement(Element & element, std::string const & tag)
{
    return element.GetDocument()->NewElement(tag.c_str());
}

Node * insertElement(Document & doc, Node * node)
{
    return doc.InsertEndChild(node);
}

Node * insertElement(Element & element, Node * node)
{
    return element.InsertEndChild(node);
}

Err readFromXmlText(Document & doc, std::string const & xml)
{
    if (doc.Parse(xml.c_str()) == tinyxml2::XML_SUCCESS) {
        return E_SUCCESS;
    }
    return E_PARSING;
}

Err writeToXmlText(Document const & doc, std::string & xml, bool compact, int depth)
{
    tinyxml2::XMLPrinter printer(0, compact, depth);
    if (doc.Accept(&printer)) {
        xml.assign(printer.CStr(), printer.CStr() + printer.CStrSize());
        return E_SUCCESS;
    }
    return E_UNKNOWN;
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

