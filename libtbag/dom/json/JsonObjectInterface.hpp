/**
 * @file   JsonObjectInterface.hpp
 * @brief  JsonObjectInterface class prototype.
 * @author zer0
 * @date   2017-08-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONOBJECTINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONOBJECTINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

/**
 * JsonObjectInterface class prototype.
 *
 * @author zer0
 * @date   2017-08-14
 */
struct JsonObjectInterface
{
    JsonObjectInterface() { /* EMPTY. */ }
    virtual ~JsonObjectInterface() { /* EMPTY. */ }

    virtual std::string toJsonString() const = 0;
    virtual bool fromJsonString(std::string const & json) = 0;
};

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONOBJECTINTERFACE_HPP__

