/**
 * @file   BoxConverter.hpp
 * @brief  BoxConverter class prototype.
 * @author zer0
 * @date   2020-05-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXCONVERTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXCONVERTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/box/Box.hpp>
#include <libtbag/net/Mime.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

TBAG_API Err convert(libtbag::net::Mime const & mime, Box const & src, Box & dest);
TBAG_API Err convert(std::string const & mime, Box const & src, Box & dest);

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXCONVERTER_HPP__

