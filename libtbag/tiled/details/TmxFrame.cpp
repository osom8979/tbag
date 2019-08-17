/**
 * @file   TmxFrame.cpp
 * @brief  TmxFrame class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/tiled/details/TmxFrame.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxFrame::TmxFrame() : TmxTag(TAG_NAME)
{
    // EMPTY.
}

TmxFrame::TmxFrame(int t, int d) : TmxTag(TAG_NAME), tileid(t), duration(d)
{
    // EMPTY.
}

TmxFrame::~TmxFrame()
{
    // EMPTY.
}

Err TmxFrame::onRead(Element const & elem)
{
    optAttr(elem, ATT_TILEID, tileid);
    optAttr(elem, ATT_DURATION, duration);
    return E_SUCCESS;
}

Err TmxFrame::onWrite(Element & elem) const
{
    setAttr(elem, ATT_TILEID, tileid);
    setAttr(elem, ATT_DURATION, duration);
    return E_SUCCESS;
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

