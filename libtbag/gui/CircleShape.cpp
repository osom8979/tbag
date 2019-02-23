/**
 * @file   CircleShape.cpp
 * @brief  CircleShape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/CircleShape.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

CircleShape::CircleShape() : Shape(SfType::ST_CIRCLE_SHAPE)
{
    assert(ptr != nullptr);
}

CircleShape::CircleShape(CircleShape && obj) TBAG_NOEXCEPT
        : Shape(SfType::ST_CIRCLE_SHAPE, no_init_no_ref)
{
    *this = std::move(obj);
}

CircleShape::~CircleShape()
{
    // EMPTY.
}

CircleShape & CircleShape::operator =(CircleShape && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void CircleShape::swap(CircleShape & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

