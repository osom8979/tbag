/**
 * @file   Execute.hpp
 * @brief  Execute class prototype.
 * @author zer0
 * @date   2017-08-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_EXECUTE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_EXECUTE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

TBAG_API int execute(std::string const & file, std::vector<std::string> const & args, std::vector<std::string> const & envs, std::string const & cwd, Err * code = nullptr);
TBAG_API int execute(std::string const & file, std::vector<std::string> const & args, std::vector<std::string> const & envs, Err * code = nullptr);
TBAG_API int execute(std::string const & file, std::vector<std::string> const & args, Err * code = nullptr);
TBAG_API int execute(std::vector<std::string> const & args, Err * code = nullptr);
TBAG_API int execute(std::string const & args, Err * code = nullptr);

TBAG_API int runShell(std::vector<std::string> const & args, Err * code = nullptr);
TBAG_API int runShellScript(std::string const & file, Err * code = nullptr);
TBAG_API int runShellCommand(std::string const & command, Err * code = nullptr);

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_EXECUTE_HPP__

