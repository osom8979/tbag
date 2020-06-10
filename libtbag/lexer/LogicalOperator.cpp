/**
 * @file   LogicalOperator.cpp
 * @brief  LogicalOperator class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/lexer/LogicalOperator.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lexer {

Logical splitLogical(std::string const & origin)
{
    using namespace libtbag::string;
    auto const src = trim(origin);
    auto const size = src.size();
    if (size == 0) {
        return { logical_operator_error };
    }

    for (auto i = 0; i < size; ++i) {
        if (src[i] == '&') {
            if (i+1 < size && src[i+1] == '&') {
                return { logical_operator_and, trim(src.substr(0, i)), trim(src.substr(i+2)) };
            } else {
                return { logical_operator_and, trim(src.substr(0, i)), trim(src.substr(i+1)) };
            }
        }
        if (src[i] == '|') {
            if (i+1 < size && src[i+1] == '|') {
                return { logical_operator_or, trim(src.substr(0, i)), trim(src.substr(i+2)) };
            } else {
                return { logical_operator_or, trim(src.substr(0, i)), trim(src.substr(i+1)) };
            }
        }
    }

    return { logical_operator_last, src };
}

} // namespace lexer

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

