/**
 * @file   ParameterExpansion.hpp
 * @brief  ParameterExpansion class prototype.
 * @author zer0
 * @date   2019-10-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_PARAMETEREXPANSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_PARAMETEREXPANSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/string/Flags.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

enum expansion_state_t
{
    expansion_state_enable_variable,
    expansion_state_enable_colon,
    expansion_state_enable_colon_minus,
    expansion_state_enable_colon_equals,
    expansion_state_enable_colon_question,
    expansion_state_enable_colon_plus,
    expansion_state_enable_colon_offset,
    expansion_state_enable_colon_offset_colon,
    expansion_state_enable_sharp,
    expansion_state_enable_sharp_done,
    expansion_state_enable_sharp_sharp,
    expansion_state_enable_percent,
    expansion_state_enable_percent_done,
    expansion_state_enable_percent_percent,
    expansion_state_enable_slash,
    expansion_state_enable_slash_slash,
    expansion_state_enable_carat,
    expansion_state_enable_carat_done,
    expansion_state_enable_carat_carat,
    expansion_state_enable_comma,
    expansion_state_enable_comma_done,
    expansion_state_enable_comma_comma,
    expansion_state_enable_at,
};

// clang-format off
enum expansion_format_t
{
    _ef_unknown,
    _ef_normal,                               ///< <code>${parameter}</code>
    _ef_sharp_parameter,                      ///< <code>${#parameter}</code>
    _ef_bang_prefix_asterisk,                 ///< <code>${!prefix*}</code>
    _ef_bang_prefix_at,                       ///< <code>${!prefix@}</code>
    _ef_bang_name_at,                         ///< <code>${!name[@]}</code>
    _ef_bang_name_asterisk,                   ///< <code>${!name[*]}</code>
    _ef_parameter_colon_minus_word,           ///< <code>${parameter:-word}</code>
    _ef_parameter_colon_equals_word,          ///< <code>${parameter:=word}</code>
    _ef_parameter_colon_question_word,        ///< <code>${parameter:?word}</code>
    _ef_parameter_colon_plus_word,            ///< <code>${parameter:+word}</code>
    _ef_parameter_colon_offset,               ///< <code>${parameter:offset}</code>
    _ef_parameter_colon_offset_colon_length,  ///< <code>${parameter:offset:length}</code>
    _ef_parameter_sharp_word,                 ///< <code>${parameter#word}</code>
    _ef_parameter_sharp_sharp_word,           ///< <code>${parameter##word}</code>
    _ef_parameter_percent_word,               ///< <code>${parameter%word}</code>
    _ef_parameter_percent_percent_word,       ///< <code>${parameter%%word}</code>
    _ef_parameter_slash_pattern_slash_string, ///< <code>${parameter/pattern/string}</code>
    _ef_parameter_carat_pattern,              ///< <code>${parameter^pattern}</code>
    _ef_parameter_carat_carat_pattern,        ///< <code>${parameter^^pattern}</code>
    _ef_parameter_comma_pattern,              ///< <code>${parameter,pattern}</code>
    _ef_parameter_comma_comma_pattern,        ///< <code>${parameter,,pattern}</code>
    _ef_parameter_at_operator_Q,              ///< <code>${parameter@Q}</code>
    _ef_parameter_at_operator_E,              ///< <code>${parameter@E}</code>
    _ef_parameter_at_operator_P,              ///< <code>${parameter@P}</code>
    _ef_parameter_at_operator_A,              ///< <code>${parameter@A}</code>
    _ef_parameter_at_operator_a,              ///< <code>${parameter@a}</code>
};
// clang-format on

/**
 * Use parameter extensions.
 *
 * Supported features:
 * <dl>
 *   <dt>
 *     <code>${parameter:-word}</code>
 *   </dt>
 *   <dl>
 *     If parameter is unset or null, the expansion of word is substituted.
 *     Otherwise, the value of parameter is substituted.
 *   </dl>
 * </dl>
 *
 * @param[in] variable
 *      Parameter variable. Prefix(<code>${</code>) and Suffix(<code>}</code>) should be excluded.
 * @param[in] flags
 *      Flags object.
 * @param[out] result
 *      Extended converted result.
 *
 * @return
 *  Error code.
 *
 * @see <https://www.gnu.org/software/bash/manual/html_node/Shell-Parameter-Expansion.html>
 */
TBAG_API Err parameterExpansion(std::string const & variable,
                                Flags const & flags,
                                std::string & result);

/**
 * Strip parameter decoration.
 * - Trim variable
 * - Remove prefix(<code>${</code>)
 * - Remove suffix(<code>}</code>)
 */
TBAG_API std::string stripParameterDecoration(std::string const & variable);

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_PARAMETEREXPANSION_HPP__

