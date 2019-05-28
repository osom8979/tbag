/**
 * @file   TmxJson.hpp
 * @brief  TmxJson class prototype.
 * @author zer0
 * @date   2019-05-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMXJSON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMXJSON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

/**
 * TmxJson class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 *
 * @see <https://doc.mapeditor.org/en/stable/reference/json-map-format/>
 */
class TBAG_API TmxJson
{
public:
    TmxJson();
    TmxJson(TmxJson const & obj);
    TmxJson(TmxJson && obj) TBAG_NOEXCEPT;
    ~TmxJson();

public:
    TmxJson & operator =(TmxJson const & obj);
    TmxJson & operator =(TmxJson && obj) TBAG_NOEXCEPT;

public:
    void copy(TmxJson const & obj);
    void swap(TmxJson & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(TmxJson & lh, TmxJson & rh) TBAG_NOEXCEPT { lh.swap(rh); }
};

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMXJSON_HPP__

