/**
 * @file   SfCommon.cpp
 * @brief  SfCommon class implementation.
 * @author zer0
 * @date   2019-02-12
 */

#include <libtbag/gui/SfCommon.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfType getSfType(int type) TBAG_NOEXCEPT
{
    assert(static_cast<int>(SfType::__START_NUMBER__) < type);
    assert(static_cast<int>(SfType::__SIZE__) > type);
    return static_cast<SfType>(type);
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

