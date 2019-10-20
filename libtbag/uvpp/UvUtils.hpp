/**
 * @file   UvUtils.hpp
 * @brief  uvpp miscellaneous utilities.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

/** Store the program arguments. Required for getting/setting the process title. */
TBAG_API char ** setupArgs(int argc, char ** argv);

/** Gets the title of the current process. */
TBAG_API std::string getProcessTitle();

/** Sets the current process title. */
TBAG_API void setProcessTitle(std::string const & title);

TBAG_CONSTEXPR std::size_t const DEFAULT_ENVIRONMENT_VARIABLE_BUFFER_SIZE = 2048;

/**
 * Retrieves the environment variable specified by name.
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API Err getEnv(std::string const & name, std::string & value);

/**
 * Creates or updates the environment variable specified by name with value.
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API Err setEnv(std::string const & name, std::string const & value);

/**
 * Deletes the environment variable specified by name.
 * If no such environment variable exists, this function returns successfully.
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API Err unsetEnv(std::string const & name);

/**
 * Returns the hostname as a null-terminated string
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API std::string getHostName();

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVUTILS_HPP__

