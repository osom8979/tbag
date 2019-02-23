/**
 * @file   VertexBuffer.cpp
 * @brief  VertexBuffer class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/VertexBuffer.hpp>
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

VertexBuffer::VertexBuffer() : Drawable(SfType::ST_VERTEX_BUFFER)
{
    assert(ptr != nullptr);
}

VertexBuffer::VertexBuffer(VertexBuffer && obj) TBAG_NOEXCEPT
        : Drawable(SfType::ST_VERTEX_BUFFER, no_init_no_ref)
{
    *this = std::move(obj);
}

VertexBuffer::~VertexBuffer()
{
    // EMPTY.
}

VertexBuffer & VertexBuffer::operator =(VertexBuffer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void VertexBuffer::swap(VertexBuffer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

