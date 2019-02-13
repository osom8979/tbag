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

char const * const getSfTypeName(SfType type) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (type) {
#define _TBAG_XX(name, type) \
        case SfType::name: return #name;
    TBAG_SF_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "UNKNOWN";
    }
    // @formatter:on
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

