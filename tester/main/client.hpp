/**
 * @file   client.hpp
 * @brief  client class prototype.
 * @author zer0
 * @date   2016-11-04
 */

#ifndef __INCLUDE_LIBTBAG__TESTER_MAIN_CLIENT_HPP__
#define __INCLUDE_LIBTBAG__TESTER_MAIN_CLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

int main_client(std::string const & ip, int port);

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__TESTER_MAIN_CLIENT_HPP__

