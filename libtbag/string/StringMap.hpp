/**
 * @file   StringMap.hpp
 * @brief  StringMap class prototype.
 * @author zer0
 * @date   2019-09-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRINGMAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRINGMAP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

using StringMap = std::map<std::string, std::string>;
using StringUnorderedMap = std::unordered_map<std::string, std::string>;

TBAG_API StringUnorderedMap convert(StringMap const & m);
TBAG_API StringMap convert(StringUnorderedMap const & m);

TBAG_API std::vector<std::string> keys(StringMap const & m);
TBAG_API std::vector<std::string> keys(StringUnorderedMap const & m, bool sorting = false);

TBAG_API bool get(StringMap const & m, std::string const & key, std::string & out);
TBAG_API bool get(StringUnorderedMap const & m, std::string const & key, std::string & out);

TBAG_API bool get(StringMap const & m, std::string const & key, bool & out);
TBAG_API bool get(StringUnorderedMap const & m, std::string const & key, bool & out);

TBAG_API bool get(StringMap const & m, std::string const & key, int & out);
TBAG_API bool get(StringUnorderedMap const & m, std::string const & key, int & out);

TBAG_API bool get(StringMap const & m, std::string const & key, unsigned & out);
TBAG_API bool get(StringUnorderedMap const & m, std::string const & key, unsigned & out);

TBAG_API bool get(StringMap const & m, std::string const & key, float & out);
TBAG_API bool get(StringUnorderedMap const & m, std::string const & key, float & out);

TBAG_API bool get(StringMap const & m, std::string const & key, double & out);
TBAG_API bool get(StringUnorderedMap const & m, std::string const & key, double & out);

template <typename ValT, typename MapT>
ValT opt(MapT const & m, std::string const & key, ValT const & def = ValT{})
{
    ValT out;
    if (!get(m, key, out)) {
        return def;
    }
    return out;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_STRINGMAP_HPP__

