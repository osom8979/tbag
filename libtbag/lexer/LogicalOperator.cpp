/**
 * @file   LogicalOperator.cpp
 * @brief  LogicalOperator class implementation.
 * @author zer0
 * @date   2020-04-28
 */

#include <libtbag/lexer/LogicalOperator.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lexer {

enum logical_operator_state_t
{
    logical_operator_state_normal,
    logical_operator_state_comma,
    logical_operator_state_and,
    logical_operator_state_or,
};

Statement splitStatement(std::string const & origin)
{
    auto const size = origin.size();
    logical_operator_state_t state = logical_operator_state_normal;
    std::size_t i = 0;

    Statement result;

    for (; i < size; ++i) {
        // clang-format on
        switch (origin[i]) {
            case ',': state = logical_operator_state_comma; break;
            case '&': state = logical_operator_state_and; break;
            case '|': state = logical_operator_state_or; break;
            default: break;
        }
        // clang-format off

        if (state != logical_operator_state_normal) {
            break;
        }
    }

    if (state == logical_operator_state_normal) {
        result.op = logical_operator_done;
        result.left = origin;
        return result;
    }

    result.left = origin.substr(0, i);
    ++i;
    assert(i <= size);
    if (i == size) {
        result.op = logical_operator_done;
        return result;
    }

    if (state == logical_operator_state_comma) {
        result.op = logical_operator_and;
        result.right = origin.substr(i);
        return result;
    }

    if (state == logical_operator_state_and) {
        switch (origin[i]) {
            case ',':
                result.op = logical_operator_error;
                break;
            case '&':
                if (i+1 < size) {
                    result.op = logical_operator_and;
                    result.right = origin.substr(i+1);
                } else {
                    result.op = logical_operator_done;
                }
                break;
            case '|':
                result.op = logical_operator_error;
                break;
            default:
                result.op = logical_operator_and;
                result.right = origin.substr(i);
                break;
        }

    } else {
        assert(state == logical_operator_state_or);
        switch (origin[i]) {
            case ',':
                result.op = logical_operator_error;
                break;
            case '&':
                result.op = logical_operator_error;
                break;
            case '|':
                if (i+1 < size) {
                    result.op = logical_operator_or;
                    result.right = origin.substr(i+1);
                } else {
                    result.op = logical_operator_done;
                }
                break;
            default:
                result.op = logical_operator_or;
                result.right = origin.substr(i);
                break;
        }
    }

    return result;
}

} // namespace lexer

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

