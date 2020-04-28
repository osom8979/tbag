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
    auto const src = libtbag::string::trim(origin);
    auto const size = src.size();
    if (size == 0) {
        return { logical_operator_error };
    }

    for (auto i = 0; /*i<size&&*/ i+1 < size; ++i) {
        if (src[i] == '&' && src[i+1] == '&') {
            return { logical_operator_and,
                     libtbag::string::trim(src.substr(0, i)),
                     libtbag::string::trim(src.substr(i+2))
            };
        }
        if (src[i] == '|' && src[i+1] == '|') {
            return { logical_operator_or,
                     libtbag::string::trim(src.substr(0, i)),
                     libtbag::string::trim(src.substr(i+2))
            };
        }
    }
    return { logical_operator_last, src };
}

} // namespace lexer

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

