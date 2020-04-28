/**
 * @file   ComparisonOperator.hpp
 * @brief  ComparisonOperator class prototype.
 * @author zer0
 * @date   2020-04-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LEXER_COMPARISONOPERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LEXER_COMPARISONOPERATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lexer {

enum comparison_operator_t
{
    comparison_operator_equal,
    comparison_operator_not_equal,
    comparison_operator_greater_than,
    comparison_operator_greater_equal,
    comparison_operator_less_than,
    comparison_operator_less_equal,
    comparison_operator_last,
    comparison_operator_error
};

struct Comparison
{
    comparison_operator_t op = comparison_operator_last;
    std::string left;
    std::string right;
};

TBAG_API Comparison splitComparison(std::string const & origin);

} // namespace lexer

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LEXER_COMPARISONOPERATOR_HPP__

