/**
 * @file   TmxAnimation.cpp
 * @brief  TmxAnimation class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/tiled/details/TmxAnimation.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxAnimation::TmxAnimation() : TmxTag(TAG_NAME)
{
    // EMPTY.
}

TmxAnimation::TmxAnimation(Frames const & f)
        : TmxTag(TAG_NAME), frames(f)
{
    // EMPTY.
}

TmxAnimation::~TmxAnimation()
{
    // EMPTY.
}

bool TmxAnimation::empty() const
{
    return frames.empty();
}

std::size_t TmxAnimation::size() const
{
    return frames.size();
}

void TmxAnimation::clear()
{
    frames.clear();
}

Err TmxAnimation::onRead(Element const & elem)
{
    read(elem, frames);
    return E_SUCCESS;
}

Err TmxAnimation::onWrite(Element & elem) const
{
    write(elem, frames);
    return E_SUCCESS;
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

