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

Animation::Animation() : _interpolator(), _fill_after(false), _fill_before(false),
                         _repeat_mode(RepeatMode::RM_RESTART), _repeat_count(INFINITE_REPEAT),
                         _duration(0), _start_offset(0),
                         _is_started(false),
                         _start(), _repeat(0)
{
    // EMPTY.
}

Animation::~Animation()
{
    // EMPTY.
}

void Animation::clear()
{
    _interpolator.reset();
    _fill_after   = false;
    _fill_before  = false;
    _repeat_mode  = RepeatMode::RM_RESTART;
    _repeat_count = INFINITE_REPEAT;
    _duration     = 0;
    _start_offset = 0;
    _is_started   = false;
    _start        = 0;
    _repeat       = 0;
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

