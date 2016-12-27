/**
 * @file   Stream.cpp
 * @brief  Stream class implementation.
 * @author zer0
 * @date   2016-12-27
 */

#include <libtbag/uv/Stream.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

Stream::Stream(UvHandleType type) : Handle(type)
{
    if (isStream() == false) {
        __tbag_error("Stream::Stream({}) type is not stream type", static_cast<int>(type));
        throw std::bad_alloc();
    }
}

Stream::~Stream()
{
    // EMPTY.
}

bool Stream::isReadable() const TBAG_NOEXCEPT
{
    return ::uv_is_readable(Parent::castNative<const uv_stream_t>()) == 1;
}

bool Stream::isWritable() const TBAG_NOEXCEPT
{
    return ::uv_is_writable(Parent::castNative<const uv_stream_t>()) == 1;
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

