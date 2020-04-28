/**
 * @file   VersionCompatible.cpp
 * @brief  VersionCompatible class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/util/VersionCompatible.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/lexer/LogicalOperator.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

Err testCompatible(Version const & origin, std::string const & requirement)
{
//    libtbag::string::replace(requirement, ",", "&&")
//    using namespace libtbag::lexer;
//    auto logical = splitLogical(libtbag::string::trim(requirement));
//    if (logical.op == logical_operator_error) {
//        return E_PARSING;
//    }
//
//    libtbag::string::trim(logical.left);
//    libtbag::string::trim(logical.right);
//
//    switch (logical.op) {
//    case logical_operator_and:
//    case logical_operator_or:
//    case logical_operator_done:
//    case logical_operator_error:
//    default:
//        TBAG_INACCESSIBLE_BLOCK_ASSERT();
//        return E_INACCESSIBLE_BLOCK;
//    }

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

