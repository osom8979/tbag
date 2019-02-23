/**
 * @file   VertexArray.cpp
 * @brief  VertexArray class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/VertexArray.hpp>
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

VertexArray::VertexArray() : Drawable(SfType::ST_VERTEX_ARRAY)
{
    assert(ptr != nullptr);
}

VertexArray::VertexArray(VertexArray && obj) TBAG_NOEXCEPT
        : Drawable(SfType::ST_VERTEX_ARRAY, no_init_no_ref)
{
    *this = std::move(obj);
}

VertexArray::~VertexArray()
{
    // EMPTY.
}

VertexArray & VertexArray::operator =(VertexArray && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void VertexArray::swap(VertexArray & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

