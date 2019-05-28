/**
 * @file   Tmx.cpp
 * @brief  Tmx class implementation.
 * @author zer0
 * @date   2019-05-29
 */

#include <libtbag/tiled/Tmx.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

Tmx::Tmx()
{
    // EMPTY.
}

Tmx::Tmx(Tmx const & obj) : Tmx()
{
    (*this) = obj;
}

Tmx::Tmx(Tmx && obj) TBAG_NOEXCEPT : Tmx()
{
    (*this) = std::move(obj);
}

Tmx::~Tmx()
{
    // EMPTY.
}

Tmx & Tmx::operator =(Tmx const & obj)
{
    copy(obj);
    return *this;
}

Tmx & Tmx::operator =(Tmx && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Tmx::copy(Tmx const & obj)
{
    if (this != &obj) {
        // TODO: Copy object.
    }
}

void Tmx::swap(Tmx & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // TODO: Swap object.
    }
}

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

