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
#include <libtbag/Err.hpp>
#include <libtbag/dom/json/jsoncpp/json.h>

#include <cstdint>
#include <vector>
#include <string>

#include <map>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace json {

TBAG_API bool parse(std::string const & json, Json::Value & result, std::string * error_message = nullptr);
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

TBAG_API bool optBool(Json::Value const & v, std::string const & key, bool def = false);
TBAG_API int optInt(Json::Value const & v, std::string const & key, int def = 0);
TBAG_API unsigned optUInt(Json::Value const & v, std::string const & key, unsigned def = 0u);
TBAG_API std::int64_t optInt64(Json::Value const & v, std::string const & key, std::int64_t def = 0);
TBAG_API std::uint64_t optUInt64(Json::Value const & v, std::string const & key, std::uint64_t def = 0u);
TBAG_API float optFloat(Json::Value const & v, std::string const & key, float def = 0.0f);
TBAG_API double optDouble(Json::Value const & v, std::string const & key, double def = 0.0);
TBAG_API std::string optString(Json::Value const & v, std::string const & key, std::string const & def = {});

TBAG_API bool getValue(Json::Value const & v, bool & val);
TBAG_API bool getValue(Json::Value const & v, int & val);
TBAG_API bool getValue(Json::Value const & v, unsigned & val);
TBAG_API bool getValue(Json::Value const & v, std::int64_t & val);
TBAG_API bool getValue(Json::Value const & v, std::uint64_t & val);
TBAG_API bool getValue(Json::Value const & v, float & val);
TBAG_API bool getValue(Json::Value const & v, double & val);
TBAG_API bool getValue(Json::Value const & v, std::string & val);

TBAG_API std::string getForceString(Json::Value const & v);
TBAG_API std::string getForceString(Json::Value const & v, std::string const & key);

TBAG_API bool getString(Json::Value const & v, std::string * out);
TBAG_API bool getString(Json::Value const & v, std::string const & key, std::string * out);

TBAG_API bool getInt(Json::Value const & v, int * out);
TBAG_API bool getInt(Json::Value const & v, std::string const & key, int * out);

TBAG_API bool getErr(Json::Value const & v, Err * out);
TBAG_API Err optErr(Json::Value const & v, Err def = E_UNKNOWN);

TBAG_API std::vector<std::string> getStringArray(Json::Value const & v);
TBAG_API std::vector<std::string> getStringArray(Json::Value const & v, std::string const & key);

TBAG_API bool getStringArray(Json::Value const & v, std::vector<std::string> * out);
TBAG_API bool getStringArray(Json::Value const & v, std::string const & key, std::vector<std::string> * out);

template <typename Iterator>
Json::Value toJsonArray(Iterator begin, Iterator end)
{
    Json::Value result(Json::arrayValue);
    for (; begin != end; ++begin) {
        result.append(*begin);
    }
    return result;
}

template <typename T>
Json::Value toJsonArray(std::vector<T> const & val)
{
    return toJsonArray(val.begin(), val.end());
}

TBAG_API std::map<std::string, std::string> getStringMap(Json::Value const & v);
TBAG_API std::map<std::string, std::string> getStringMap(Json::Value const & v, std::string const & key);

TBAG_API bool getStringMap(Json::Value const & v, std::map<std::string, std::string> * out);
TBAG_API bool getStringMap(Json::Value const & v, std::string const & key, std::map<std::string, std::string> * out);

TBAG_API std::unordered_map<std::string, std::string> getStringUnorderedMap(Json::Value const & v);
TBAG_API std::unordered_map<std::string, std::string> getStringUnorderedMap(Json::Value const & v, std::string const & key);

TBAG_API bool getStringUnorderedMap(Json::Value const & v, std::unordered_map<std::string, std::string> * out);
TBAG_API bool getStringUnorderedMap(Json::Value const & v, std::string const & key, std::unordered_map<std::string, std::string> * out);

template <typename Iterator>
Json::Value toJsonObject(Iterator begin, Iterator end)
{
    Json::Value result(Json::objectValue);
    for (; begin != end; ++begin) {
        result[begin->first] = begin->second;
    }
    return result;
}

template <typename K, typename V>
Json::Value toJsonObject(std::map<K, V> const & val)
{
    return toJsonObject(val.begin(), val.end());
}

template <typename K, typename V>
Json::Value toJsonObject(std::unordered_map<K, V> const & val)
{
    return toJsonObject(val.begin(), val.end());
}

} // namespace json
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_JSON_JSONUTILS_HPP__

