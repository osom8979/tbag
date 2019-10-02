/**
 * @file   JsonHelper.cpp
 * @brief  JsonHelper class implementation.
 * @author zer0
 * @date   2019-07-12
 */

#include <libtbag/dom/json/JsonHelper.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

bool JsonHelper::parseFromJsonText(std::string const & json, Value & result, std::string * error_message)
{
    return libtbag::dom::json::parse(json, result, error_message);
}

bool JsonHelper::testFromJsonText(std::string const & json)
{
    return libtbag::dom::json::testJsonText(json);
}

JsonHelper::Value JsonHelper::getJsonValueFromJsonText(std::string const & json)
{
    return libtbag::dom::json::getJsonValue(json);
}

std::string JsonHelper::writeToJsonText(Value const & value, bool fast)
{
    return libtbag::dom::json::write(value, fast);
}

bool JsonHelper::exists(Value const & v, std::string const & key)
{
    return libtbag::dom::json::exists(v, key);
}

bool JsonHelper::existsNull(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsNull(v, key);
}

bool JsonHelper::existsBool(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsBool(v, key);
}

bool JsonHelper::existsInt(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsInt(v, key);
}

bool JsonHelper::existsUInt(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsUInt(v, key);
}

bool JsonHelper::existsInt64(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsInt64(v, key);
}

bool JsonHelper::existsUInt64(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsUInt64(v, key);
}

bool JsonHelper::existsIntegral(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsIntegral(v, key);
}

bool JsonHelper::existsDouble(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsDouble(v, key);
}

bool JsonHelper::existsNumeric(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsNumeric(v, key);
}

bool JsonHelper::existsString(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsString(v, key);
}

bool JsonHelper::existsArray(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsArray(v, key);
}

bool JsonHelper::existsObject(Value const & v, std::string const & key)
{
    return libtbag::dom::json::existsObject(v, key);
}

std::string JsonHelper::getForceString(Json::Value const & v)
{
    return libtbag::dom::json::getForceString(v);
}

std::string JsonHelper::getForceString(Json::Value const & v, std::string const & key)
{
    return libtbag::dom::json::getForceString(v, key);
}

bool JsonHelper::getString(Json::Value const & v, std::string * out)
{
    return libtbag::dom::json::getString(v, out);
}

bool JsonHelper::getString(Json::Value const & v, std::string const & key, std::string * out)
{
    return libtbag::dom::json::getString(v, key, out);
}

bool JsonHelper::getInt(Json::Value const & v, int * out)
{
    return libtbag::dom::json::getInt(v, out);
}

bool JsonHelper::getInt(Json::Value const & v, std::string const & key, int * out)
{
    return libtbag::dom::json::getInt(v, key, out);
}

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

