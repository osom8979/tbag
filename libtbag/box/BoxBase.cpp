/**
 * @file   BoxBase.cpp
 * @brief  BoxBase class implementation.
 * @author zer0
 * @date   2020-01-01
 */

#include <libtbag/box/BoxBase.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

BoxBase::BoxBase() : _base(std::make_shared<box_data>())
{
    // EMPTY.
}

BoxBase::BoxBase(std::nullptr_t) TBAG_NOEXCEPT : _base(nullptr)
{
    // EMPTY.
}

BoxBase::BoxBase(box_data && box) TBAG_NOEXCEPT : _base(std::make_shared<box_data>(std::move(box)))
{
    // EMPTY.
}

BoxBase::BoxBase(SharedBoxData const & box) TBAG_NOEXCEPT : _base(box)
{
    // EMPTY.
}

BoxBase::BoxBase(SharedBoxData && box) TBAG_NOEXCEPT : _base(std::move(box))
{
    // EMPTY.
}

BoxBase::BoxBase(BoxBase const & obj) TBAG_NOEXCEPT : _base(obj._base)
{
    // EMPTY.
}

BoxBase::BoxBase(BoxBase && obj) TBAG_NOEXCEPT : _base(std::move(obj._base))
{
    // EMPTY.
}

BoxBase::~BoxBase()
{
    // EMPTY.
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

