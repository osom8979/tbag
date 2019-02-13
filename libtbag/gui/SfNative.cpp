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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfNative::SfNative(SfType type, no_init_t) : TYPE(type), _user(nullptr)
{
    // EMPTY.
}

SfNative::SfNative(SfType type) : TYPE(type), _user(nullptr)
{
    // @formatter:off
    switch (TYPE) {
#define _TBAG_XX(name, type) \
        case SfType::name: ptr = new sf::type(); break;
    TBAG_SF_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    default:
        tDLogE("SfNative::SfNative({}) Unknown sf type error.", static_cast<int>(TYPE));
        throw std::bad_alloc();
    }
    // @formatter:on
}

SfNative::SfNative(int type) : SfNative(getSfType(type))
{
    // EMPTY.
}

SfNative::~SfNative()
{
    if (ptr != nullptr) {
        // @formatter:off
        switch (TYPE) {
#define _TBAG_XX(name, type) \
            case SfType::name: delete ((sf::type*)ptr); break;
        TBAG_SF_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            break;
        }
        // @formatter:on
        ptr = nullptr;
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

