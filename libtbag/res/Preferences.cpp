/**
 * @file   Preferences.cpp
 * @brief  Preferences class implementation.
 * @author zer0
 * @date   2019-10-22
 */

#include <libtbag/res/Preferences.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

Preferences::Preferences() : _model(std::make_shared<DefaultXmlModel>())
{
    assert(static_cast<bool>(_model));
}

Preferences::Preferences(std::nullptr_t) TBAG_NOEXCEPT : _model(nullptr)
{
    // EMPTY.
}

Preferences::Preferences(Preferences const & obj) TBAG_NOEXCEPT : Preferences(nullptr)
{
    (*this) = obj;
}

Preferences::Preferences(Preferences && obj) TBAG_NOEXCEPT : Preferences(nullptr)
{
    (*this) = std::move(obj);
}

Preferences::~Preferences()
{
    // EMPTY.
}

Preferences & Preferences::operator =(Preferences const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Preferences & Preferences::operator =(Preferences && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Preferences::copy(Preferences const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _model = obj._model;
    }
}

void Preferences::swap(Preferences & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _model.swap(obj._model);
    }
}

void Preferences::reset()
{
    _model.reset();
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

