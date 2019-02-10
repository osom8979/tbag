/**
 * @file   CircleShape.cpp
 * @brief  CircleShape class implementation.
 * @author zer0
 * @date   2019-02-10
 */

#include <libtbag/gui/CircleShape.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Graphics/CircleShape.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

struct CircleShape::Impl
{
    sf::CircleShape shape;
};

// --------------------------
// CircleShape implementation
// --------------------------

CircleShape::CircleShape() : _impl(std::make_unique<Impl>())
{
    // EMPTY.
}

CircleShape::~CircleShape()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

