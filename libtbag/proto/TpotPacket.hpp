/**
 * @file   TpotPacket.hpp
 * @brief  TpotPacket class prototype.
 * @author zer0
 * @date   2017-09-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__

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
 * TpotPacket class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TpotPacket : private Noncopyable
{
private:
    // Insert member variables.

public:
    TpotPacket();
    virtual ~TpotPacket();
};

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__

