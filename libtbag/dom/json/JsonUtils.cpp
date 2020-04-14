/**
 * @file   JsonUtils.cpp
 * @brief  JsonUtils class implementation.
 * @author zer0
 * @date   2018-12-12
 */

#include <libtbag/dom/json/JsonUtils.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

bool parse(std::string const & json, Json::Value & result, std::string * error_message)
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    auto * reader = builder.newCharReader();
    auto * json_text = json.c_str();
    return reader->parse(json_text, json_text + json.size(), &result, error_message);
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
    builder["dropNullPlaceholders"] = false;
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

bool optBool(Json::Value const & v, std::string const & key, bool def)
{
    if (existsBool(v, key)) {
        return v[key].asBool();
    }
    return def;
}

int optInt(Json::Value const & v, std::string const & key, int def)
{
    if (existsInt(v, key)) {
        return v[key].asInt();
    }
    return def;
}

unsigned optUInt(Json::Value const & v, std::string const & key, unsigned def)
{
    if (existsUInt(v, key)) {
        return v[key].asUInt();
    }
    return def;
}

std::int64_t optInt64(Json::Value const & v, std::string const & key, std::int64_t def)
{
    if (existsInt64(v, key)) {
        return v[key].asInt64();
    }
    return def;
}

std::uint64_t optUInt64(Json::Value const & v, std::string const & key, std::uint64_t def)
{
    if (existsUInt64(v, key)) {
        return v[key].asUInt64();
    }
    return def;
}

float optFloat(Json::Value const & v, std::string const & key, float def)
{
    if (existsNumeric(v, key)) {
        return v[key].asFloat();
    }
    return def;
}

double optDouble(Json::Value const & v, std::string const & key, double def)
{
    if (existsDouble(v, key)) {
        return v[key].asDouble();
    }
    return def;
}

std::string optString(Json::Value const & v, std::string const & key, std::string const & def)
{
    if (existsString(v, key)) {
        return v[key].asString();
    }
    return def;
}

bool getValue(Json::Value const & v, bool & val)
{
    switch (v.type()) {
    case Json::booleanValue:
        TBAG_FALLTHROUGH
    case Json::nullValue:
        TBAG_FALLTHROUGH
    case Json::intValue:
        TBAG_FALLTHROUGH
    case Json::uintValue:
        TBAG_FALLTHROUGH
    case Json::realValue:
        val = v.asBool();
        return true;
    default:
        return false;
    }
}

bool getValue(Json::Value const & v, int & val)
{
    return getInt(v, &val);
}

bool getValue(Json::Value const & v, unsigned & val)
{
    if (v.isUInt()) {
        val = v.asUInt();
        return true;
    }
    return false;
}

bool getValue(Json::Value const & v, std::int64_t & val)
{
    if (v.isInt64()) {
        val = v.asInt64();
        return true;
    }
    return false;
}

bool getValue(Json::Value const & v, std::uint64_t & val)
{
    if (v.isUInt64()) {
        val = v.asUInt64();
        return true;
    }
    return false;
}

bool getValue(Json::Value const & v, float & val)
{
    if (v.isDouble()) {
        val = v.asFloat();
        return true;
    }
    return false;
}

bool getValue(Json::Value const & v, double & val)
{
    if (v.isDouble()) {
        val = v.asDouble();
        return true;
    }
    return false;
}

bool getValue(Json::Value const & v, std::string & val)
{
    return getString(v, &val);
}

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

bool getErr(Json::Value const & v, Err * out)
{
    switch (v.type()) {
    case Json::intValue:
        if (existsErr(v.asInt())) {
            if (out != nullptr) {
                *out = toErr(v.asInt());
            }
            return true;
        } else {
            return false;
        }

    case Json::uintValue:
        if (existsErr(static_cast<int>(v.asUInt()))) {
            if (out != nullptr) {
                *out = toErr(static_cast<int>(v.asUInt()));
            }
            return true;
        } else {
            return false;
        }

    case Json::stringValue:
        {
            using namespace libtbag::string;
            auto const text = upper(trim(v.asString()));
            if (text.empty()) {
                return false;
            }
            if (text == "E_UNKNOWN") {
                if (out != nullptr) {
                    *out = E_UNKNOWN;
                }
                return true;
            } else {
                auto const code = libtbag::getErr(text.c_str());
                if (code != E_UNKNOWN) {
                    if (out != nullptr) {
                        *out = code;
                    }
                    return true;
                }
                return false;
            }
        }

    default:
        return false;
    }
}

