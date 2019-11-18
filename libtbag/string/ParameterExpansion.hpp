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
    _ef_normal,                               ///< @code{.cpp} ${parameter @endcode
    _ef_sharp_parameter,                      ///< @code{.cpp} ${#parameter @endcode
    _ef_bang_prefix_asterisk,                 ///< @code{.cpp} ${!prefix* @endcode
    _ef_bang_prefix_at,                       ///< @code{.cpp} ${!prefix@ @endcode
    _ef_bang_name_at,                         ///< @code{.cpp} ${!name[@] @endcode
    _ef_bang_name_asterisk,                   ///< @code{.cpp} ${!name[*] @endcode
    _ef_parameter_colon_minus_word,           ///< @code{.cpp} ${parameter:-word @endcode
    _ef_parameter_colon_equals_word,          ///< @code{.cpp} ${parameter:=word @endcode
    _ef_parameter_colon_question_word,        ///< @code{.cpp} ${parameter:?word @endcode
    _ef_parameter_colon_plus_word,            ///< @code{.cpp} ${parameter:+word @endcode
    _ef_parameter_colon_offset,               ///< @code{.cpp} ${parameter:offset @endcode
    _ef_parameter_colon_offset_colon_length,  ///< @code{.cpp} ${parameter:offset:length @endcode
    _ef_parameter_sharp_word,                 ///< @code{.cpp} ${parameter#word @endcode
    _ef_parameter_sharp_sharp_word,           ///< @code{.cpp} ${parameter##word @endcode
    _ef_parameter_percent_word,               ///< @code{.cpp} ${parameter%word @endcode
    _ef_parameter_percent_percent_word,       ///< @code{.cpp} ${parameter%%word @endcode
    _ef_parameter_slash_pattern_slash_string, ///< @code{.cpp} ${parameter/pattern/string @endcode
    _ef_parameter_carat_pattern,              ///< @code{.cpp} ${parameter^pattern @endcode
    _ef_parameter_carat_carat_pattern,        ///< @code{.cpp} ${parameter^^pattern @endcode
    _ef_parameter_comma_pattern,              ///< @code{.cpp} ${parameter,pattern @endcode
    _ef_parameter_comma_comma_pattern,        ///< @code{.cpp} ${parameter,,pattern @endcode
    _ef_parameter_at_operator_Q,              ///< @code{.cpp} ${parameter@Q @endcode
    _ef_parameter_at_operator_E,              ///< @code{.cpp} ${parameter@E @endcode
    _ef_parameter_at_operator_P,              ///< @code{.cpp} ${parameter@P @endcode
    _ef_parameter_at_operator_A,              ///< @code{.cpp} ${parameter@A @endcode
    _ef_parameter_at_operator_a,              ///< @code{.cpp} ${parameter@a @endcode
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
 *   <dd>
 *     If parameter is unset or null, the expansion of word is substituted.
 *     Otherwise, the value of parameter is substituted.
 *   </dd>
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

