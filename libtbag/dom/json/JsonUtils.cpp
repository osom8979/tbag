/**
 * @file   JsonUtils.cpp
 * @brief  JsonUtils class implementation.
 * @author zer0
 * @date   2018-12-12
 */

#include <libtbag/dom/json/JsonUtils.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

bool parse(std::string const & json, Json::Value & result)
{
    try {
        return Json::Reader().parse(json, result);
    } catch (...) {
        return false;
    }
}

std::string write(Json::Value const & value)
{
    try {
        return Json::FastWriter().write(value);
    } catch (...) {
        return std::string();
    }
}

bool exists(Json::Value const & value, std::string const & key)
{
    try {
        return value.isMember(key);
    } catch (...) {
        return false;
    }
}

// @formatter:off
bool existsNull    (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isNull    (); }
bool existsBool    (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isBool    (); }
bool existsInt     (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isInt     (); }
bool existsUInt    (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isUInt    (); }
bool existsInt64   (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isInt64   (); }
bool existsUInt64  (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isUInt64  (); }
bool existsIntegral(Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isIntegral(); }
bool existsDouble  (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isDouble  (); }
bool existsNumeric (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isNumeric (); }
bool existsString  (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isString  (); }
bool existsArray   (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isArray   (); }
bool existsObject  (Json::Value const & v, std::string const & key) { return exists(v, key) && v[key].isObject  (); }
// @formatter:on

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

