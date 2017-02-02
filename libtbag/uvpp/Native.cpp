/**
 * @file   Native.cpp
 * @brief  Native class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Native.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

Native::Native(UvType t) : TYPE(t), _user(nullptr)
{
    // @formatter:off
    switch (TYPE) {
#define _TBAG_XX(name, type)                    \
        case UvType::name:                      \
            ptr = ::malloc(sizeof(type));       \
            ::memset(ptr, 0x00, sizeof(type));  \
            return;
    TBAG_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    default:
        __tbag_error("Native::Native({}) Unknown uv type error.", static_cast<int>(TYPE));
        return;
    }
    // @formatter:on
}

Native::Native(int type) : Native(static_cast<UvType>(type))
{
    // EMPTY.
}

Native::~Native()
{
    if (ptr != nullptr) {
        ::free(ptr);
        ptr = nullptr;
    }
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

