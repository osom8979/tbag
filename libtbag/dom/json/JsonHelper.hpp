/**
 * @file   JsonHelper.hpp
 * @brief  JsonHelper class prototype.
 * @author zer0
 * @date   2019-07-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONHELPER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONHELPER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/json/JsonUtils.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

/**
 * JsonHelper class prototype.
 *
 * @author zer0
 * @date   2019-07-12
 */
struct TBAG_API JsonHelper
{
    using Value = Json::Value;

    static bool parseFromJsonText(std::string const & json, Value & result);
    static bool testFromJsonText(std::string const & json);
    static Value getJsonValueFromJsonText(std::string const & json);

    static std::string writeToJsonText(Value const & value, bool fast = false);

    // clang-format off
    static bool exists        (Value const & v, std::string const & key);
    static bool existsNull    (Value const & v, std::string const & key);
    static bool existsBool    (Value const & v, std::string const & key);
    static bool existsInt     (Value const & v, std::string const & key);
    static bool existsUInt    (Value const & v, std::string const & key);
    static bool existsInt64   (Value const & v, std::string const & key);
    static bool existsUInt64  (Value const & v, std::string const & key);
    static bool existsIntegral(Value const & v, std::string const & key);
    static bool existsDouble  (Value const & v, std::string const & key);
    static bool existsNumeric (Value const & v, std::string const & key);
    static bool existsString  (Value const & v, std::string const & key);
    static bool existsArray   (Value const & v, std::string const & key);
    static bool existsObject  (Value const & v, std::string const & key);
    // clang-format on

    static std::string getForceString(Json::Value const & v);
    static std::string getForceString(Json::Value const & v, std::string const & key);

    static bool getString(Json::Value const & v, std::string * out);
    static bool getString(Json::Value const & v, std::string const & key, std::string * out);

    static bool getInt(Json::Value const & v, int * out);
    static bool getInt(Json::Value const & v, std::string const & key, int * out);
};

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONHELPER_HPP__

