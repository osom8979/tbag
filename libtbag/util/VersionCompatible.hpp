/**
 * @file   VersionCompatible.hpp
 * @brief  VersionCompatible class prototype.
 * @author zer0
 * @date   2020-04-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSIONCOMPATIBLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSIONCOMPATIBLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/Version.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * Test version compatibility.
 *
 * Example:
 * <table>
 *   <caption id="multi_row">Python versioning with requirements.txt syntax</caption>
 *   <tr>
 *     <th>Requirement</th>
 *     <th>Description</th>
 *   </tr>
 *   <tr>
 *     <td>foo</td>
 *     <td>any version of foo</td>
 *   <tr>
 *   <tr>
 *     <td>foo>=5</td>
 *     <td>any version of foo, above or equal to 5</td>
 *   <tr>
 *   <tr>
 *     <td>foo>=5.6</td>
 *     <td>any version of foo, above or equal to 5.6</td>
 *   <tr>
 *   <tr>
 *     <td>foo==5.6.1</td>
 *     <td>exact match</td>
 *   <tr>
 *   <tr>
 *     <td>foo>5</td>
 *     <td>foo-5 or greater, including minor and patch</td>
 *   <tr>
 *   <tr>
 *     <td>foo>5, <5.7</td>
 *     <td>foo-5 or greater, but less than foo-5.7</td>
 *   <tr>
 *   <tr>
 *     <td>foo>0, <5.7</td>
 *     <td>any foo version less than foo-5.7</td>
 *   <tr>
 * </table>
 *
 * @retval E_SUCCESS
 *  Compatible version.
 * @retval E_VERSION
 *  Incompatible version.
 * @retval E_PARSING
 *  Parsing error.
 * @retval E_ILLARGS
 *  Illegal arguments.
 */
TBAG_API Err testCompatible(Version const & origin, std::string const & requirement);
TBAG_API bool isCompatible(Version const & origin, std::string const & requirement);

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSIONCOMPATIBLE_HPP__

