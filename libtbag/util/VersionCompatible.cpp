/**
 * @file   VersionCompatible.cpp
 * @brief  VersionCompatible class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/util/VersionCompatible.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

Err testCompatible(Version const & origin, std::string const & requirement)
{
    auto const req = libtbag::string::trim(requirement);
    if (req.empty()) {
        return E_ILLARGS;
    }

    std::string left;
    std::string right;

    return E_SUCCESS;
}

bool isCompatible(Version const & origin, std::string const & requirement)
{
    return testCompatible(origin, requirement) == E_SUCCESS;
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

