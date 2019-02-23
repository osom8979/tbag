/**
 * @file   ConvexShape.cpp
 * @brief  ConvexShape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/ConvexShape.hpp>
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

ConvexShape::ConvexShape() : Shape(SfType::ST_CONVEX_SHAPE)
{
    assert(ptr != nullptr);
}

ConvexShape::ConvexShape(ConvexShape && obj) TBAG_NOEXCEPT
        : Shape(SfType::ST_CONVEX_SHAPE, no_init_no_ref)
{
    *this = std::move(obj);
}

ConvexShape::~ConvexShape()
{
    // EMPTY.
}

ConvexShape & ConvexShape::operator =(ConvexShape && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ConvexShape::swap(ConvexShape & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

