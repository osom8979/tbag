/**
 * @file   JsonUtils.cpp
 * @brief  JsonUtils class implementation.
 * @author zer0
 * @date   2018-12-12
 */

#include <libtbag/dom/json/JsonUtils.hpp>
#include <libtbag/string/StringUtils.hpp>

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
    return reader->parse(json_text, json_text + json.size(), &result, &error);
}

bool testJsonText(std::string const & json)
{
    Json::Value val;
    return parse(json, val);
}

Json::Value getJsonValue(std::string const & json)
{
    Json::Value result;
    if (parse(json, result)) {
        return result;
    }
    return Json::Value();
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

std::string getForceString(Json::Value const & v)
{
    std::string result;
    if (getString(v, &result)) {
        return result;
    }
    return writeFast(v);
}

std::string getForceString(Json::Value const & v, std::string const & key)
{
    if (exists(v, key)) {
        return getForceString(v[key]);
    }
    return {};
}

bool getString(Json::Value const & v, std::string * out)
{
    switch (v.type()) {
    case Json::nullValue:
        if (out != nullptr) {
            out->clear();
        }
        return true;
    case Json::intValue:
        if (out != nullptr) {
            *out = libtbag::string::toString(v.asInt());
        }
        return true;
    case Json::uintValue:
        if (out != nullptr) {
            *out = libtbag::string::toString(v.asUInt());
        }
        return true;
    case Json::realValue:
        if (out != nullptr) {
            *out = libtbag::string::toString(v.asDouble());
        }
        return true;
    case Json::stringValue:
        if (out != nullptr) {
            *out = v.asString();
        }
        return true;
    case Json::booleanValue:
        if (out != nullptr) {
            *out = libtbag::string::toString(v.asBool());
        }
        return true;
    case Json::arrayValue:
        TBAG_FALLTHROUGH
    case Json::objectValue:
        TBAG_FALLTHROUGH
    default:
        return false;
    }
}

bool getString(Json::Value const & v, std::string const & key, std::string * out)
{
    if (exists(v, key)) {
        return getString(v[key], out);
    }
    return false;
}

bool getInt(Json::Value const & v, int * out)
{
    switch (v.type()) {
    case Json::nullValue:
        if (out != nullptr) {
            *out = 0;
        }
        return true;
    case Json::intValue:
        if (out != nullptr) {
            *out = v.asInt();
        }
        return true;
    case Json::uintValue:
        if (out != nullptr) {
            *out = static_cast<int>(v.asUInt());
        }
        return true;
    case Json::realValue:
        if (out != nullptr) {
            *out = static_cast<int>(v.asDouble());
        }
        return true;
    case Json::stringValue:
        BRACE("String to Integer") {
            int temp;
            if (libtbag::string::toVal(v.asString(), temp)) {
                if (out != nullptr) {
                    *out = temp;
                }
                return true;
            }
        }
        return false;
    case Json::booleanValue:
        if (out != nullptr) {
            *out = v.asBool() ? 1 : 0;
        }
        return true;
    case Json::arrayValue:
        TBAG_FALLTHROUGH
    case Json::objectValue:
        TBAG_FALLTHROUGH
    default:
        return false;
    }
}

bool getInt(Json::Value const & v, std::string const & key, int * out)
{
    if (!exists(v, key)) {
        return false;
    }
    return getInt(v[key], out);
}

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

