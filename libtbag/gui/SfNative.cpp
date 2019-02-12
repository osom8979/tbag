/**
 * @file   SfNative.cpp
 * @brief  SfNative class implementation.
 * @author zer0
 * @date   2019-02-12
 */

#include <libtbag/gui/SfNative.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfNative::SfNative(SfType type) : TYPE(type), _user(nullptr)
{
    // EMPTY.
}

SfNative::SfNative(int type) : TYPE(getSfType(type)), _user(nullptr)
{
    // EMPTY.
}

SfNative::~SfNative()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

