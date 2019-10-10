/**
 * @file   ParameterExpansion.cpp
 * @brief  ParameterExpansion class implementation.
 * @author zer0
 * @date   2019-10-10
 */

#include <libtbag/string/ParameterExpansion.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Err parameterExpansion(std::string const & variable, Flags const & flags, std::string & result)
{
    if (variable.empty()) {
        return E_ILLARGS;
    }

    auto const size = variable.size();
    assert(size >= 1);

    if (variable[0] == '#') {
        result = libtbag::string::toString(flags.opt(variable.substr(1)).size());
        return E_SUCCESS;
    }

    if (variable[0] == '!') {
        // TODO: implement this expansion.
        return E_ENOSYS;
    }

    expansion_state_t state = expansion_state_enable_variable;
    expansion_format_t format = _ef_unknown;

    std::stringstream name_ss;
    std::stringstream word_ss;
    std::stringstream patt_ss;
    std::stringstream sub_ss; // substitution
    std::stringstream offset_ss;
    std::stringstream length_ss;

    char c = 0;

    for (std::size_t i = 0; i < size; ++i) {
        c = variable[i];
        switch (state) {
        case expansion_state_enable_variable:
            switch (c) {
            case ':': state = expansion_state_enable_colon;   break;
            case '#': state = expansion_state_enable_sharp;   break;
            case '%': state = expansion_state_enable_percent; break;
            case '/': state = expansion_state_enable_slash;   break;
            case '^': state = expansion_state_enable_carat;   break;
            case ',': state = expansion_state_enable_comma;   break;
            case '@': state = expansion_state_enable_at;      break;
            default:
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                name_ss << c;
                break;
            }
            break;

        case expansion_state_enable_colon:
            switch (c) {
            case '-': state = expansion_state_enable_colon_minus;    break;
            case '=': state = expansion_state_enable_colon_equals;   break;
            case '?': state = expansion_state_enable_colon_question; break;
            case '+': state = expansion_state_enable_colon_plus;     break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                state = expansion_state_enable_colon_offset;
                offset_ss << c;
                break;
            default:
                return E_PARSING;
            }
            break;

        case expansion_state_enable_colon_minus:
            TBAG_FALLTHROUGH
        case expansion_state_enable_colon_equals:
            TBAG_FALLTHROUGH
        case expansion_state_enable_colon_question:
            TBAG_FALLTHROUGH
        case expansion_state_enable_colon_plus:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            word_ss << c;
            break;

        case expansion_state_enable_colon_offset:
            switch (c) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                offset_ss << c;
                break;
            case ':':
                state = expansion_state_enable_colon_offset_colon;
                break;
            default:
                return E_PARSING;
            }
            break;

        case expansion_state_enable_colon_offset_colon:
            switch (c) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                length_ss << c;
                break;
            default:
                return E_PARSING;
            }
            break;

        case expansion_state_enable_sharp:
            if (c == '#') {
                state = expansion_state_enable_sharp_sharp;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_sharp_done;
                word_ss << c;
            }
            break;

        case expansion_state_enable_sharp_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_sharp_sharp:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            word_ss << c;
            break;

        case expansion_state_enable_percent:
            if (c == '%') {
                state = expansion_state_enable_percent_percent;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_percent_done;
                word_ss << c;
            }
            break;

        case expansion_state_enable_percent_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_percent_percent:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            word_ss << c;
            break;

        case expansion_state_enable_slash:
            if (c == '/') {
                state = expansion_state_enable_slash_slash;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                patt_ss << c;
            }
            break;

        case expansion_state_enable_slash_slash:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            sub_ss << c;
            break;

        case expansion_state_enable_carat:
            if (c == '^') {
                state = expansion_state_enable_carat_carat;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_carat_done;
                patt_ss << c;
            }
            break;

        case expansion_state_enable_carat_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_carat_carat:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            patt_ss << c;
            break;

        case expansion_state_enable_comma:
            if (c == ',') {
                state = expansion_state_enable_carat_carat;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_comma_done;
                patt_ss << c;
            }
            break;

        case expansion_state_enable_comma_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_comma_comma:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            patt_ss << c;
            break;

        case expansion_state_enable_at:
            switch (c) {
            case 'Q': format = _ef_parameter_at_operator_Q; break;
            case 'E': format = _ef_parameter_at_operator_E; break;
            case 'P': format = _ef_parameter_at_operator_P; break;
            case 'A': format = _ef_parameter_at_operator_A; break;
            case 'a': format = _ef_parameter_at_operator_a; break;
            default:
                return E_PARSING;
            }
            break;
        }

        if (format != _ef_unknown) {
            break;
        }
    }

    // Select result format.
    if (format == _ef_unknown) {
        switch (state) {
        case expansion_state_enable_variable:
            format = _ef_normal;
            break;

        case expansion_state_enable_colon:
            return E_ILLSTATE;
        case expansion_state_enable_colon_minus:
            format = _ef_parameter_colon_minus_word;
            break;
        case expansion_state_enable_colon_equals:
            format = _ef_parameter_colon_equals_word;
            break;
        case expansion_state_enable_colon_question:
            format = _ef_parameter_colon_question_word;
            break;
        case expansion_state_enable_colon_plus:
            format = _ef_parameter_colon_plus_word;
            break;

        case expansion_state_enable_colon_offset:
            format = _ef_parameter_colon_offset;
            break;
        case expansion_state_enable_colon_offset_colon:
            format = _ef_parameter_colon_offset_colon_length;
            break;

        case expansion_state_enable_sharp:
            return E_ILLSTATE;
        case expansion_state_enable_sharp_done:
            format = _ef_parameter_sharp_word;
            break;
        case expansion_state_enable_sharp_sharp:
            format = _ef_parameter_sharp_sharp_word;
            break;

        case expansion_state_enable_percent:
            return E_ILLSTATE;
        case expansion_state_enable_percent_done:
            format = _ef_parameter_percent_word;
            break;
        case expansion_state_enable_percent_percent:
            format = _ef_parameter_percent_percent_word;
            break;

        case expansion_state_enable_slash:
            return E_ILLSTATE;
        case expansion_state_enable_slash_slash:
            format = _ef_parameter_slash_pattern_slash_string;
            break;

        case expansion_state_enable_carat:
            return E_ILLSTATE;
        case expansion_state_enable_carat_done:
            format = _ef_parameter_carat_pattern;
            break;
        case expansion_state_enable_carat_carat:
            format = _ef_parameter_carat_carat_pattern;
            break;

        case expansion_state_enable_comma:
            return E_ILLSTATE;
        case expansion_state_enable_comma_done:
            format = _ef_parameter_comma_pattern;
            break;
        case expansion_state_enable_comma_comma:
            format = _ef_parameter_comma_comma_pattern;
            break;

        case expansion_state_enable_at:
            return E_ILLSTATE;
        default:
            return E_INACCESSIBLE_BLOCK;
        }
    }

    assert(format != _ef_unknown);
    assert(format != _ef_sharp_parameter);
    assert(format != _ef_bang_prefix_asterisk);
    assert(format != _ef_bang_prefix_at);
    assert(format != _ef_bang_name_at);
    assert(format != _ef_bang_name_asterisk);

    auto const name = name_ss.str();
    auto const word = word_ss.str();
    auto const patt = patt_ss.str();
    auto const sub = sub_ss.str();
    auto const offset = offset_ss.str();
    auto const length = length_ss.str();

    switch (format) {
    case _ef_normal:
        result = flags.opt(name);
        return E_SUCCESS;

    case _ef_parameter_colon_minus_word:
        // If parameter is unset or null, the expansion of word is substituted.
        // Otherwise, the value of parameter is substituted.
        if (flags.existsByKey(name)) {
            auto const value = flags.opt(name);
            if (value.empty()) {
                result = word;
            } else {
                result = value;
            }
        } else {
            result = word;
        }
        return E_SUCCESS;

    case _ef_parameter_colon_equals_word:
        return E_ENOSYS;
    case _ef_parameter_colon_question_word:
        return E_ENOSYS;
    case _ef_parameter_colon_plus_word:
        return E_ENOSYS;
    case _ef_parameter_colon_offset:
        return E_ENOSYS;
    case _ef_parameter_colon_offset_colon_length:
        return E_ENOSYS;
    case _ef_parameter_sharp_word:
        return E_ENOSYS;
    case _ef_parameter_sharp_sharp_word:
        return E_ENOSYS;
    case _ef_parameter_percent_word:
        return E_ENOSYS;
    case _ef_parameter_percent_percent_word:
        return E_ENOSYS;
    case _ef_parameter_slash_pattern_slash_string:
        return E_ENOSYS;
    case _ef_parameter_carat_pattern:
        return E_ENOSYS;
    case _ef_parameter_carat_carat_pattern:
        return E_ENOSYS;
    case _ef_parameter_comma_pattern:
        return E_ENOSYS;
    case _ef_parameter_comma_comma_pattern:
        return E_ENOSYS;
    case _ef_parameter_at_operator_Q:
        return E_ENOSYS;
    case _ef_parameter_at_operator_E:
        return E_ENOSYS;
    case _ef_parameter_at_operator_P:
        return E_ENOSYS;
    case _ef_parameter_at_operator_A:
        return E_ENOSYS;
    case _ef_parameter_at_operator_a:
        return E_ENOSYS;
    default:
        return E_INACCESSIBLE_BLOCK;
    }
}

std::string stripParameterDecoration(std::string const & variable)
{
    auto temp = libtbag::string::trim(variable);
    auto const size = temp.size();
    if (size >= 3u && temp[0] == '$' && temp[1] == '{' && temp.back() == '}') {
        return temp.substr(2, size-3);
    }
    return temp;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

