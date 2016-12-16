/**
 * @file   UvOutStream.cpp
 * @brief  UvOutStream class implementation.
 * @author zer0
 * @date   2016-12-16
 */

#include <libtbag/stream/UvOutStream.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace stream {

UvOutStream::UvOutStream(Type type) : util::UvNative(type)
{
    // EMPTY.
}

UvOutStream::~UvOutStream()
{
    // EMPTY.
}

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

