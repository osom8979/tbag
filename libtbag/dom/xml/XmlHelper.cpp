/**
 * @file   XmlHelper.cpp
 * @brief  XmlHelper class implementation.
 * @author zer0
 * @date   2017-06-02
 */

#include <libtbag/dom/xml/XmlHelper.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

std::string XmlHelper::name(Element const & element)
{
    return libtbag::dom::xml::name(element);
}

std::string XmlHelper::text(Element const & element)
{
    return libtbag::dom::xml::text(element);
}

void XmlHelper::text(Element & element, std::string const & value)
{
    libtbag::dom::xml::text(element, value);
}

void XmlHelper::text(Element & element, char const * value)
{
    libtbag::dom::xml::text(element, value);
}

void XmlHelper::text(Element & element, bool value)
{
    libtbag::dom::xml::text(element, value);
}

void XmlHelper::text(Element & element, int value)
{
    libtbag::dom::xml::text(element, value);
}

void XmlHelper::text(Element & element, unsigned int value)
{
    libtbag::dom::xml::text(element, value);
}

void XmlHelper::text(Element & element, float value)
{
    libtbag::dom::xml::text(element, value);
}

void XmlHelper::text(Element & element, double value)
{
    libtbag::dom::xml::text(element, value);
}

bool XmlHelper::existsChildElement(Element const & element, std::string const & tag)
{
    return libtbag::dom::xml::existsChildElement(element, tag);
}

bool XmlHelper::existsAttribute(Element const & element, std::string const & key)
{
    return libtbag::dom::xml::existsAttribute(element, key);
}

Err XmlHelper::opt(Element const & element, std::string const & key, std::string & result, std::string const & default_value)
{
    return libtbag::dom::xml::opt(element, key, result, default_value);
}

Err XmlHelper::opt(Element const & element, std::string const & key, bool & result, bool default_value)
{
    return libtbag::dom::xml::opt(element, key, result, default_value);
}

Err XmlHelper::opt(Element const & element, std::string const & key, int & result, int default_value)
{
    return libtbag::dom::xml::opt(element, key, result, default_value);
}

Err XmlHelper::opt(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value)
{
    return libtbag::dom::xml::opt(element, key, result, default_value);
}

Err XmlHelper::opt(Element const & element, std::string const & key, float & result, float default_value)
{
    return libtbag::dom::xml::opt(element, key, result, default_value);
}

Err XmlHelper::opt(Element const & element, std::string const & key, double & result, double default_value)
{
    return libtbag::dom::xml::opt(element, key, result, default_value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, std::string const & value)
{
    return libtbag::dom::xml::set(element, key, value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, char const * value)
{
    return libtbag::dom::xml::set(element, key, value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, bool value)
{
    return libtbag::dom::xml::set(element, key, value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, int value)
{
    return libtbag::dom::xml::set(element, key, value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, unsigned int value)
{
    return libtbag::dom::xml::set(element, key, value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, float value)
{
    return libtbag::dom::xml::set(element, key, value);
}

XmlHelper::Element & XmlHelper::set(Element & element, std::string const & key, double value)
{
    return libtbag::dom::xml::set(element, key, value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, std::string & result, std::string const & default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, bool & result, bool default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, int & result, int default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, std::int64_t & result, std::int64_t default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, float & result, float default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

Err XmlHelper::optAttr(Element const & element, std::string const & key, double & result, double default_value)
{
    return libtbag::dom::xml::optAttr(element, key, result, default_value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, std::string const & value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, char const * value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, bool value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, int value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, unsigned int value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, std::int64_t value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, float value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element & XmlHelper::setAttr(Element & element, std::string const & key, double value)
{
    return libtbag::dom::xml::setAttr(element, key, value);
}

XmlHelper::Element * XmlHelper::newElement(Document & doc, std::string const & tag)
{
    return libtbag::dom::xml::newElement(doc, tag);
}

XmlHelper::Element * XmlHelper::newElement(Element & element, std::string const & tag)
{
    return libtbag::dom::xml::newElement(element, tag);
}

XmlHelper::Node * XmlHelper::insertElement(Document & doc, Node * node)
{
    return libtbag::dom::xml::insertElement(doc, node);
}

XmlHelper::Node * XmlHelper::insertElement(Element & element, Node * node)
{
    return libtbag::dom::xml::insertElement(element, node);
}

Err XmlHelper::readFromXmlText(Document & doc, std::string const & xml)
{
    return libtbag::dom::xml::readDocumentFromXmlText(doc, xml);
}

Err XmlHelper::writeDocumentToXmlText(Document const & doc, std::string & xml, bool compact, int depth)
{
    return libtbag::dom::xml::writeDocumentToXmlText(doc, xml, compact, depth);
}

Err XmlHelper::writeElementToXmlText(Element const & elem, std::string & xml, bool compact, int depth)
{
    return libtbag::dom::xml::writeElementToXmlText(elem, xml, compact, depth);
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

