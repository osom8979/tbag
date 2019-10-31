/**
 * @file   XmlUtils.hpp
 * @brief  XmlUtils class prototype.
 * @author zer0
 * @date   2019-08-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/tinyxml2/tinyxml2.h>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

using Document = tinyxml2::XMLDocument;
using Element = tinyxml2::XMLElement;
using Node = tinyxml2::XMLNode;

TBAG_API std::string name(Element const & elem);
TBAG_API std::string text(Element const & element);
TBAG_API void text(Element & element, std::string const & value);
TBAG_API void text(Element & element, char const * value);
TBAG_API void text(Element & element, bool value);
TBAG_API void text(Element & element, int value);
TBAG_API void text(Element & element, unsigned int value);
TBAG_API void text(Element & element, float value);
TBAG_API void text(Element & element, double value);

TBAG_API bool existsChildElement(Element const & element, std::string const & tag);
TBAG_API bool existsAttribute(Element const & element, std::string const & key);

TBAG_API Err opt(Element const & element, std::string const & key, std::string & result, std::string const & default_value = std::string());
TBAG_API Err opt(Element const & element, std::string const & key, bool & result, bool default_value = false);
TBAG_API Err opt(Element const & element, std::string const & key, int & result, int default_value = 0);
TBAG_API Err opt(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value = 0);
TBAG_API Err opt(Element const & element, std::string const & key, float & result, float default_value = 0.0);
TBAG_API Err opt(Element const & element, std::string const & key, double & result, double default_value = 0.0);

TBAG_API Element & set(Element & element, std::string const & key, std::string const & value);
TBAG_API Element & set(Element & element, std::string const & key, char const * value);
TBAG_API Element & set(Element & element, std::string const & key, bool value);
TBAG_API Element & set(Element & element, std::string const & key, int value);
TBAG_API Element & set(Element & element, std::string const & key, unsigned int value);
TBAG_API Element & set(Element & element, std::string const & key, float value);
TBAG_API Element & set(Element & element, std::string const & key, double value);

TBAG_API Err optAttr(Element const & element, std::string const & key, std::string & result, std::string const & default_value = std::string());
TBAG_API Err optAttr(Element const & element, std::string const & key, bool & result, bool default_value = false);
TBAG_API Err optAttr(Element const & element, std::string const & key, int & result, int default_value = 0);
TBAG_API Err optAttr(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value = 0);
TBAG_API Err optAttr(Element const & element, std::string const & key, std::int64_t & result, std::int64_t default_value = 0);
TBAG_API Err optAttr(Element const & element, std::string const & key, float & result, float default_value = 0.0);
TBAG_API Err optAttr(Element const & element, std::string const & key, double & result, double default_value = 0.0);

TBAG_API Element & setAttr(Element & element, std::string const & key, std::string const & value);
TBAG_API Element & setAttr(Element & element, std::string const & key, char const * value);
TBAG_API Element & setAttr(Element & element, std::string const & key, bool value);
TBAG_API Element & setAttr(Element & element, std::string const & key, int value);
TBAG_API Element & setAttr(Element & element, std::string const & key, unsigned int value);
TBAG_API Element & setAttr(Element & element, std::string const & key, std::int64_t value);
TBAG_API Element & setAttr(Element & element, std::string const & key, float value);
TBAG_API Element & setAttr(Element & element, std::string const & key, double value);

TBAG_API Element * newElement(Document & doc, std::string const & tag);
TBAG_API Element * newElement(Element & element, std::string const & tag);
TBAG_API Node * insertElement(Document & doc, Node * node);
TBAG_API Node * insertElement(Element & element, Node * node);

TBAG_API Err readFromXmlText(Document & doc, std::string const & xml);
TBAG_API Err writeDocumentToXmlText(Document const & doc, std::string & xml, bool compact = false, int depth = 0);
TBAG_API Err writeElementToXmlText(Element const & elem, std::string & xml, bool compact = false, int depth = 0);

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLUTILS_HPP__

