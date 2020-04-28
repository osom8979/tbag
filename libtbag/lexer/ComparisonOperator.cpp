/**
 * @file   ComparisonOperator.cpp
 * @brief  ComparisonOperator class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/lexer/ComparisonOperator.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lexer {

Comparison splitComparison(std::string const & origin)
{
    using namespace libtbag::string;
    auto const src = trim(origin);
    auto const size = src.size();
    if (size == 0) {
        return { comparison_operator_error };
    }

    for (auto i = 0; i < size; ++i) {
        switch (src[i]) {
        case '=':
            if (i+1 < size && src[i+1] == '=') {
                return { comparison_operator_equal,
                         trim(src.substr(0, i)),
                         trim(src.substr(i+2))
                };
            }
            break;

        case '!':
            if (i+1 < size && src[i+1] == '=') {
                return { comparison_operator_not_equal,
                         trim(src.substr(0, i)),
                         trim(src.substr(i+2))
                };
            }
            break;

        case '>':
            if (i+1 >= size) {
                return { comparison_operator_greater_than,
                         trim(src.substr(0, i))
                };
            } else {
                assert(i+1 < size);
                if (src[i+1] == '=') {
                    return { comparison_operator_greater_equal,
                             trim(src.substr(0, i)),
                             trim(src.substr(i+2))
                    };
                } else {
                    return { comparison_operator_greater_than,
                             trim(src.substr(0, i)),
                             trim(src.substr(i+1))
                    };
                }
            }

        case '<':
            if (i+1 >= size) {
                return { comparison_operator_less_than,
                         trim(src.substr(0, i))
                };
            } else {
                assert(i+1 < size);
                if (src[i+1] == '=') {
                    return { comparison_operator_less_equal,
                             trim(src.substr(0, i)),
                             trim(src.substr(i+2))
                    };
                } else {
                    return { comparison_operator_less_than,
                             trim(src.substr(0, i)),
                             trim(src.substr(i+1))
                    };
                }
            }

        default:
            break;
        }
    }

    return {comparison_operator_last, src};
}

} // namespace lexer

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

