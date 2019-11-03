/**
 * @file   Preferences.cpp
 * @brief  Preferences class implementation.
 * @author zer0
 * @date   2019-11-03
 */

#include <libtbag/pref/Preferences.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

Preferences::Preferences()
{
    // EMPTY.
}

Preferences::Preferences(Preferences const & obj) : Preferences()
{
    (*this) = obj;
}

Preferences::Preferences(Preferences && obj) TBAG_NOEXCEPT : Preferences()
{
    (*this) = std::move(obj);
}

Preferences::~Preferences()
{
    // EMPTY.
}

Preferences & Preferences::operator =(Preferences const & obj)
{
    copy(obj);
    return *this;
}

Preferences & Preferences::operator =(Preferences && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Preferences::copy(Preferences const & obj)
{
    if (this != &obj) {
        // TODO: Copy object.
    }
}

void Preferences::swap(Preferences & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // TODO: Swap object.
    }
}

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

