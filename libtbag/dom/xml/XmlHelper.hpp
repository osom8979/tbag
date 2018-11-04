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
#include <libtbag/Err.hpp>
#include <libtbag/3rd/tinyxml2/tinyxml2.h>
#include <string>

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
    using Document = tinyxml2::XMLDocument;
    using Element  = tinyxml2::XMLElement;

    static std::string text(Element const & element);

    static Err opt(Element const & element, std::string const & key, std::string & result, std::string const & default_value = std::string());
    static Err opt(Element const & element, std::string const & key, bool & result, bool default_value = false);
    static Err opt(Element const & element, std::string const & key, int & result, int default_value = 0);
    static Err opt(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value = 0);
    static Err opt(Element const & element, std::string const & key, float & result, float default_value = 0.0);
    static Err opt(Element const & element, std::string const & key, double & result, double default_value = 0.0);

    static Element & set(Element & element, std::string const & key, std::string const & value);
    static Element & set(Element & element, std::string const & key, bool value);
    static Element & set(Element & element, std::string const & key, int value);
    static Element & set(Element & element, std::string const & key, unsigned int value);
    static Element & set(Element & element, std::string const & key, float value);
    static Element & set(Element & element, std::string const & key, double value);

    static Err optAttr(Element const & element, std::string const & key, std::string & result, std::string const & default_value = std::string());
    static Err optAttr(Element const & element, std::string const & key, bool & result, bool default_value = false);
    static Err optAttr(Element const & element, std::string const & key, int & result, int default_value = 0);
    static Err optAttr(Element const & element, std::string const & key, unsigned int & result, unsigned int default_value = 0);
    static Err optAttr(Element const & element, std::string const & key, float & result, float default_value = 0.0);
    static Err optAttr(Element const & element, std::string const & key, double & result, double default_value = 0.0);

    static Element & setAttr(Element & element, std::string const & key, std::string const & value);
    static Element & setAttr(Element & element, std::string const & key, bool value);
    static Element & setAttr(Element & element, std::string const & key, int value);
    static Element & setAttr(Element & element, std::string const & key, float value);
    static Element & setAttr(Element & element, std::string const & key, double value);
};

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLHELPER_HPP__

