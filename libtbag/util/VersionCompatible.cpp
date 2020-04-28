/**
 * @file   VersionCompatible.cpp
 * @brief  VersionCompatible class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/util/VersionCompatible.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

Err testCompatible(Version const & origin, std::string const & requirement)
{
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

