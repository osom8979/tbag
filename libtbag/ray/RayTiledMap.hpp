/**
 * @file   RayTiledMap.hpp
 * @brief  RayTiledMap class prototype.
 * @author zer0
 * @date   2020-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYTILEDMAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYTILEDMAP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/tiled/TiledMap.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

/**
 * RayTiledMap class prototype.
 *
 * @author zer0
 * @date   2020-01-07
 */
class TBAG_API RayTiledMap : private Noncopyable
{
public:
    using Err = libtbag::Err;
    using TiledMap = libtbag::tiled::TiledMap;

private:
    TiledMap _tiled;

public:
    RayTiledMap();
    virtual ~RayTiledMap();
};

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYTILEDMAP_HPP__

