/**
 * @file   TbagPacket.hpp
 * @brief  TbagPacket class prototype.
 * @author zer0
 * @date   2017-09-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

// Forward declaration.

/**
 * TbagPacket class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TbagPacket : private Noncopyable
{
private:
    // Insert member variables.

public:
    TbagPacket();
    virtual ~TbagPacket();
};

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__

