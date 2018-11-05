/**
 * @file   XmlHelper.cpp
 * @brief  XmlHelper class implementation.
 * @author zer0
 * @date   2017-06-02
 */

#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

// ---------------
namespace __impl {
// ---------------

template <typename BaseType, typename Predicated>
static Err queryText(XmlHelper::Element const * node,
                     std::string const & key,
                     BaseType & result,
                     BaseType default_value,
                     Predicated predicated)
{
    BaseType temp = 0;
    if (predicated(&temp) != tinyxml2::XML_NO_ERROR) {
        result = default_value;
        return Err::E_EQUERY;
    }
    result = temp;
    return Err::E_SUCCESS;
}

template <typename BaseType, typename Fp>
static Err queryText2(XmlHelper::Element const & element,
                      std::string const & key,
                      BaseType & result,
                      BaseType default_value,
                      Fp f)
{
    auto * node = element.FirstChildElement(key.c_str());
    if (node == nullptr) {
        result = default_value;
        return Err::E_ENFOUND;
    }
    auto query = std::bind(std::forward<Fp>(f), node, std::placeholders::_1);
    return queryText(node, key, result, default_value, query);
}

template <typename BaseType>
static XmlHelper::Element & newChild(XmlHelper::Element & element,
                                    std::string const & key,
                                    BaseType value)
{
    auto * node = element.GetDocument()->NewElement(key.c_str());
    assert(node != nullptr);
    node->SetText(value);
    element.InsertEndChild(node);
    return *node;
}

template <typename BaseType>
static Err queryAttribute(XmlHelper::Element const & element,
                          std::string const & key,
                          BaseType & result,
                          BaseType default_value)
{
    BaseType temp;
    if (element.QueryAttribute(key.c_str(), &temp) != tinyxml2::XML_NO_ERROR) {
        result = default_value;
        return Err::E_EQUERY;
    }
    result = temp;
    return Err::E_SUCCESS;
}

// ------------------
} // namespace __impl
// ------------------

// -------------------------
// XmlHelper implementation.
// -------------------------

std::string XmlHelper::text(Element const & element)
{
    if (element.GetText() != nullptr) {
        return std::string(element.GetText());
    }
    return std::string();
}

void XmlHelper::text(Element & element, std::string const & value)
{
    element.SetText(value.c_str());
}

void XmlHelper::text(Element & element, bool value)
{
    element.SetText(value);
}

void XmlHelper::text(Element & element, int value)
{
    element.SetText(value);
}

void XmlHelper::text(Element & element, unsigned int value)
{
    element.SetText(value);
}

void XmlHelper::text(Element & element, float value)
{
    element.SetText(value);
}

void XmlHelper::text(Element & element, double value)
{
    element.SetText(value);
}

Err XmlHelper::opt(Element const & element, std::string const & key, std::string & result, std::string const & default_value)
{
    auto * node = element.FirstChildElement(key.c_str());
    if (node != nullptr && node->GetText() != nullptr) {
        result.assign(node->GetText());
        return Err::E_SUCCESS;
    }
    result = default_value;
    return Err::E_PARSING;
}

// @formatter:off
Err XmlHelper::opt(Element const & element, std::string const & key, bool & result, bool default_value)
{ return __impl::queryText2(element, key, result, default_value, &Element::QueryBoolText); }
Err XmlHelper::opt(Element const & element, std::string const & key, int & result, int default_value)
{ return __impl::queryText2(element, key, result, default_value, &Element::QueryIntText); }
Err XmlHelper::opt(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value)
{ return __impl::queryText2(element, key, result, default_value, &Element::QueryUnsignedText); }
Err XmlHelper::opt(Element const & element, std::string const & key, float & result, float default_value)
{ return __impl::queryText2(element, key, result, default_value, &Element::QueryFloatText); }
Err XmlHelper::opt(Element const & element, std::string const & key, double & result, double default_value)
{ return __impl::queryText2(element, key, result, default_value, &Element::QueryDoubleText); }
// @formatter:on

// @formatter:off
XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, std::string const & value)
{ return __impl::newChild(element, key, value.c_str()); }
XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, bool value)
{ return __impl::newChild(element, key, value); }
XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, int value)
{ return __impl::newChild(element, key, value); }
XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, unsigned int value)
{ return __impl::newChild(element, key, value); }
XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, float value)
{ return __impl::newChild(element, key, value); }
XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, double value)
{ return __impl::newChild(element, key, value); }
// @formatter:on

Err XmlHelper::optAttr(Element const & element, std::string const & key, std::string & result, std::string const & default_value)
{
    char const * value = element.Attribute(key.c_str());
    if (value == nullptr) {
        result = default_value;
        return Err::E_EQUERY;
    }
    result.assign(value);
    return Err::E_SUCCESS;
}

// @formatter:off
Err XmlHelper::optAttr(Element const & element, std::string const & key, bool & result, bool default_value)
{ return __impl::queryAttribute(element, key, result, default_value); }
Err XmlHelper::optAttr(Element const & element, std::string const & key, int & result, int default_value)
{ return __impl::queryAttribute(element, key, result, default_value); }
Err XmlHelper::optAttr(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value)
{ return __impl::queryAttribute(element, key, result, default_value); }
Err XmlHelper::optAttr(Element const & element, std::string const & key, float & result, float default_value)
{ return __impl::queryAttribute(element, key, result, default_value); }
Err XmlHelper::optAttr(Element const & element, std::string const & key, double & result, double default_value)
{ return __impl::queryAttribute(element, key, result, default_value); }
// @formatter:on

// @formatter:off
XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, std::string const & value)
{ element.SetAttribute(key.c_str(), value.c_str()); return element; }
XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, bool value)
{ element.SetAttribute(key.c_str(), value); return element; }
XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, int value)
{ element.SetAttribute(key.c_str(), value); return element; }
XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, float value)
{ element.SetAttribute(key.c_str(), value); return element; }
XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, double value)
{ element.SetAttribute(key.c_str(), value); return element; }
// @formatter:on

XmlHelper::Element * XmlHelper::newElement(Element & element, std::string const & tag)
{
    return element.GetDocument()->NewElement(tag.c_str());
}

XmlHelper::Node * XmlHelper::insertElement(Element & element, Node * node)
{
    return element.InsertEndChild(node);
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

