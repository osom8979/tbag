/**
 * @file   Native.cpp
 * @brief  Native class implementation.
 * @author zer0
 * @date   2016-12-27
 */

#include <libtbag/uv/Native.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

Native::Native(Type t) : TYPE(t), _native(nullptr), _user(nullptr)
{
    // @formatter:off
    switch (TYPE) {
#define _TBAG_XX(name, type)                        \
        case UvType::name:                          \
            _native = ::malloc(sizeof(type));       \
            ::memset(_native, 0x00, sizeof(type));  \
            return;
    TBAG_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    }
    // @formatter:on

    __tbag_error("Native::Native({}) Unknown uv type error.", static_cast<int>(TYPE));
}

Native::~Native()
{
    if (_native != nullptr) {
        ::free(_native);
        _native = nullptr;
    }
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

