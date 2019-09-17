/**
 * @file   StringMap.cpp
 * @brief  StringMap class implementation.
 * @author zer0
 * @date   2019-09-17
 */

#include <libtbag/string/StringMap.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <iterator>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

StringUnorderedMap convert(StringMap const & m)
{
    return {m.begin(), m.end()};
}

StringMap convert(StringUnorderedMap const & m)
{
    return {m.begin(), m.end()};
}

std::vector<std::string> keys(StringMap const & m)
{
    std::vector<std::string> keys;
    std::transform(m.begin(), m.end(), std::back_inserter(keys), [](StringMap::value_type const & e){
        return e.first;
    });
    return keys;
}

std::vector<std::string> keys(StringUnorderedMap const & m, bool sorting)
{
    std::vector<std::string> keys;
    std::transform(m.begin(), m.end(), std::back_inserter(keys), [](StringUnorderedMap::value_type const & e){
        return e.first;
    });
    if (sorting) {
        std::sort(keys.begin(), keys.end());
    }
    return keys;
}

bool get(StringMap const & m, std::string const & key, std::string & out)
{
    auto itr = m.find(key);
    if (itr == m.end()) {
        return false;
    }
    out = itr->second;
    return true;
}

bool get(StringUnorderedMap const & m, std::string const & key, std::string & out)
{
    auto itr = m.find(key);
    if (itr == m.end()) {
        return false;
    }
    out = itr->second;
    return true;
}

template <typename MapT, typename ValT>
static bool __get(MapT const & m, std::string const & key, ValT & out)
{
    auto itr = m.find(key);
    if (itr == m.end()) {
        return false;
    }
    out = libtbag::string::toValue<ValT>(itr->second);
    return true;
}

bool get(StringMap const & m, std::string const & key, bool & out)
{
    return __get(m, key, out);
}

bool get(StringUnorderedMap const & m, std::string const & key, bool & out)
{
    return __get(m, key, out);
}

bool get(StringMap const & m, std::string const & key, int & out)
{
    return __get(m, key, out);
}

bool get(StringUnorderedMap const & m, std::string const & key, int & out)
{
    return __get(m, key, out);
}

bool get(StringMap const & m, std::string const & key, unsigned & out)
{
    return __get(m, key, out);
}

bool get(StringUnorderedMap const & m, std::string const & key, unsigned & out)
{
    return __get(m, key, out);
}

bool get(StringMap const & m, std::string const & key, float & out)
{
    return __get(m, key, out);
}

bool get(StringUnorderedMap const & m, std::string const & key, float & out)
{
    return __get(m, key, out);
}

bool get(StringMap const & m, std::string const & key, double & out)
{
    return __get(m, key, out);
}

bool get(StringUnorderedMap const & m, std::string const & key, double & out)
{
    return __get(m, key, out);
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

