/**
 * @file   JsonUtils.hpp
 * @brief  JsonUtils class prototype.
 * @author zer0
 * @date   2018-12-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/json/jsoncpp/json.h>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

TBAG_API bool parse(std::string const & json, Json::Value & result);
TBAG_API bool testJsonText(std::string const & json);
TBAG_API Json::Value getJsonValue(std::string const & json);

TBAG_API std::string writeFast(Json::Value const & value);
TBAG_API std::string writeStyled(Json::Value const & value);
TBAG_API std::string write(Json::Value const & value, bool fast = false);

TBAG_API bool exists(Json::Value const & value, std::string const & key);

// clang-format off
TBAG_API bool existsNull    (Json::Value const & v, std::string const & key);
TBAG_API bool existsBool    (Json::Value const & v, std::string const & key);
TBAG_API bool existsInt     (Json::Value const & v, std::string const & key);
TBAG_API bool existsUInt    (Json::Value const & v, std::string const & key);
TBAG_API bool existsInt64   (Json::Value const & v, std::string const & key);
TBAG_API bool existsUInt64  (Json::Value const & v, std::string const & key);
TBAG_API bool existsIntegral(Json::Value const & v, std::string const & key);
TBAG_API bool existsDouble  (Json::Value const & v, std::string const & key);
TBAG_API bool existsNumeric (Json::Value const & v, std::string const & key);
TBAG_API bool existsString  (Json::Value const & v, std::string const & key);
TBAG_API bool existsArray   (Json::Value const & v, std::string const & key);
TBAG_API bool existsObject  (Json::Value const & v, std::string const & key);
// clang-format on

TBAG_API bool getString(Json::Value const & v, std::string * out);
TBAG_API bool getString(Json::Value const & v, std::string const & key, std::string * out);

TBAG_API bool getInt(Json::Value const & v, int * out);
TBAG_API bool getInt(Json::Value const & v, std::string const & key, int * out);

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONUTILS_HPP__

