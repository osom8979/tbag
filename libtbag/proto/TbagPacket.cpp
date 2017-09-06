/**
 * @file   TbagPacket.cpp
 * @brief  TbagPacket class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#include <libtbag/proto/TbagPacket.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

util::Version toVersion(proto::fbs::tbag::Version const & version)
{
    return util::Version(version.major(), version.minor(), version.patch());
}

proto::fbs::tbag::Version fromVersion(util::Version const & version)
{
    return proto::fbs::tbag::Version(version.getMajor(), version.getMinor(), version.getPatch());
}

geometry::Point toPoint(proto::fbs::tbag::Point const & point)
{
    return geometry::makePoint<int>(point.x(), point.y());
}

proto::fbs::tbag::Point fromPoint(geometry::Point const & point)
{
    return proto::fbs::tbag::Point(point.x, point.y);
}

geometry::Size toSize(proto::fbs::tbag::Size const & size)
{
    return geometry::makeSize<int>(size.width(), size.height());
}

proto::fbs::tbag::Size fromSize(geometry::Size const & size)
{
    return proto::fbs::tbag::Size(size.width, size.height);
}

geometry::Rect toRect(proto::fbs::tbag::Rect const & rect)
{
    return geometry::makeRect<int>(rect.x(), rect.y(), rect.width(), rect.height());
}

proto::fbs::tbag::Rect fromRect(geometry::Rect const & rect)
{
    return proto::fbs::tbag::Rect(rect.x, rect.y, rect.width, rect.height);
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

