/**
 * @file   Animation.cpp
 * @brief  Animation class implementation.
 * @author zer0
 * @date   2017-11-22
 */

#include <libtbag/animation/Animation.hpp>
#include <libtbag/log/Log.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

Animation::Animation()
{
    // EMPTY.
}

Animation::Animation(Animation const & obj)
{
    (*this) = obj;
}

Animation::Animation(Animation && obj)
{
    (*this) = std::move(obj);
}

Animation::~Animation()
{
    // EMPTY.
}

Animation & Animation::operator =(Animation const & obj)
{
    if (this != &obj) {
    }
    return *this;
}

Animation & Animation::operator =(Animation && obj)
{
    swap(obj);
    return *this;
}

void Animation::swap(Animation & obj)
{
    if (this != &obj) {
        _interpolator.swap(obj._interpolator);
    }
}

bool Animation::isFinished() const
{
    return true;
}

float Animation::getInterpolation(float input)
{
    return 0;
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

