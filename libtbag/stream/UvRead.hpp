/**
 * @file   UvRead.hpp
 * @brief  UvRead class prototype.
 * @author zer0
 * @date   2016-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVREAD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVREAD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/loop/event/UvHandler.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace stream {

/**
 * UvRead class prototype.
 *
 * @author zer0
 * @date   2016-11-03
 */
class TBAG_API UvRead : public libtbag::loop::event::UvHandler
{
public:
    UvRead();
    virtual ~UvRead();
};

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVREAD_HPP__

