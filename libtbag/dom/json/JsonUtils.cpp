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
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    auto * reader = builder.newCharReader();
    auto * json_text = json.c_str();
    std::string error;
    auto const CODE = reader->parse(json_text, json_text + json.size(), &result, &error);
    if (!CODE) {
        tDLogE("parse() Json parse error: {}", error);
    }
    return CODE;
}

std::string writeFast(Json::Value const & value)
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["enableYAMLCompatibility"] = false;
    builder["dropNullPlaceholders"] = true;
    builder["useSpecialFloats"] = false;
    return Json::writeString(builder, value);
}

std::string writeStyled(Json::Value const & value)
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "All";
    builder["indentation"] = "  ";
    builder["enableYAMLCompatibility"] = true;
    builder["dropNullPlaceholders"] = false;
    builder["useSpecialFloats"] = true;
    return Json::writeString(builder, value);
}

std::string write(Json::Value const & value, bool fast)
{
    if (fast) {
        return writeFast(value);
    } else {
        return writeStyled(value);
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

// clang-format off
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
// clang-format on

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