Err optErr(Json::Value const & v, Err def)
{
    Err result;
    if (getErr(v, &result)) {
        return result;
    } else {
        return def;
    }
}

std::vector<std::string> getStringArray(Json::Value const & v)
{
    std::vector<std::string> result;
    if (getStringArray(v, &result)) {
        return result;
    }
    return {};
}

std::vector<std::string> getStringArray(Json::Value const & v, std::string const & key)
{
    if (exists(v, key)) {
        return getStringArray(v[key]);
    }
    return {};
}

bool getStringArray(Json::Value const & v, std::vector<std::string> * out)
{
    switch (v.type()) {
    case Json::nullValue:
        if (out != nullptr) {
            out->clear();
        }
        return true;
    case Json::intValue:
        if (out != nullptr) {
            out->clear();
            out->push_back(libtbag::string::toString(v.asInt()));
        }
        return true;
    case Json::uintValue:
        if (out != nullptr) {
            out->clear();
            out->push_back(libtbag::string::toString(v.asUInt()));
        }
        return true;
    case Json::realValue:
        if (out != nullptr) {
            out->clear();
            out->push_back(libtbag::string::toString(v.asDouble()));
        }
        return true;
    case Json::stringValue:
        if (out != nullptr) {
            out->clear();
            out->push_back(v.asString());
        }
        return true;
    case Json::booleanValue:
        if (out != nullptr) {
            out->clear();
            out->push_back(libtbag::string::toString(v.asBool()));
        }
        return true;
    case Json::arrayValue:
        if (out != nullptr) {
            auto const size = v.size();
            out->clear();
            out->resize(size);
            for (auto i = 0; i < size; ++i) {
                (*out)[i] = getForceString(v[i]);
            }
        }
        return true;
    case Json::objectValue:
        TBAG_FALLTHROUGH
    default:
        return false;
    }
}

bool getStringArray(Json::Value const & v, std::string const & key, std::vector<std::string> * out)
{
    if (exists(v, key)) {
        return getStringArray(v[key], out);
    }
    return false;
}

template <typename MapT>
static bool getStringObject(Json::Value const & v, MapT * out)
{
    using K = typename MapT::key_type;
    using V = typename MapT::mapped_type;
    using std_map = std::map<K, V>;
    using std_unordered_map = std::unordered_map<K, V>;
    static_assert(std::is_same<MapT, std_map>::value || std::is_same<MapT, std_unordered_map>::value,
                  "Only std::map or std::unordered_map is supported.");

    if (v.type() != Json::objectValue) {
        return false;
    }

    MapT result;
    for (auto const & member : v.getMemberNames()) {
        V member_value;
        if (!getValue(v[member], member_value)) {
            return false;
        }
        result.emplace(K(member), std::move(member_value));
    }

    if (out != nullptr) {
        *out = std::move(result);
    }
    return true;
}

std::map<std::string, std::string> getStringMap(Json::Value const & v)
{
    std::map<std::string, std::string> result;
    if (getStringMap(v, &result)) {
        return result;
    }
    return {};
}

std::map<std::string, std::string> getStringMap(Json::Value const & v, std::string const & key)
{
    if (exists(v, key)) {
        return getStringMap(v[key]);
    }
    return {};
}

bool getStringMap(Json::Value const & v, std::map<std::string, std::string> * out)
{
    return getStringObject(v, out);
}

bool getStringMap(Json::Value const & v, std::string const & key, std::map<std::string, std::string> * out)
{
    if (exists(v, key)) {
        return getStringMap(v[key], out);
    }
    return false;
}

std::unordered_map<std::string, std::string> getStringUnorderedMap(Json::Value const & v)
{
    std::unordered_map<std::string, std::string> result;
    if (getStringUnorderedMap(v, &result)) {
        return result;
    }
    return {};
}

std::unordered_map<std::string, std::string> getStringUnorderedMap(Json::Value const & v, std::string const & key)
{
    if (exists(v, key)) {
        return getStringUnorderedMap(v[key]);
    }
    return {};
}

bool getStringUnorderedMap(Json::Value const & v, std::unordered_map<std::string, std::string> * out)
{
    return getStringObject(v, out);
}

bool getStringUnorderedMap(Json::Value const & v, std::string const & key, std::unordered_map<std::string, std::string> * out)
{
    if (exists(v, key)) {
        return getStringUnorderedMap(v[key], out);
    }
    return false;
}

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

