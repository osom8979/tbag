/**
 * @file   Text.cpp
 * @brief  Text class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/Text.hpp>
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

Text::Text() : Drawable(SfType::ST_TEXT)
{
    assert(ptr != nullptr);
}

Text::Text(Text && obj) TBAG_NOEXCEPT
        : Drawable(SfType::ST_TEXT, no_init_no_ref)
{
    *this = std::move(obj);
}

Text::~Text()
{
    // EMPTY.
}

Text & Text::operator =(Text && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Text::swap(Text & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

