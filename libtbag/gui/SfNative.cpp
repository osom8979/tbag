/**
 * @file   SfNative.cpp
 * @brief  SfNative class implementation.
 * @author zer0
 * @date   2019-02-12
 */

#include <libtbag/gui/SfNative.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#if defined(USE_GUI)
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

SfNative::SfNative(SfType type, no_init_t, bool ref)
        : Parent(nullptr), _type(type), _ref(ref)
{
    assert(ptr == nullptr);
}

SfNative::SfNative(SfType type, bool ref)
        : Parent(newSfType(type)), _type(type), _ref(ref)
{
    assert(ptr != nullptr);
}

SfNative::SfNative(SfNative && obj) TBAG_NOEXCEPT
        : Parent(nullptr), _type(obj._type), _ref(false)
{
    *this = std::move(obj);
}

SfNative::~SfNative()
{
    if (_ref && ptr != nullptr) {
        deleteSfType(_type, ptr);
        ptr = nullptr;
    }
}

SfNative & SfNative::operator =(SfNative && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SfNative::swap(SfNative & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(ptr, obj.ptr);
        std::swap(_type, obj._type);
        std::swap(_ref, obj._ref);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

