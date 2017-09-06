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

#include <libtbag/proto/fbs/tbag_generated.h>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/geometry/Rect.hpp>
#include <libtbag/geometry/Size.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

TBAG_API util::Version toVersion(proto::fbs::tbag::Version const & version);
TBAG_API proto::fbs::tbag::Version fromVersion(util::Version const & version);

TBAG_API geometry::Point toPoint(proto::fbs::tbag::Point const & point);
TBAG_API proto::fbs::tbag::Point fromPoint(geometry::Point const & point);

TBAG_API geometry::Size toSize(proto::fbs::tbag::Size const & size);
TBAG_API proto::fbs::tbag::Size fromSize(geometry::Size const & size);

TBAG_API geometry::Rect toRect(proto::fbs::tbag::Rect const & rect);
TBAG_API proto::fbs::tbag::Rect fromRect(geometry::Rect const & rect);

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__

