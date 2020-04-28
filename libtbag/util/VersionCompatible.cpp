/**
 * @file   VersionCompatible.cpp
 * @brief  VersionCompatible class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/util/VersionCompatible.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/lexer/LogicalOperator.hpp>
#include <libtbag/lexer/ComparisonOperator.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

static Err __test_compatible(Version const & origin, std::string const & requirement)
{
    using namespace libtbag::lexer;
    auto logical = splitLogical(libtbag::string::trim(requirement));
    if (logical.op == logical_operator_error) {
        return E_PARSING;
    }

    auto comparison = splitComparison(logical.left);
    if (comparison.op == comparison_operator_error) {
        return E_PARSING;
    }

    std::string comp_version_text;
    if (comparison.op == comparison_operator_last) {
        comp_version_text = comparison.left;
    } else {
        comp_version_text = comparison.right;
    }

    Version comp_version;
    auto const code = comp_version.fromString(comp_version_text);
    if (isFailure(code)) {
        return E_PARSING;
    }

    bool comp = false;
    switch (comparison.op) {
    case comparison_operator_equal:         comp = (origin == comp_version); break;
    case comparison_operator_not_equal:     comp = (origin != comp_version); break;
    case comparison_operator_greater_than:  comp = (origin >  comp_version); break;
    case comparison_operator_greater_equal: comp = (origin >= comp_version); break;
    case comparison_operator_less_than:     comp = (origin <  comp_version); break;
    case comparison_operator_less_equal:    comp = (origin <= comp_version); break;
    case comparison_operator_last:          comp = (origin == comp_version); break;
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return E_INACCESSIBLE_BLOCK;
    }

    switch (logical.op) {
    case logical_operator_and:
        if (comp) {
            return __test_compatible(origin, logical.right);
        } else {
            return E_VERSION;
        }

    case logical_operator_or:
        if (comp) {
            return E_SUCCESS;
        } else {
            return __test_compatible(origin, logical.right);
        }

    case logical_operator_last:
        return comp ? E_SUCCESS : E_VERSION;

    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return E_INACCESSIBLE_BLOCK;
    }
}

Err testCompatible(Version const & origin, std::string const & requirement)
{
    using namespace libtbag::string;
    return __test_compatible(origin, trim(replace(requirement, ",", "&&")));
}

bool isCompatible(Version const & origin, std::string const & requirement)
{
    return testCompatible(origin, requirement) == E_SUCCESS;
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

