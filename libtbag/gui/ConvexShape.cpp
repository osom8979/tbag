/**
 * @file   ConvexShape.cpp
 * @brief  ConvexShape class implementation.
 * @author zer0
 * @date   2019-02-10
 */

#include <libtbag/gui/ConvexShape.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Graphics/ConvexShape.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

struct ConvexShape::Impl
{
    sf::ConvexShape shape;
};

// --------------------------
// ConvexShape implementation
// --------------------------

ConvexShape::ConvexShape() : _impl(std::make_unique<Impl>())
{
    // EMPTY.
}

ConvexShape::~ConvexShape()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

