/**
 * @file   LogicalOperator.hpp
 * @brief  LogicalOperator class prototype.
 * @author zer0
 * @date   2020-04-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LEXER_LOGICALOPERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LEXER_LOGICALOPERATOR_HPP__

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

enum logical_operator_t
{
    logical_operator_and,
    logical_operator_or,
    logical_operator_done, ///< Don't use the right-side statement.
    logical_operator_error
};

struct Statement
{
    logical_operator_t op = logical_operator_done;
    std::string left;
    std::string right;
};

TBAG_API Statement splitStatement(std::string const & origin);

} // namespace lexer

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LEXER_LOGICALOPERATOR_HPP__

