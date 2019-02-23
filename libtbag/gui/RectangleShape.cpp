/**
 * @file   RectangleShape.cpp
 * @brief  RectangleShape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/RectangleShape.hpp>
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

RectangleShape::RectangleShape() : SfNative(SfType::ST_RECTANGLE_SHAPE)
{
    assert(ptr != nullptr);
}

RectangleShape::RectangleShape(RectangleShape && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_RECTANGLE_SHAPE, no_init_no_ref)
{
    *this = std::move(obj);
}

RectangleShape::~RectangleShape()
{
    // EMPTY.
}

RectangleShape & RectangleShape::operator =(RectangleShape && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void RectangleShape::swap(RectangleShape & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

