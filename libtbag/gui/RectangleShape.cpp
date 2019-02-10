/**
 * @file   RectangleShape.cpp
 * @brief  RectangleShape class implementation.
 * @author zer0
 * @date   2019-02-10
 */

#include <libtbag/gui/RectangleShape.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Graphics/RectangleShape.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

struct RectangleShape::Impl
{
    sf::RectangleShape shape;
};

// -----------------------------
// RectangleShape implementation
// -----------------------------

RectangleShape::RectangleShape() : _impl(std::make_unique<Impl>())
{
    // EMPTY.
}

RectangleShape::~RectangleShape()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

